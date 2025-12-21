package com.example.calorietrack

import androidx.lifecycle.LiveData
import androidx.lifecycle.ViewModel

class MealsViewModel : ViewModel() {
	val meals: LiveData<List<Meal>> = MealRepository.getMeals()
}


