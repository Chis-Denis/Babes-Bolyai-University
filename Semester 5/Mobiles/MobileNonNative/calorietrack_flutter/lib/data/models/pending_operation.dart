/// Model for pending operations that need to be synced with the server
class PendingOperation {
  final int? id; // Database ID (auto-increment)
  final String operationType; // 'create', 'update', 'delete'
  final int? mealId; // ID of the meal (null for create, required for update/delete)
  final Map<String, dynamic>? mealData; // Meal data for create/update operations
  final DateTime timestamp; // When the operation was created

  PendingOperation({
    this.id,
    required this.operationType,
    this.mealId,
    this.mealData,
    required this.timestamp,
  });

  /// Convert to Map for database storage
  /// Note: meal_data will be stored as JSON string by database helper
  Map<String, dynamic> toMap() {
    return {
      if (id != null) 'id': id,
      'operation_type': operationType,
      'meal_id': mealId,
      'meal_data': mealData, // Will be converted to JSON string in database helper
      'timestamp': timestamp.millisecondsSinceEpoch,
    };
  }

  @override
  String toString() {
    return 'PendingOperation(id: $id, type: $operationType, mealId: $mealId, timestamp: $timestamp)';
  }
}
