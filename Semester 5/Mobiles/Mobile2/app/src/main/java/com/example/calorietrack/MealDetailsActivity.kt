package com.example.calorietrack

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity

class MealDetailsActivity : AppCompatActivity() {
	private var mealId: Int = -1

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		enableEdgeToEdge()
		setContentView(R.layout.activity_meal_details)

		mealId = intent.getIntExtra("mealId", -1)
		val meal = MealRepository.findById(mealId)

		findViewById<TextView>(R.id.tvName).text = meal?.mealName ?: ""
		findViewById<TextView>(R.id.tvCalories).text = "${meal?.calories?.toInt() ?: 0} kcal"
		findViewById<TextView>(R.id.tvMacros).text = "Protein ${meal?.protein?.toInt() ?: 0}g  •  Carbs ${meal?.carbs?.toInt() ?: 0}g  •  Fat ${meal?.fat?.toInt() ?: 0}g"
		findViewById<TextView>(R.id.tvNotes).text = meal?.notes ?: ""

		findViewById<Button>(R.id.btnBack).setOnClickListener {
			startActivity(Intent(this, MainActivity::class.java))
			finish()
		}
	}
}


