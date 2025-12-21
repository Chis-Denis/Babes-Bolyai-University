import 'dart:async';
import 'dart:convert';
import 'package:flutter/foundation.dart';
import 'package:http/http.dart' as http;
import 'package:web_socket_channel/web_socket_channel.dart';
import '../data/models/meal.dart';
import '../core/config/server_config.dart';

class ServerApiService {
  static final ServerApiService _instance = ServerApiService._internal();
  factory ServerApiService() => _instance;
  ServerApiService._internal();

  String get baseUrl => ServerConfig.baseUrl;
  String get wsUrl => ServerConfig.wsUrl;
  
  WebSocketChannel? _webSocketChannel;
  StreamSubscription? _webSocketSubscription;
  final _webSocketStreamController = StreamController<Map<String, dynamic>>.broadcast();
  
  Stream<Map<String, dynamic>> get webSocketStream => _webSocketStreamController.stream;
  
  bool _isConnected = false;

  Future<void> connectWebSocket() async {
    if (_isConnected) {
      _log('WebSocket already connected');
      return;
    }
    
    try {
      _log('Connecting to WebSocket: $wsUrl');
      _webSocketChannel = WebSocketChannel.connect(Uri.parse(wsUrl));
      _isConnected = true;
      
      _webSocketSubscription = _webSocketChannel!.stream.listen(
        (message) {
          try {
            final data = json.decode(message as String) as Map<String, dynamic>;
            _log('Received WebSocket message', data);
            _webSocketStreamController.add(data);
          } catch (e) {
            _logError('Error parsing WebSocket message', e);
          }
        },
        onError: (error) {
          _logError('WebSocket error', error);
          _isConnected = false;
        },
        onDone: () {
          _log('WebSocket connection closed');
          _isConnected = false;
        },
      );
      
      _log('WebSocket connected successfully');
    } catch (e) {
      _logError('Failed to connect WebSocket', e);
      _isConnected = false;
      rethrow;
    }
  }

  void disconnectWebSocket() {
    _log('Disconnecting WebSocket');
    _webSocketSubscription?.cancel();
    _webSocketChannel?.sink.close();
    _isConnected = false;
    _webSocketChannel = null;
  }

  Future<List<Meal>> getAllMeals() async {
    try {
      _log('GET /api/meals - Fetching all meals from server');
      final response = await http.get(
        Uri.parse('$baseUrl/api/meals'),
        headers: {'Content-Type': 'application/json'},
      ).timeout(const Duration(seconds: 10));

      if (response.statusCode == 200) {
        final List<dynamic> data = json.decode(response.body);
        final meals = data.map((json) => _mealFromServerJson(json)).toList();
        _log('Successfully fetched ${meals.length} meals from server');
        return meals;
      } else {
        final error = json.decode(response.body);
        throw ServerException(
          'Failed to fetch meals',
          error['message'] ?? 'Unknown error',
        );
      }
    } catch (e) {
      _logError('Error fetching meals from server', e);
      if (e is ServerException) rethrow;
      throw ServerException('Network error', 'Unable to connect to server. Please check your connection.');
    }
  }

  Future<Meal> createMeal({
    required String mealName,
    required double calories,
    required double protein,
    required double carbs,
    required double fat,
    required DateTime date,
    String? notes,
  }) async {
    try {
      _log('POST /api/meals - Creating meal on server', {
        'mealName': mealName,
        'calories': calories,
        'protein': protein,
        'carbs': carbs,
        'fat': fat,
        'date': date.toIso8601String(),
      });

      final response = await http.post(
        Uri.parse('$baseUrl/api/meals'),
        headers: {'Content-Type': 'application/json'},
        body: json.encode({
          'mealName': mealName,
          'calories': calories,
          'protein': protein,
          'carbs': carbs,
          'fat': fat,
          'date': date.toIso8601String(),
          'notes': notes,
        }),
      ).timeout(const Duration(seconds: 10));

      if (response.statusCode == 201) {
        final data = json.decode(response.body);
        final meal = _mealFromServerJson(data);
        _log('Meal created successfully on server', meal);
        return meal;
      } else {
        final error = json.decode(response.body);
        throw ServerException(
          'Failed to create meal',
          error['message'] ?? 'Unknown error',
        );
      }
    } catch (e) {
      _logError('Error creating meal on server', e);
      if (e is ServerException) rethrow;
      throw ServerException('Network error', 'Unable to create meal. Please check your connection.');
    }
  }

  Future<Meal> updateMeal(Meal meal) async {
    try {
      _log('PUT /api/meals/${meal.mealId} - Updating meal on server', meal);

      final response = await http.put(
        Uri.parse('$baseUrl/api/meals/${meal.mealId}'),
        headers: {'Content-Type': 'application/json'},
        body: json.encode({
          'mealName': meal.mealName,
          'calories': meal.calories,
          'protein': meal.protein,
          'carbs': meal.carbs,
          'fat': meal.fat,
          'date': meal.date.toIso8601String(),
          'notes': meal.notes,
        }),
      ).timeout(const Duration(seconds: 10));

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        final updatedMeal = _mealFromServerJson(data);
        _log('Meal updated successfully on server', updatedMeal);
        return updatedMeal;
      } else {
        final error = json.decode(response.body);
        throw ServerException(
          'Failed to update meal',
          error['message'] ?? 'Unknown error',
        );
      }
    } catch (e) {
      _logError('Error updating meal on server', e);
      if (e is ServerException) rethrow;
      throw ServerException('Network error', 'Unable to update meal. Please check your connection.');
    }
  }

  Future<void> deleteMeal(int mealId) async {
    try {
      _log('DELETE /api/meals/$mealId - Deleting meal from server');

      final response = await http.delete(
        Uri.parse('$baseUrl/api/meals/$mealId'),
        headers: {'Content-Type': 'application/json'},
      ).timeout(const Duration(seconds: 10));

      if (response.statusCode == 200) {
        _log('Meal deleted successfully from server', {'mealId': mealId});
      } else {
        final error = json.decode(response.body);
        throw ServerException(
          'Failed to delete meal',
          error['message'] ?? 'Unknown error',
        );
      }
    } catch (e) {
      _logError('Error deleting meal from server', e);
      if (e is ServerException) rethrow;
      throw ServerException('Network error', 'Unable to delete meal. Please check your connection.');
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

  void _log(String message, [dynamic data]) {
    if (kDebugMode) {
      final timestamp = DateTime.now().toIso8601String();
      debugPrint('[$timestamp] [ServerApiService] $message');
      if (data != null) {
        debugPrint('  Data: $data');
      }
    }
  }

  void _logError(String message, dynamic error) {
    if (kDebugMode) {
      final timestamp = DateTime.now().toIso8601String();
      debugPrint('[$timestamp] [ServerApiService] [ERROR] $message');
      debugPrint('  Error: $error');
      if (error is Error) {
        debugPrint('  Stack trace: ${error.stackTrace}');
      }
    }
  }

  void dispose() {
    disconnectWebSocket();
    _webSocketStreamController.close();
  }
}

class ServerException implements Exception {
  final String title;
  final String message;

  ServerException(this.title, this.message);

  @override
  String toString() => '$title: $message';
}
