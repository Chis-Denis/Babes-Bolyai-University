package com.example.calorietrack

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import java.util.concurrent.atomic.AtomicInteger

object MealRepository {
	private val idGenerator = AtomicInteger(0)
	private val mealsInternal = mutableListOf<Meal>()
	private val mealsLiveData = MutableLiveData<List<Meal>>(emptyList())

	fun getMeals(): LiveData<List<Meal>> = mealsLiveData

	fun addMeal(
		mealName: String,
		calories: Float,
		protein: Float,
		carbs: Float,
		fat: Float,
		dateMillis: Long,
		notes: String?
	): Meal {
		val meal = Meal(
			mealId = idGenerator.incrementAndGet(),
			mealName = mealName,
			calories = calories,
			protein = protein,
			carbs = carbs,
			fat = fat,
			dateMillis = dateMillis,
			notes = notes
		)
		mealsInternal.add(meal)
		emit()
		return meal
	}

	fun updateMeal(updated: Meal) {
		val index = mealsInternal.indexOfFirst { it.mealId == updated.mealId }
		if (index != -1) {
			mealsInternal[index] = updated
			emit()
		}
	}

	fun removeMealById(id: Int) {
		val index = mealsInternal.indexOfFirst { it.mealId == id }
		if (index != -1) {
			mealsInternal.removeAt(index)
			emit()
		}
	}

	fun findById(id: Int): Meal? = mealsInternal.firstOrNull { it.mealId == id }?.copy()

	private fun emit() {
		mealsLiveData.postValue(mealsInternal.map { it.copy() })
	}
}


