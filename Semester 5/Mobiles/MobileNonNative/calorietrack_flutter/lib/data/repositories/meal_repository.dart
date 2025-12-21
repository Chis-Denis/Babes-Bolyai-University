import 'dart:async';
import 'package:flutter/foundation.dart';
import 'package:http/http.dart' as http;
import '../models/meal.dart';
import '../models/pending_operation.dart';
import '../local/database_helper.dart';
import '../../network/server_api_service.dart' show ServerApiService, ServerException;

class MealRepository {
  static final MealRepository _instance = MealRepository._internal();
  factory MealRepository() => _instance;
  MealRepository._internal();

  final DatabaseHelper _dbHelper = DatabaseHelper();
  final ServerApiService _serverApi = ServerApiService();
  final List<Meal> _meals = [];
  bool _isInitialized = false;
  bool _isServerOnline = false;
  final _mealsController = StreamController<List<Meal>>.broadcast();
  final _serverStatusController = StreamController<bool>.broadcast();
  StreamSubscription? _webSocketSubscription;
  Timer? _syncTimer;

  Stream<List<Meal>> get mealsStream => _mealsController.stream;
  Stream<bool> get serverStatusStream => _serverStatusController.stream;
  bool get isServerOnline => _isServerOnline;
  List<Meal> get meals => List.unmodifiable(_meals);
  bool get isInitialized => _isInitialized;

  Future<void> initialize() async {
    if (_isInitialized) {
      _log('Repository already initialized');
      return;
    }
    
    _log('Initializing MealRepository...');
    
    await loadMealsFromLocal();
    
    try {
      await _serverApi.connectWebSocket();
      _webSocketSubscription = _serverApi.webSocketStream.listen(
        (message) {
          _handleWebSocketMessage(message);
        },
        onError: (error) {
          _logError('WebSocket stream error', error);
        },
      );
      _log('WebSocket connected successfully');
    } catch (e) {
      _log('WebSocket connection failed (server may be offline), will retry later');
    }
    
    final hasPendingOps = await _dbHelper.getPendingOperationsCount() > 0;
    if (hasPendingOps) {
      _log('Found pending operations, checking server connectivity...');
      bool serverConnected = false;
      for (int attempt = 0; attempt < 3; attempt++) {
        try {
          await _serverApi.getAllMeals();
          _setServerOnline(true);
          serverConnected = true;
          _log('Server is online, syncing pending operations...');
          await _syncPendingOperations();
          _log('Refreshing from server after sync to get any other changes...');
          await loadMealsFromServer();
          break;
        } catch (e) {
          if (attempt < 2) {
            _log('Server not available (attempt ${attempt + 1}/3), retrying in 2 seconds...');
            await Future.delayed(const Duration(seconds: 2));
          } else {
            _log('Server not available after 3 attempts, will sync later via timer');
            _setServerOnline(false);
          }
        }
      }
    } else {
      try {
        await loadMealsFromServer();
        _setServerOnline(true);
      } catch (e) {
        _log('Server not available, using local data');
        _setServerOnline(false);
      }
    }
    
    _isInitialized = true;
    _log('MealRepository initialized successfully');
    
    _startSyncTimer();
    
    Future.delayed(const Duration(seconds: 2), () async {
      if (!_isServerOnline) {
        _log('Doing immediate server check after initialization delay...');
        await _checkServerAndSync();
      }
    });
  }

  void _setServerOnline(bool online) {
    if (_isServerOnline != online) {
      _isServerOnline = online;
      _serverStatusController.add(online);
      _log('Server status changed: ${online ? "ONLINE" : "OFFLINE"}');
    }
  }

  void _startSyncTimer() {
    _syncTimer?.cancel();
    _syncTimer = Timer.periodic(const Duration(seconds: 5), (timer) async {
      await _checkServerAndSync();
      
      if (_isServerOnline && _webSocketSubscription == null) {
        try {
          await _serverApi.connectWebSocket();
          _webSocketSubscription = _serverApi.webSocketStream.listen(
            (message) {
              _handleWebSocketMessage(message);
            },
            onError: (error) {
              _logError('WebSocket stream error', error);
            },
          );
          _log('WebSocket reconnected');
        } catch (e) {
          // Ignore WebSocket errors
        }
      }
    });
  }

  Future<void> loadMealsFromServer() async {
    try {
      _log('Loading meals from server...');
      final serverMeals = await _serverApi.getAllMeals();
      
      final pendingDeletes = await _dbHelper.getAllPendingOperations();
      final deleteIds = pendingDeletes
          .where((op) => op.operationType == 'delete' && op.mealId != null && op.mealId! > 0)
          .map((op) => op.mealId!)
          .toSet();
      
      final localMeals = await _dbHelper.getAllMeals();
      final tempMealIds = localMeals.where((m) => m.mealId < 0).map((m) => m.mealId).toSet();
      
      for (final serverMeal in serverMeals) {
        if (deleteIds.contains(serverMeal.mealId)) {
          continue;
        }
        
        final index = _meals.indexWhere((m) => m.mealId == serverMeal.mealId);
        if (index != -1) {
          if (_meals[index].mealId > 0) {
            _meals[index] = serverMeal;
            try {
              await _dbHelper.updateMeal(serverMeal);
            } catch (e) {
              try {
                await _dbHelper.insertMeal(serverMeal);
              } catch (e2) {
                _log('Meal already exists in local DB, skipping');
              }
            }
          } else {
            _log('Preserving temp meal ${_meals[index].mealId} - will be synced later');
          }
        } else {
          final targetDate = serverMeal.date;
          final mightBeTempMeal = localMeals.any(
            (m) => m.mealName == serverMeal.mealName &&
                   m.date.year == targetDate.year &&
                   m.date.month == targetDate.month &&
                   m.date.day == targetDate.day &&
                   m.mealId < 0,
          );
          
          if (!mightBeTempMeal) {
            _meals.add(serverMeal);
            try {
              await _dbHelper.insertMeal(serverMeal);
            } catch (e) {
              await _dbHelper.updateMeal(serverMeal);
            }
          } else {
            _log('Skipping server meal that might match a temp meal - will be handled during sync');
          }
        }
      }
      
      _meals.sort((a, b) => b.date.compareTo(a.date));
      
      _notifyListeners();
      _setServerOnline(true);
      _log('Successfully merged ${serverMeals.length} meals from server with local data');
    } catch (e) {
      _logError('Error loading meals from server', e);
      _setServerOnline(false);
      rethrow;
    }
  }

  Future<void> _checkServerAndSync() async {
    try {
      await _serverApi.getAllMeals();
      final wasOffline = !_isServerOnline;
      
      if (wasOffline) {
        _setServerOnline(true);
        final hasPendingOps = await _dbHelper.getPendingOperationsCount() > 0;
        if (hasPendingOps) {
          _log('Server came online, syncing pending operations...');
          await _syncPendingOperations();
          await loadMealsFromServer();
        } else {
          await loadMealsFromServer();
        }
      } else {
        final hasPendingOps = await _dbHelper.getPendingOperationsCount() > 0;
        if (hasPendingOps) {
          _log('Server is online, checking for pending operations to sync...');
          await _syncPendingOperations();
        }
      }
    } catch (e) {
      if (_isServerOnline) {
        _setServerOnline(false);
        _log('Server went offline');
      }
    }
  }

  Future<void> loadMealsFromLocal() async {
    try {
      _log('Loading meals from local database...');
      final localMeals = await _dbHelper.getAllMeals();
      _meals.clear();
      _meals.addAll(localMeals);
      _notifyListeners();
      _log('Successfully loaded ${localMeals.length} meals from local database');
    } catch (e) {
      _logError('Error loading meals from local database', e);
      rethrow;
    }
  }

  Future<void> _syncToLocalDatabase(List<Meal> meals) async {
    try {
      _log('Syncing ${meals.length} meals to local database...');
      
      final localMeals = await _dbHelper.getAllMeals();
      final tempMeals = localMeals.where((m) => m.mealId < 0).toList();
      
      final serverMealIds = meals.map((m) => m.mealId).where((id) => id > 0).toList();
      for (final id in serverMealIds) {
        await _dbHelper.deleteMeal(id).catchError((e) {});
      }
      
      for (final meal in meals) {
        await _dbHelper.insertMeal(meal).catchError((e) async {
          await _dbHelper.updateMeal(meal);
        });
      }
      
      _log('Successfully synced meals to local database (preserved ${tempMeals.length} temp meals)');
    } catch (e) {
      _logError('Error syncing to local database', e);
    }
  }

  void _handleWebSocketMessage(Map<String, dynamic> message) {
    try {
      final type = message['type'] as String?;
      _log('Received WebSocket message: $type', message);

      switch (type) {
        case 'meal_created':
          final mealData = message['meal'] as Map<String, dynamic>;
          final meal = _mealFromServerJson(mealData);
          _meals.add(meal);
          _meals.sort((a, b) => b.date.compareTo(a.date));
          _notifyListeners();
          _log('Meal created via WebSocket', meal);
          break;

        case 'meal_updated':
          final mealData = message['meal'] as Map<String, dynamic>;
          final updatedMeal = _mealFromServerJson(mealData);
          final index = _meals.indexWhere((m) => m.mealId == updatedMeal.mealId);
          if (index != -1) {
            _meals[index] = updatedMeal;
            _notifyListeners();
            _log('Meal updated via WebSocket', updatedMeal);
          }
          break;

        case 'meal_deleted':
          final mealId = message['mealId'] as int;
          _meals.removeWhere((m) => m.mealId == mealId);
          _notifyListeners();
          _log('Meal deleted via WebSocket', {'mealId': mealId});
          break;

        default:
          _log('Unknown WebSocket message type: $type');
      }
    } catch (e) {
      _logError('Error handling WebSocket message', e);
    }
  }

  Future<Meal> addMeal({
    required String mealName,
    required double calories,
    required double protein,
    required double carbs,
    required double fat,
    DateTime? date,
    String? notes,
  }) async {
    final mealDate = date ?? DateTime.now();
    
    // Create meal data
    final mealData = {
      'mealName': mealName,
      'calories': calories,
      'protein': protein,
      'carbs': carbs,
      'fat': fat,
      'date': mealDate.toIso8601String(),
      'notes': notes,
    };

    _log('Adding new meal', mealData);

    if (_isServerOnline) {
      try {
        final meal = await _serverApi.createMeal(
          mealName: mealName,
          calories: calories,
          protein: protein,
          carbs: carbs,
          fat: fat,
          date: mealDate,
          notes: notes,
        );

        if (!_meals.any((m) => m.mealId == meal.mealId)) {
          _meals.add(meal);
          _meals.sort((a, b) => b.date.compareTo(a.date));
          _notifyListeners();
        }

        await _dbHelper.insertMeal(meal);

        _log('Meal added successfully on server', meal);
        return meal;
      } catch (e) {
        _logError('Error adding meal on server, will queue for offline', e);
        _setServerOnline(false);
      }
    }

    _log('Saving meal locally (offline mode)');
    
    final tempId = -DateTime.now().millisecondsSinceEpoch;
    final localMeal = Meal(
      mealId: tempId,
      mealName: mealName,
      calories: calories,
      protein: protein,
      carbs: carbs,
      fat: fat,
      date: mealDate,
      notes: notes,
    );

    await _dbHelper.insertMeal(localMeal);

    _meals.add(localMeal);
    _meals.sort((a, b) => b.date.compareTo(a.date));
    _notifyListeners();

    final pendingOp = PendingOperation(
      operationType: 'create',
      mealId: tempId,
      mealData: mealData,
      timestamp: DateTime.now(),
    );
    await _dbHelper.addPendingOperation(pendingOp);

    _log('Meal saved locally and queued for sync', localMeal);
    return localMeal;
  }

  Future<void> updateMeal(Meal updatedMeal) async {
    _log('Updating meal', updatedMeal);

    if (_isServerOnline) {
      try {
        final serverMeal = await _serverApi.updateMeal(updatedMeal);

        final index = _meals.indexWhere((m) => m.mealId == serverMeal.mealId);
        if (index != -1) {
          _meals[index] = serverMeal;
          _notifyListeners();
        }

        await _dbHelper.updateMeal(serverMeal);

        _log('Meal updated successfully on server', serverMeal);
        return;
      } catch (e) {
        _logError('Error updating meal on server, will queue for offline', e);
        _setServerOnline(false);
      }
    }

    _log('Updating meal locally (offline mode)');

    final index = _meals.indexWhere((m) => m.mealId == updatedMeal.mealId);
    if (index != -1) {
      _meals[index] = updatedMeal;
      _notifyListeners();
    }

    await _dbHelper.updateMeal(updatedMeal);

    final mealData = {
      'mealName': updatedMeal.mealName,
      'calories': updatedMeal.calories,
      'protein': updatedMeal.protein,
      'carbs': updatedMeal.carbs,
      'fat': updatedMeal.fat,
      'date': updatedMeal.date.toIso8601String(),
      'notes': updatedMeal.notes,
    };

    final pendingOp = PendingOperation(
      operationType: 'update',
      mealId: updatedMeal.mealId,
      mealData: mealData,
      timestamp: DateTime.now(),
    );
    await _dbHelper.addPendingOperation(pendingOp);

    _log('Meal updated locally and queued for sync', updatedMeal);
  }

  Future<void> removeMealById(int id) async {
    _log('Removing meal', {'mealId': id});

    if (_isServerOnline) {
      try {
        await _serverApi.deleteMeal(id);

        _meals.removeWhere((m) => m.mealId == id);
        _notifyListeners();

        await _dbHelper.deleteMeal(id);

        _log('Meal removed successfully from server', {'mealId': id});
        return;
      } catch (e) {
        _logError('Error removing meal from server, will queue for offline', e);
        _setServerOnline(false);
      }
    }

    _log('Removing meal locally (offline mode)');

    _meals.removeWhere((m) => m.mealId == id);
    _notifyListeners();

    await _dbHelper.deleteMeal(id);

    final pendingOp = PendingOperation(
      operationType: 'delete',
      mealId: id,
      mealData: null,
      timestamp: DateTime.now(),
    );
    await _dbHelper.addPendingOperation(pendingOp);

    _log('Meal removed locally and queued for sync', {'mealId': id});
  }

  Meal? findById(int id) {
    try {
      return _meals.firstWhere((m) => m.mealId == id);
    } catch (e) {
      return null;
    }
  }

  Future<void> clearAllData() async {
    _log('Clearing all data (local and server)...');
    
    try {
      await _dbHelper.deleteAllMeals();
      await _dbHelper.deleteAllPendingOperations();
      _meals.clear();
      _notifyListeners();
      _log('Cleared local database');
      
      if (_isServerOnline) {
        try {
          final response = await http.delete(
            Uri.parse('${_serverApi.baseUrl}/api/meals'),
            headers: {'Content-Type': 'application/json'},
          ).timeout(const Duration(seconds: 10));
          
          if (response.statusCode == 200) {
            _log('Cleared server database');
          } else {
            _logError('Failed to clear server database', response.statusCode);
          }
        } catch (e) {
          _logError('Error clearing server database', e);
        }
      }
      
      _log('All data cleared successfully');
    } catch (e) {
      _logError('Error clearing data', e);
      rethrow;
    }
  }

  Future<Meal?> findByIdAsync(int id) async {
    try {
      return await _dbHelper.getMealById(id);
    } catch (e) {
      return null;
    }
  }

  Future<int> getMealCount() async {
    return _meals.length;
  }

  Future<void> _syncPendingOperations() async {
    if (!_isServerOnline) {
      _log('Cannot sync: server is offline');
      return;
    }

    try {
      final pendingOps = await _dbHelper.getAllPendingOperations();
      if (pendingOps.isEmpty) {
        _log('No pending operations to sync');
        return;
      }

      _log('Syncing ${pendingOps.length} pending operations...');

      final createOps = pendingOps.where((op) => op.operationType == 'create').toList();
      final updateOps = pendingOps.where((op) => op.operationType == 'update').toList();
      final deleteOps = pendingOps.where((op) => op.operationType == 'delete').toList();

      _log('Operation breakdown: ${createOps.length} creates, ${updateOps.length} updates, ${deleteOps.length} deletes');

      for (final op in createOps) {
        try {
          switch (op.operationType) {
            case 'create':
              if (op.mealData != null) {
                Meal? localMeal;
                
                if (op.mealId != null) {
                  localMeal = await _dbHelper.getMealById(op.mealId!);
                }
                
                if (localMeal == null) {
                  final allMeals = await _dbHelper.getAllMeals();
                  try {
                    final targetDate = DateTime.parse(op.mealData!['date'] as String);
                    localMeal = allMeals.firstWhere(
                      (m) => m.mealName == op.mealData!['mealName'] &&
                             m.date.year == targetDate.year &&
                             m.date.month == targetDate.month &&
                             m.date.day == targetDate.day &&
                             m.mealId < 0,
                    );
                  } catch (e) {
                    try {
                      final targetDate = DateTime.parse(op.mealData!['date'] as String);
                      localMeal = _meals.firstWhere(
                        (m) => m.mealName == op.mealData!['mealName'] &&
                               m.date.year == targetDate.year &&
                               m.date.month == targetDate.month &&
                               m.date.day == targetDate.day &&
                               m.mealId < 0,
                      );
                    } catch (e2) {
                      _logError('Could not find local meal for create sync', e2);
                      _log('Skipping create sync - local meal not found and may have been deleted');
                      continue;
                    }
                  }
                }

                bool mealExistsOnServer = false;
                Meal? existingServerMeal;
                try {
                  final allServerMeals = await _serverApi.getAllMeals();
                  final targetDate = DateTime.parse(op.mealData!['date'] as String);
                  try {
                    existingServerMeal = allServerMeals.firstWhere(
                      (m) => m.mealName == op.mealData!['mealName'] &&
                             m.date.year == targetDate.year &&
                             m.date.month == targetDate.month &&
                             m.date.day == targetDate.day,
                    );
                    mealExistsOnServer = true;
                    _log('Meal already exists on server, will update local instead of creating', existingServerMeal);
                  } catch (e) {
                    mealExistsOnServer = false;
                  }
                } catch (e) {
                  _logError('Error checking if meal exists on server', e);
                }

                Meal serverMeal;
                if (mealExistsOnServer && existingServerMeal != null) {
                  serverMeal = existingServerMeal;
                  _log('Using existing server meal instead of creating duplicate', serverMeal);
                } else {
                  serverMeal = await _serverApi.createMeal(
                    mealName: op.mealData!['mealName'] as String,
                    calories: (op.mealData!['calories'] as num).toDouble(),
                    protein: (op.mealData!['protein'] as num).toDouble(),
                    carbs: (op.mealData!['carbs'] as num).toDouble(),
                    fat: (op.mealData!['fat'] as num).toDouble(),
                    date: DateTime.parse(op.mealData!['date'] as String),
                    notes: op.mealData!['notes'] as String?,
                  );
                }

                if (localMeal != null) {
                  final localIndex = _meals.indexWhere((m) => m.mealId == localMeal!.mealId);
                  if (localIndex != -1) {
                    _meals[localIndex] = serverMeal;
                  } else {
                    _meals.add(serverMeal);
                    _meals.sort((a, b) => b.date.compareTo(a.date));
                  }

                  try {
                    await _dbHelper.deleteMeal(localMeal.mealId);
                  } catch (e) {
                    _log('Temp meal already deleted from local DB');
                  }
                  try {
                    await _dbHelper.insertMeal(serverMeal);
                  } catch (e) {
                    await _dbHelper.updateMeal(serverMeal);
                  }
                  
                  _log('Synced create operation: replaced temp ID ${localMeal.mealId} with server ID ${serverMeal.mealId}');
                } else {
                  _meals.add(serverMeal);
                  _meals.sort((a, b) => b.date.compareTo(a.date));
                  try {
                    await _dbHelper.insertMeal(serverMeal);
                  } catch (e) {
                    await _dbHelper.updateMeal(serverMeal);
                  }
                  _log('Synced create operation: added new meal from server', serverMeal);
                }
              }
              break;
          }

          if (op.id != null) {
            await _dbHelper.deletePendingOperation(op.id!);
            _log('Removed pending operation ${op.id} from queue');
          }
        } catch (e) {
          _logError('Error syncing CREATE operation (mealId: ${op.mealId})', e);
        }
      }

      for (final op in updateOps) {
        try {
          switch (op.operationType) {
            case 'update':
              if (op.mealId != null && op.mealData != null) {
                // Find the meal (try in-memory first, then database)
                Meal? meal;
                try {
                  meal = _meals.firstWhere((m) => m.mealId == op.mealId);
                } catch (e) {
                  meal = await _dbHelper.getMealById(op.mealId!);
                }

                // If meal not found by ID, it might have been synced and got a new ID
                // Try to find it by matching data (for temp meals that were synced)
                if (meal == null && op.mealId! < 0) {
                  _log('Meal with temp ID ${op.mealId} not found, checking if it was synced...');
                  final allMeals = await _dbHelper.getAllMeals();
                  try {
                    final targetDate = DateTime.parse(op.mealData!['date'] as String);
                    meal = allMeals.firstWhere(
                      (m) => m.mealName == op.mealData!['mealName'] &&
                             m.date.year == targetDate.year &&
                             m.date.month == targetDate.month &&
                             m.date.day == targetDate.day &&
                             m.mealId > 0, // Now has real ID from create sync
                    );
                    _log('Found synced meal with real ID ${meal.mealId} for update operation');
                  } catch (e) {
                    // Still not found
                  }
                }

                if (meal == null) {
                  _logError('Could not find meal for update sync', {'mealId': op.mealId});
                  // Remove the pending operation since meal doesn't exist
                  if (op.id != null) {
                    await _dbHelper.deletePendingOperation(op.id!);
                  }
                  continue; // Skip to next operation
                }

                // Check if meal has temp ID (negative) - this means create hasn't synced yet
                if (meal.mealId < 0) {
                  _log('Update operation for temp meal ${meal.mealId} - create must sync first, skipping update');
                  // Don't remove the pending operation - it will be retried after create syncs
                  continue; // Skip this update, create operation will handle it
                }

                // Meal has real ID, proceed with update
                // Store meal in a local non-nullable variable since we know it's not null here
                final mealToUpdate = meal!;
                try {
                  // Update on server
                  final updatedMeal = await _serverApi.updateMeal(mealToUpdate);

                  // Update in-memory and local database
                  final index = _meals.indexWhere((m) => m.mealId == updatedMeal.mealId);
                  if (index != -1) {
                    _meals[index] = updatedMeal;
                  } else {
                    _meals.add(updatedMeal);
                    _meals.sort((a, b) => b.date.compareTo(a.date));
                  }
                  await _dbHelper.updateMeal(updatedMeal);

                  _log('Synced update operation', updatedMeal);
                } catch (e) {
                  // Check if meal doesn't exist on server (might have been deleted)
                  final errorMessage = e.toString().toLowerCase();
                  if (e is ServerException && 
                      (errorMessage.contains('not found') || 
                       errorMessage.contains('404') ||
                       e.message.toLowerCase().contains('not found'))) {
                    _log('Meal ${mealToUpdate.mealId} not found on server for update - may have been deleted, removing pending operation');
                    // Meal doesn't exist on server, remove the pending operation
                    if (op.id != null) {
                      await _dbHelper.deletePendingOperation(op.id!);
                    }
                    // Also remove from local if it exists
                    try {
                      await _dbHelper.deleteMeal(mealToUpdate.mealId);
                      _meals.removeWhere((m) => m.mealId == mealToUpdate.mealId);
                      _notifyListeners();
                    } catch (e2) {
                      // Ignore
                    }
                  } else {
                    // Real error, rethrow to be caught by outer try-catch
                    rethrow;
                  }
                }
              }
              break;
          }

          // Remove pending operation after successful sync
          if (op.id != null) {
            await _dbHelper.deletePendingOperation(op.id!);
            _log('Removed pending operation ${op.id} from queue');
          }
        } catch (e) {
          _logError('Error syncing UPDATE operation (mealId: ${op.mealId})', e);
          // Continue with next operation - don't remove from queue if sync failed
        }
      }

      // Finally, sync all DELETE operations
      for (final op in deleteOps) {
        try {
          switch (op.operationType) {
            case 'delete':
              if (op.mealId != null) {
                if (op.mealId! < 0) {
                  _log('Delete operation for temp meal ${op.mealId} - create must sync first, skipping delete');
                  Meal? localMeal;
                  try {
                    localMeal = await _dbHelper.getMealById(op.mealId!);
                  } catch (e) {
                  }
                  
                  if (localMeal == null) {
                    _log('Temp meal ${op.mealId} already deleted locally, removing pending delete operation');
                    if (op.id != null) {
                      await _dbHelper.deletePendingOperation(op.id!);
                    }
                  } else {
                    _log('Deleting temp meal ${op.mealId} locally (never synced to server)');
                    await _dbHelper.deleteMeal(op.mealId!);
                    _meals.removeWhere((m) => m.mealId == op.mealId);
                    _notifyListeners();
                    if (op.id != null) {
                      await _dbHelper.deletePendingOperation(op.id!);
                    }
                  }
                  continue;
                }

                try {
                  await _serverApi.deleteMeal(op.mealId!);
                  _log('Synced delete operation', {'mealId': op.mealId});
                } catch (e) {
                  final errorMessage = e.toString().toLowerCase();
                  if (e is ServerException && 
                      (errorMessage.contains('not found') || 
                       errorMessage.contains('404') ||
                       e.message.toLowerCase().contains('not found'))) {
                    _log('Meal ${op.mealId} already deleted on server, removing pending operation');
                  } else {
                    rethrow;
                  }
                }
              }
              break;
          }

          if (op.id != null) {
            await _dbHelper.deletePendingOperation(op.id!);
            _log('Removed pending operation ${op.id} from queue');
          }
        } catch (e) {
          _logError('Error syncing DELETE operation (mealId: ${op.mealId})', e);
        }
      }

      final syncedMeals = await _dbHelper.getAllMeals();
      
      for (final meal in syncedMeals) {
        final index = _meals.indexWhere((m) => m.mealId == meal.mealId);
        if (index != -1) {
          _meals[index] = meal;
        } else {
          _meals.add(meal);
        }
      }
      
      _meals.removeWhere((m) => !syncedMeals.any((dbMeal) => dbMeal.mealId == m.mealId));
      _meals.sort((a, b) => b.date.compareTo(a.date));
      _notifyListeners();
      _log('Finished syncing pending operations. Total meals: ${_meals.length}');
    } catch (e) {
      _logError('Error in syncPendingOperations', e);
      rethrow;
    }
  }

  Meal _mealFromServerJson(Map<String, dynamic> json) {
    return Meal(
      mealId: json['id'] as int,
      mealName: json['mealName'] as String,
      calories: (json['calories'] as num).toDouble(),
      protein: (json['protein'] as num).toDouble(),
      carbs: (json['carbs'] as num).toDouble(),
      fat: (json['fat'] as num).toDouble(),
      date: DateTime.parse(json['date'] as String),
      notes: json['notes'] as String?,
    );
  }

  void _notifyListeners() {
    _mealsController.add(List.unmodifiable(_meals));
  }

  void _log(String message, [dynamic data]) {
    if (kDebugMode) {
      final timestamp = DateTime.now().toIso8601String();
      debugPrint('[$timestamp] [MealRepository] $message');
      if (data != null) {
        debugPrint('  Data: $data');
      }
    }
  }

  void _logError(String message, dynamic error) {
    if (kDebugMode) {
      final timestamp = DateTime.now().toIso8601String();
      debugPrint('[$timestamp] [MealRepository] [ERROR] $message');
      debugPrint('  Error: $error');
      if (error is Error) {
        debugPrint('  Stack trace: ${error.stackTrace}');
      }
    }
  }

  Future<int> getPendingOperationsCount() async {
    return await _dbHelper.getPendingOperationsCount();
  }

  Future<void> syncPendingOperations() async {
    await _checkServerAndSync();
  }
  void dispose() {
    _syncTimer?.cancel();
    _webSocketSubscription?.cancel();
    _serverApi.dispose();
    _mealsController.close();
    _serverStatusController.close();
  }
}
