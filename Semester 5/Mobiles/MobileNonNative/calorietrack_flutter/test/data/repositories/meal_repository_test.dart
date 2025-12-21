import 'package:flutter_test/flutter_test.dart';
import 'package:calorietrack_flutter/data/repositories/meal_repository.dart';
import 'package:calorietrack_flutter/data/models/meal.dart';
import 'package:calorietrack_flutter/data/local/database_helper.dart';
import 'package:sqflite_common_ffi/sqflite_ffi.dart';

void main() {
  // Initialize FFI for testing
  setUpAll(() {
    sqfliteFfiInit();
    databaseFactory = databaseFactoryFfi;
  });

  group('MealRepository CRUD Tests', () {
    late MealRepository repository;
    late DatabaseHelper dbHelper;

    setUp(() async {
      repository = MealRepository();
      dbHelper = DatabaseHelper();
      // Clear database directly to avoid stream issues
      await dbHelper.deleteAllMeals();
      await dbHelper.deleteAllPendingOperations();
    });

    tearDown(() async {
      // Clear data after each test
      await dbHelper.deleteAllMeals();
      await dbHelper.deleteAllPendingOperations();
    });

    test('CREATE: should add a new meal', () async {
      final meal = await repository.addMeal(
        mealName: 'Test Meal',
        calories: 500.0,
        protein: 30.0,
        carbs: 50.0,
        fat: 20.0,
        date: DateTime(2024, 1, 15),
        notes: 'Test notes',
      );

      expect(meal.mealName, 'Test Meal');
      expect(meal.calories, 500.0);
      expect(meal.protein, 30.0);
      expect(meal.carbs, 50.0);
      expect(meal.fat, 20.0);
      expect(meal.notes, 'Test notes');
    });

    test('READ: should retrieve all meals', () async {
      // Create multiple meals
      await repository.addMeal(
        mealName: 'Meal 1',
        calories: 300.0,
        protein: 20.0,
        carbs: 40.0,
        fat: 10.0,
        date: DateTime(2024, 1, 15),
      );

      await repository.addMeal(
        mealName: 'Meal 2',
        calories: 400.0,
        protein: 25.0,
        carbs: 45.0,
        fat: 15.0,
        date: DateTime(2024, 1, 16),
      );

      await repository.loadMealsFromLocal();
      final meals = repository.meals;
      
      expect(meals.length, greaterThanOrEqualTo(2));
      expect(meals.any((m) => m.mealName == 'Meal 1'), true);
      expect(meals.any((m) => m.mealName == 'Meal 2'), true);
    });

    test('READ: should find meal by id', () async {
      final createdMeal = await repository.addMeal(
        mealName: 'Test Meal',
        calories: 500.0,
        protein: 30.0,
        carbs: 50.0,
        fat: 20.0,
        date: DateTime(2024, 1, 15),
      );

      final foundMeal = repository.findById(createdMeal.mealId);
      
      expect(foundMeal, isNotNull);
      expect(foundMeal!.mealId, createdMeal.mealId);
      expect(foundMeal.mealName, 'Test Meal');
      expect(foundMeal.calories, 500.0);
    });

    test('UPDATE: should update an existing meal', () async {
      // Create a meal
      final originalMeal = await repository.addMeal(
        mealName: 'Original Meal',
        calories: 500.0,
        protein: 30.0,
        carbs: 50.0,
        fat: 20.0,
        date: DateTime(2024, 1, 15),
        notes: 'Original notes',
      );

      // Update the meal
      final updatedMeal = originalMeal.copyWith(
        mealName: 'Updated Meal',
        calories: 600.0,
        notes: 'Updated notes',
      );

      await repository.updateMeal(updatedMeal);

      // Verify the update
      final foundMeal = repository.findById(originalMeal.mealId);
      expect(foundMeal, isNotNull);
      expect(foundMeal!.mealName, 'Updated Meal');
      expect(foundMeal.calories, 600.0);
      expect(foundMeal.notes, 'Updated notes');
    });

    test('DELETE: should remove a meal by id', () async {
      // Create a meal
      final meal = await repository.addMeal(
        mealName: 'Meal to delete',
        calories: 300.0,
        protein: 20.0,
        carbs: 40.0,
        fat: 10.0,
        date: DateTime(2024, 1, 15),
      );

      final mealId = meal.mealId;
      
      // Verify meal exists
      expect(repository.findById(mealId), isNotNull);

      // Delete the meal
      await repository.removeMealById(mealId);

      // Verify meal is deleted
      expect(repository.findById(mealId), isNull);
    });

    test('Full CRUD flow: Create, Read, Update, Delete', () async {
      // CREATE
      final meal = await repository.addMeal(
        mealName: 'CRUD Test Meal',
        calories: 500.0,
        protein: 30.0,
        carbs: 50.0,
        fat: 20.0,
        date: DateTime(2024, 1, 15),
        notes: 'Initial notes',
      );

      expect(meal.mealName, 'CRUD Test Meal');

      // READ
      final foundMeal = repository.findById(meal.mealId);
      expect(foundMeal, isNotNull);
      expect(foundMeal!.mealName, 'CRUD Test Meal');

      // UPDATE
      final updatedMeal = meal.copyWith(
        mealName: 'Updated CRUD Meal',
        calories: 600.0,
        notes: 'Updated notes',
      );
      await repository.updateMeal(updatedMeal);

      final foundUpdatedMeal = repository.findById(meal.mealId);
      expect(foundUpdatedMeal, isNotNull);
      expect(foundUpdatedMeal!.mealName, 'Updated CRUD Meal');
      expect(foundUpdatedMeal.calories, 600.0);

      // DELETE
      await repository.removeMealById(meal.mealId);
      expect(repository.findById(meal.mealId), isNull);
    });
  });
}
