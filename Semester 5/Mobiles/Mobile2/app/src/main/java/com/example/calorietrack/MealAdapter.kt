package com.example.calorietrack

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class MealAdapter(
	private var items: List<Meal>,
	private val onEdit: (Meal) -> Unit,
	private val onDelete: (Meal) -> Unit,
	private val onOpen: (Meal) -> Unit
) : RecyclerView.Adapter<MealAdapter.MealVH>() {

	class MealVH(itemView: View) : RecyclerView.ViewHolder(itemView) {
		val title: TextView = itemView.findViewById(R.id.tvTitle)
		val subtitle: TextView = itemView.findViewById(R.id.tvSubtitle)
		val btnEdit: ImageButton = itemView.findViewById(R.id.btnEdit)
		val btnDelete: ImageButton = itemView.findViewById(R.id.btnDelete)
	}

	override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MealVH {
		val v = LayoutInflater.from(parent.context).inflate(R.layout.item_meal, parent, false)
		return MealVH(v)
	}

	override fun getItemCount(): Int = items.size

	override fun onBindViewHolder(holder: MealVH, position: Int) {
		val meal = items[position]
		holder.title.text = "${meal.mealName} • ${meal.calories.toInt()} kcal"
		holder.subtitle.text = "P ${meal.protein.toInt()}g • C ${meal.carbs.toInt()}g • F ${meal.fat.toInt()}g"
		holder.btnEdit.setOnClickListener { onEdit(meal) }
		holder.btnDelete.setOnClickListener { onDelete(meal) }
		holder.itemView.setOnClickListener { onOpen(meal) }
	}

	fun submitList(newItems: List<Meal>) {
		items = newItems
		notifyDataSetChanged()
	}
}


