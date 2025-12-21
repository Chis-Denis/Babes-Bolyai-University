const Database = require('better-sqlite3');
const path = require('path');
const fs = require('fs');

/// Database helper class for managing SQLite database operations on the server
class DatabaseHelper {
  constructor() {
    // Database configuration
    const dbDir = path.join(__dirname, 'data');
    
    // Create data directory if it doesn't exist
    if (!fs.existsSync(dbDir)) {
      fs.mkdirSync(dbDir, { recursive: true });
    }
    
    const dbPath = path.join(dbDir, 'calorie_track.db');
    this.db = new Database(dbPath);
    this.tableName = 'meals';
    
    // Initialize database schema
    this._initDatabase();
  }

  /// Initialize the database schema
  _initDatabase() {
    this.db.exec(`
      CREATE TABLE IF NOT EXISTS ${this.tableName} (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        meal_name TEXT NOT NULL,
        calories REAL NOT NULL,
        protein REAL NOT NULL,
        carbs REAL NOT NULL,
        fat REAL NOT NULL,
        date TEXT NOT NULL,
        notes TEXT
      )
    `);
  }

  /// Get all meals from the database, ordered by date (newest first)
  getAllMeals() {
    try {
      const stmt = this.db.prepare(`
        SELECT * FROM ${this.tableName} 
        ORDER BY date DESC
      `);
      const rows = stmt.all();
      return rows.map(row => this._rowToMeal(row));
    } catch (error) {
      throw new Error(`Failed to get all meals: ${error.message}`);
    }
  }

  /// Get a meal by its ID
  getMealById(id) {
    try {
      const stmt = this.db.prepare(`
        SELECT * FROM ${this.tableName} 
        WHERE id = ?
      `);
      const row = stmt.get(id);
      return row ? this._rowToMeal(row) : null;
    } catch (error) {
      throw new Error(`Failed to get meal by id: ${error.message}`);
    }
  }

  /// Insert a new meal into the database
  /// Returns the inserted meal with the generated ID
  insertMeal(mealData) {
    try {
      const stmt = this.db.prepare(`
        INSERT INTO ${this.tableName} (meal_name, calories, protein, carbs, fat, date, notes)
        VALUES (?, ?, ?, ?, ?, ?, ?)
      `);
      
      const result = stmt.run(
        mealData.mealName,
        mealData.calories,
        mealData.protein,
        mealData.carbs,
        mealData.fat,
        mealData.date,
        mealData.notes || null
      );
      
      // Get the inserted meal with the generated ID
      return this.getMealById(result.lastInsertRowid);
    } catch (error) {
      throw new Error(`Failed to insert meal: ${error.message}`);
    }
  }

  /// Update an existing meal in the database
  /// The element is reused, ID remains the same
  updateMeal(id, mealData) {
    try {
      const stmt = this.db.prepare(`
        UPDATE ${this.tableName}
        SET meal_name = ?, calories = ?, protein = ?, carbs = ?, fat = ?, date = ?, notes = ?
        WHERE id = ?
      `);
      
      const result = stmt.run(
        mealData.mealName,
        mealData.calories,
        mealData.protein,
        mealData.carbs,
        mealData.fat,
        mealData.date,
        mealData.notes || null,
        id
      );
      
      if (result.changes === 0) {
        return null; // Meal not found
      }
      
      // Return the updated meal
      return this.getMealById(id);
    } catch (error) {
      throw new Error(`Failed to update meal: ${error.message}`);
    }
  }

  /// Delete a meal by its ID
  /// Only the ID is needed
  deleteMeal(id) {
    try {
      const stmt = this.db.prepare(`
        DELETE FROM ${this.tableName}
        WHERE id = ?
      `);
      
      const result = stmt.run(id);
      
      if (result.changes === 0) {
        return false; // Meal not found
      }
      
      return true; // Successfully deleted
    } catch (error) {
      throw new Error(`Failed to delete meal: ${error.message}`);
    }
  }

  /// Get the count of meals in the database
  getMealCount() {
    try {
      const stmt = this.db.prepare(`SELECT COUNT(*) as count FROM ${this.tableName}`);
      const row = stmt.get();
      return row.count;
    } catch (error) {
      throw new Error(`Failed to get meal count: ${error.message}`);
    }
  }

  /// Delete all meals from the database (one-time use for reset)
  deleteAllMeals() {
    try {
      const stmt = this.db.prepare(`DELETE FROM ${this.tableName}`);
      const result = stmt.run();
      return result.changes;
    } catch (error) {
      throw new Error(`Failed to delete all meals: ${error.message}`);
    }
  }

  /// Convert database row to meal object
  _rowToMeal(row) {
    return {
      id: row.id,
      mealName: row.meal_name,
      calories: row.calories,
      protein: row.protein,
      carbs: row.carbs,
      fat: row.fat,
      date: row.date, // Already in ISO format from database
      notes: row.notes
    };
  }

  /// Close the database connection
  close() {
    this.db.close();
  }
}

module.exports = DatabaseHelper;
