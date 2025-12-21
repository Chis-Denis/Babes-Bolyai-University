import 'dart:convert';
import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';
import '../models/meal.dart';
import '../models/pending_operation.dart';

class DatabaseHelper {
  static final DatabaseHelper _instance = DatabaseHelper._internal();
  factory DatabaseHelper() => _instance;
  DatabaseHelper._internal();

  static Database? _database;

  static const String _databaseName = 'calorie_track.db';
  static const int _databaseVersion = 2;
  static const String _tableName = 'meals';
  static const String _pendingOperationsTableName = 'pending_operations';

  static const String _columnId = 'id';
  static const String _columnMealName = 'meal_name';
  static const String _columnCalories = 'calories';
  static const String _columnProtein = 'protein';
  static const String _columnCarbs = 'carbs';
  static const String _columnFat = 'fat';
  static const String _columnDate = 'date';
  static const String _columnNotes = 'notes';

  /// Get the database instance, creating it if it doesn't exist
  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDatabase();
    return _database!;
  }

  /// Initialize the database
  Future<Database> _initDatabase() async {
    final databasePath = await getDatabasesPath();
    final path = join(databasePath, _databaseName);

    return await openDatabase(
      path,
      version: _databaseVersion,
      onCreate: _onCreate,
      onUpgrade: _onUpgrade,
    );
  }

  /// Create the database schema
  Future<void> _onCreate(Database db, int version) async {
    // Create meals table
    await db.execute('''
      CREATE TABLE $_tableName (
        $_columnId INTEGER PRIMARY KEY AUTOINCREMENT,
        $_columnMealName TEXT NOT NULL,
        $_columnCalories REAL NOT NULL,
        $_columnProtein REAL NOT NULL,
        $_columnCarbs REAL NOT NULL,
        $_columnFat REAL NOT NULL,
        $_columnDate INTEGER NOT NULL,
        $_columnNotes TEXT
      )
    ''');

    // Create pending_operations table for offline sync
    await db.execute('''
      CREATE TABLE $_pendingOperationsTableName (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        operation_type TEXT NOT NULL,
        meal_id INTEGER,
        meal_data TEXT,
        timestamp INTEGER NOT NULL
      )
    ''');
  }

    Future<void> _onUpgrade(Database db, int oldVersion, int newVersion) async {
    // Handle migrations here when schema changes
    if (oldVersion < 2) {
      // Add pending_operations table for version 2
      await db.execute('''
        CREATE TABLE IF NOT EXISTS $_pendingOperationsTableName (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          operation_type TEXT NOT NULL,
          meal_id INTEGER,
          meal_data TEXT,
          timestamp INTEGER NOT NULL
        )
      ''');
    }
  }

  /// Insert a new meal into the database
  Future<int> insertMeal(Meal meal) async {
    final db = await database;
    final map = meal.toMap();
    // Remove id from map for insert (auto-increment)
    map.remove(_columnId);
    return await db.insert(_tableName, map);
  }

  /// Get all meals from the database, ordered by date (newest first)
  Future<List<Meal>> getAllMeals() async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _tableName,
      orderBy: '$_columnDate DESC',
    );
    return List.generate(maps.length, (i) => Meal.fromMap(maps[i]));
  }

  /// Get a meal by its ID
  Future<Meal?> getMealById(int id) async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _tableName,
      where: '$_columnId = ?',
      whereArgs: [id],
    );
    if (maps.isEmpty) return null;
    return Meal.fromMap(maps.first);
  }

  /// Update an existing meal
  Future<int> updateMeal(Meal meal) async {
    final db = await database;
    return await db.update(
      _tableName,
      meal.toMap(),
      where: '$_columnId = ?',
      whereArgs: [meal.mealId],
    );
  }

  /// Delete a meal by its ID
  Future<int> deleteMeal(int id) async {
    final db = await database;
    return await db.delete(
      _tableName,
      where: '$_columnId = ?',
      whereArgs: [id],
    );
  }

  /// Delete all meals (useful for testing or reset)
  Future<int> deleteAllMeals() async {
    final db = await database;
    return await db.delete(_tableName);
  }

  /// Get the count of meals in the database
  Future<int> getMealCount() async {
    final db = await database;
    final List<Map<String, dynamic>> result = await db.rawQuery(
      'SELECT COUNT(*) as count FROM $_tableName',
    );
    return Sqflite.firstIntValue(result) ?? 0;
  }

  // ==================== PENDING OPERATIONS ====================

  /// Add a pending operation to the queue
  Future<int> addPendingOperation(PendingOperation operation) async {
    final db = await database;
    final map = operation.toMap();
    map.remove('id'); // Remove id for insert (auto-increment)
    
    // Convert meal_data Map to JSON string
    if (map['meal_data'] != null && map['meal_data'] is Map) {
      map['meal_data'] = json.encode(map['meal_data']);
    }
    
    return await db.insert(_pendingOperationsTableName, map);
  }

  /// Get all pending operations, ordered by timestamp (oldest first)
  Future<List<PendingOperation>> getAllPendingOperations() async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _pendingOperationsTableName,
      orderBy: 'timestamp ASC',
    );
    
    return maps.map((map) {
      // Parse meal_data from JSON string
      Map<String, dynamic>? mealData;
      if (map['meal_data'] != null) {
        try {
          mealData = json.decode(map['meal_data'] as String) as Map<String, dynamic>;
        } catch (e) {
          mealData = null;
        }
      }
      
      return PendingOperation(
        id: map['id'] as int?,
        operationType: map['operation_type'] as String,
        mealId: map['meal_id'] as int?,
        mealData: mealData,
        timestamp: DateTime.fromMillisecondsSinceEpoch(map['timestamp'] as int),
      );
    }).toList();
  }

  /// Delete a pending operation by ID
  Future<int> deletePendingOperation(int id) async {
    final db = await database;
    return await db.delete(
      _pendingOperationsTableName,
      where: 'id = ?',
      whereArgs: [id],
    );
  }

  /// Delete all pending operations
  Future<int> deleteAllPendingOperations() async {
    final db = await database;
    return await db.delete(_pendingOperationsTableName);
  }

  /// Get count of pending operations
  Future<int> getPendingOperationsCount() async {
    final db = await database;
    final List<Map<String, dynamic>> result = await db.rawQuery(
      'SELECT COUNT(*) as count FROM $_pendingOperationsTableName',
    );
    return Sqflite.firstIntValue(result) ?? 0;
  }

  /// Close the database connection
  Future<void> close() async {
    final db = await database;
    await db.close();
    _database = null;
  }
}

