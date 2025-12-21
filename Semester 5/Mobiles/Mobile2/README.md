CalorieTrack – Mobile App Project Idea
1. Short Description

CalorieTrack is a simple and intuitive mobile application designed to help users monitor their daily food intake and physical activity. It allows users to record meals, view nutritional information, and track progress toward personalized calorie goals. The app is built with non-technical users in mind — focusing on a clean interface and automated insights to support healthier habits. Whether you want to lose weight, maintain your lifestyle, or gain muscle, CalorieTrack provides daily motivation and detailed analytics.


2. Domain Details

Entity: Meal

mealId - Integer - Unique identifier for each meal entry.
mealName - String - The name of the meal (e.g., “Breakfast,” “Lunch,” “Snack”).
calories - Float - Total calories consumed in that meal.
protein - Float - -Amount of protein in grams.
carbs - Float - Amount of carbohydrates in grams.
fat - Float - Amount of fats in grams.
date - Date - The date and time when the meal was logged.
notes - String - Optional notes about the meal (e.g., “homemade sandwich”).

Each field helps users understand and analyze their nutritional intake throughout the day.


3. CRUD Operations
   
Operation	Description
Create	Add a new meal entry by specifying the meal name, calorie amount, and nutrients.
Read	View a list of meals for the current day or a summary of weekly calorie intake.
Update	Edit a meal’s information (e.g., change calorie count or meal name).
Delete	Remove a meal entry permanently from the log.


5. Persistence Details

Local Database:

Stores all user meal entries locally.

Supports Create, Read, and Update operations even when offline.

Server:

Syncs all meals to a remote server for data backup and multi-device access.

Supports Create, Update, and Delete operations.

Ensures data synchronization once the device reconnects to the internet.


6. Offline Scenario

When the device is offline:

Create: Users can still add new meals, which are stored locally and later synchronized.

Read: Existing local entries remain accessible in the app.

Update: Changes to meals are saved locally and synced automatically once online.

Delete: Deleted meals are marked as “pending deletion” and removed from the server when the connection is restored.

Each CRUD operation has its own separate offline handling, ensuring full usability without internet.
