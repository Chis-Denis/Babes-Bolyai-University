package com.example.calorietrack

data class Meal(
	val mealId: Int,
	var mealName: String,
	var calories: Float,
	var protein: Float,
	var carbs: Float,
	var fat: Float,
	var dateMillis: Long,
	var notes: String?
)


