package com.example.calorietrack

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.textfield.TextInputLayout

class CreateMealActivity : AppCompatActivity() {
	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		enableEdgeToEdge()
		setContentView(R.layout.activity_create_meal)

		val name = findViewById<EditText>(R.id.inputMealName)
		val calories = findViewById<EditText>(R.id.inputCalories)
		val protein = findViewById<EditText>(R.id.inputProtein)
		val carbs = findViewById<EditText>(R.id.inputCarbs)
		val fat = findViewById<EditText>(R.id.inputFat)
		val notes = findViewById<EditText>(R.id.inputNotes)

		findViewById<Button>(R.id.btnSave).setOnClickListener {
			if (validate()) {
				MealRepository.addMeal(
					mealName = name.text.toString(),
					calories = calories.text.toString().toFloat(),
					protein = protein.text.toString().toFloat(),
					carbs = carbs.text.toString().toFloat(),
					fat = fat.text.toString().toFloat(),
					dateMillis = System.currentTimeMillis(),
					notes = notes.text.toString().ifBlank { null }
				)
				startActivity(Intent(this, MainActivity::class.java))
				finish()
			}
		}

		findViewById<Button>(R.id.btnCancel).setOnClickListener {
			startActivity(Intent(this, MainActivity::class.java))
			finish()
		}
	}

	private fun validate(): Boolean {
		var ok = true
		ok = ok and required(R.id.tilMealName)
		ok = ok and required(R.id.tilCalories)
		ok = ok and required(R.id.tilProtein)
		ok = ok and required(R.id.tilCarbs)
		ok = ok and required(R.id.tilFat)
		return ok
	}

	private fun required(tilId: Int): Boolean {
		val til = findViewById<TextInputLayout>(tilId)
		val text = til.editText?.text?.toString()?.trim().orEmpty()
		return if (text.isEmpty()) {
			til.error = "Required"
			false
		} else {
			til.error = null
			true
		}
	}
}


