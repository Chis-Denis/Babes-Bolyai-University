package com.example.calorietrack

import android.content.Intent
import android.os.Bundle
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import com.google.android.material.dialog.MaterialAlertDialogBuilder

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

		val viewModel = ViewModelProvider(this)[MealsViewModel::class.java]
		val recycler = findViewById<androidx.recyclerview.widget.RecyclerView>(R.id.recyclerMeals)
		recycler.layoutManager = LinearLayoutManager(this)
		val adapter = MealAdapter(emptyList(), onEdit = { meal ->
			val intent = Intent(this, UpdateMealActivity::class.java)
			intent.putExtra("mealId", meal.mealId)
			startActivity(intent)
			//finish()
		}, onDelete = { meal ->
			MaterialAlertDialogBuilder(this)
				.setTitle("Delete meal")
				.setMessage("Are you sure you want to delete ${meal.mealName}?")
				.setNegativeButton("Cancel", null)
				.setPositiveButton("Delete") { _, _ ->
					MealRepository.removeMealById(meal.mealId)
				}
				.show()
		}, onOpen = { meal ->
			val intent = Intent(this, MealDetailsActivity::class.java)
			intent.putExtra("mealId", meal.mealId)
			startActivity(intent)
			finish()
		})
		recycler.adapter = adapter

		viewModel.meals.observe(this) { list ->
			adapter.submitList(list)
		}

		findViewById<com.google.android.material.floatingactionbutton.FloatingActionButton>(R.id.fabAdd)
			.setOnClickListener {
				startActivity(Intent(this, CreateMealActivity::class.java))
				finish()
			}
    }
}