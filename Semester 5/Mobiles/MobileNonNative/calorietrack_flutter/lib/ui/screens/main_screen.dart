import 'dart:async';
import 'package:flutter/material.dart';
import '../../data/models/meal.dart';
import '../../data/repositories/meal_repository.dart';
import '../../core/utils/error_handler.dart';
import 'create_meal_screen.dart';
import 'meal_details_screen.dart';
import 'update_meal_screen.dart';
import '../widgets/meal_item.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});

  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  final MealRepository _repository = MealRepository();
  List<Meal> _meals = [];
  bool _isLoading = true;
  bool _isServerOnline = false;
  late StreamSubscription<List<Meal>> _mealsSubscription;
  late StreamSubscription<bool> _serverStatusSubscription;

  @override
  void initState() {
    super.initState();
    _loadInitialMeals();
    _mealsSubscription = _repository.mealsStream.listen((meals) {
      if (mounted) {
        setState(() {
          _meals = meals;
          _isLoading = false;
        });
      }
    });
    _serverStatusSubscription = _repository.serverStatusStream.listen((isOnline) {
      if (mounted) {
        setState(() {
          _isServerOnline = isOnline;
        });
      }
    });
    // Initialize server status
    _isServerOnline = _repository.isServerOnline;
  }

  Future<void> _loadInitialMeals() async {
    // Repository should be initialized in main.dart
    // But if initialization failed, try to initialize again and handle errors in this view
    try {
      if (!_repository.isInitialized) {
        // If not initialized (e.g., initialization failed in main.dart), try again
        await _repository.initialize();
      }
      // Get the meals that were loaded during initialization
      if (mounted) {
        setState(() {
          _meals = _repository.meals;
          _isLoading = false;
        });
      }
    } catch (e, stackTrace) {
      debugPrint('MainScreen._loadInitialMeals() retrieve error: $e');
      debugPrint('Stack trace: $stackTrace');
      if (mounted) {
        setState(() {
          _meals = _repository.meals; // Show whatever we have, even if empty
          _isLoading = false;
        });
        // Present error to user in a friendly manner - no raw messages
        ErrorHandler.showError(context, e);
      }
    }
  }

  @override
  void dispose() {
    _mealsSubscription.cancel();
    _serverStatusSubscription.cancel();
    super.dispose();
  }

  void _navigateToCreateMeal() {
    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => const CreateMealScreen()),
    );
  }

  void _navigateToMealDetails(Meal meal) {
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => MealDetailsScreen(mealId: meal.mealId),
      ),
    );
  }

  void _navigateToUpdateMeal(Meal meal) {
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => UpdateMealScreen(mealId: meal.mealId),
      ),
    );
  }

  void _showDeleteDialog(Meal meal) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Delete meal'),
          content: Text('Are you sure you want to delete ${meal.mealName}?'),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: const Text('Cancel'),
            ),
            TextButton(
              onPressed: () async {
                Navigator.of(context).pop();
                try {
                  await _repository.removeMealById(meal.mealId);
                } catch (e, stackTrace) {
                  debugPrint('MainScreen._showDeleteDialog() error: $e');
                  debugPrint('Stack trace: $stackTrace');
                  if (mounted) {
                    // Present error to user in a friendly manner - no raw messages
                    // Errors are logged and presented to the user
                    ErrorHandler.showError(context, e);
                  }
                }
              },
              child: const Text('Delete'),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('CalorieTrack'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Column(
        children: [
          // Server status indicator banner
          if (!_isServerOnline)
            Container(
              width: double.infinity,
              padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
              color: Colors.orange,
              child: Row(
                children: [
                  const Icon(Icons.warning_amber_rounded, color: Colors.white),
                  const SizedBox(width: 8),
                  const Expanded(
                    child: Text(
                      'Server is down. Showing local data. Changes will sync when server is back online.',
                      style: TextStyle(color: Colors.white, fontSize: 12),
                    ),
                  ),
                ],
              ),
            ),
          // Meals list
          Expanded(
            child: _isLoading
                ? const Center(child: CircularProgressIndicator())
                : _meals.isEmpty
                    ? const Center(
                        child: Text(
                          'No meals yet.\nTap the + button to add one!',
                          textAlign: TextAlign.center,
                          style: TextStyle(fontSize: 16),
                        ),
                      )
                    : ListView.builder(
                        key: const Key('meals_list'),
                        itemCount: _meals.length,
                        itemBuilder: (context, index) {
                          final meal = _meals[index];
                          return MealItem(
                            key: ValueKey(meal.mealId),
                            meal: meal,
                            onTap: () => _navigateToMealDetails(meal),
                            onEdit: () => _navigateToUpdateMeal(meal),
                            onDelete: () => _showDeleteDialog(meal),
                          );
                        },
                      ),
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _navigateToCreateMeal,
        tooltip: 'Add Meal',
        child: const Icon(Icons.add),
      ),
    );
  }
}

