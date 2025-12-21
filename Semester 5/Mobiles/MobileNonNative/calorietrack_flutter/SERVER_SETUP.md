# Server Setup Guide

This guide explains how to set up and run the server for the CalorieTrack Flutter application.

## Overview

The server provides:
- **REST API** for CRUD operations (Create, Read, Update, Delete)
- **WebSocket** for real-time updates
- **Comprehensive logging** for debugging
- **Error handling** with user-friendly messages

## Requirements

- **Node.js** (version 14 or higher)
- **npm** (comes with Node.js)

## Step-by-Step Setup

### Step 1: Install Node.js

If you don't have Node.js installed:

1. Download Node.js from [https://nodejs.org/](https://nodejs.org/)
2. Install it following the installation wizard
3. Verify installation by opening a terminal and running:
   ```powershell
   node --version
   npm --version
   ```

### Step 2: Install Server Dependencies

1. Navigate to the server directory:
   ```powershell
   cd server
   ```

2. Install dependencies:
   ```powershell
   npm install
   ```

   This will install:
   - `express` - Web framework for REST API
   - `ws` - WebSocket library
   - `cors` - Cross-Origin Resource Sharing support
   - `body-parser` - Parse JSON request bodies
   - `better-sqlite3` - SQLite database for persistent storage
   - `nodemon` (dev) - Auto-restart server during development

   **Note**: `better-sqlite3` requires native compilation. If installation fails:
   - Make sure you have Python and build tools installed
   - On Windows, you may need Visual Studio Build Tools
   - Alternatively, use `sqlite3` package (async, but slower)

### Step 3: Start the Server

#### Development Mode (with auto-restart):
```powershell
npm run dev
```

#### Production Mode:
```powershell
npm start
```

The server will start on **port 3000** by default.

You should see:
```
[timestamp] [INFO] Server started on port 3000
[timestamp] [INFO] REST API available at http://localhost:3000/api
[timestamp] [INFO] WebSocket available at ws://localhost:3000
[timestamp] [INFO] Database initialized: calorie_track.db
```

The database file will be automatically created in `server/data/calorie_track.db` on first run.

### Step 4: Verify Server is Running

Open your browser and visit:
- **Health Check**: http://localhost:3000/health
- **API Endpoint**: http://localhost:3000/api/meals

You should see a JSON response (empty array `[]` if no meals exist).

## Server Architecture

### REST API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/meals` | Get all meals |
| GET | `/api/meals/:id` | Get a specific meal |
| POST | `/api/meals` | Create a new meal |
| PUT | `/api/meals/:id` | Update an existing meal |
| DELETE | `/api/meals/:id` | Delete a meal |

### WebSocket Events

The server broadcasts the following events to all connected clients:

- `meal_created` - When a new meal is created
- `meal_updated` - When a meal is updated
- `meal_deleted` - When a meal is deleted

### Data Storage

The server uses **SQLite database** for persistent storage. The database file is stored in `server/data/calorie_track.db`.

**Features**:
- Data persists across server restarts
- Same schema as Flutter app's local database
- Automatic database initialization on first run
- Database file created in `server/data/` directory

**Note**: The database file (`*.db`) is excluded from git (see `.gitignore`). Each environment will have its own database instance.

For production, you may want to:
- Use a more robust database (PostgreSQL, MySQL, etc.)
- Add database backups
- Implement proper migrations
- Add authentication and authorization

## Flutter App Configuration

### Update Server URL (if needed)

If your server is running on a different machine or port, update the server URL in:

**File**: `lib/services/server_api_service.dart`

```dart
// Change these constants:
static const String baseUrl = 'http://YOUR_SERVER_IP:3000';
static const String wsUrl = 'ws://YOUR_SERVER_IP:3000';
```

### For Android Emulator

If running the Flutter app on an Android emulator, use:
- `http://10.0.2.2:3000` instead of `http://localhost:3000`

### For Physical Device

If running on a physical device:
1. Find your computer's IP address:
   ```powershell
   ipconfig
   ```
2. Use that IP address in the Flutter app configuration
3. Make sure your device and computer are on the same network

## Features Implementation

### ✅ Read Operation
- All values retrieved **only once** on app startup
- Data is **reused** while the application is alive
- A **separate repository** is used (`MealRepository`)
- **WebSocket** listens for server changes in real-time

### ✅ Create Operation
- **Only the created element** is sent to the server
- The **ID is managed by the server**
- The user is **not aware of the internal ID**

### ✅ Update Operation
- Server element is **reused** (not deleted and recreated)
- The **ID remains the same**

### ✅ Delete Operation
- **Only the ID** is sent to the server
- Errors are **logged and presented** to the user

### ✅ Additional Requirements
- All server operations handled in **separate thread/coroutine** (async)
- Error/validation messages presented in a **friendly manner**
- **All client operations** have debug logs
- **All server operations** have debug logs

## Testing the Server

### Using cURL (PowerShell)

**Get all meals:**
```powershell
curl http://localhost:3000/api/meals
```

**Create a meal:**
```powershell
curl -X POST http://localhost:3000/api/meals -H "Content-Type: application/json" -d '{\"mealName\":\"Test Meal\",\"calories\":500,\"protein\":30,\"carbs\":50,\"fat\":20,\"date\":\"2024-01-01T12:00:00Z\"}'
```

**Update a meal (replace :id with actual ID):**
```powershell
curl -X PUT http://localhost:3000/api/meals/1 -H "Content-Type: application/json" -d '{\"mealName\":\"Updated Meal\",\"calories\":600,\"protein\":35,\"carbs\":60,\"fat\":25,\"date\":\"2024-01-01T12:00:00Z\"}'
```

**Delete a meal:**
```powershell
curl -X DELETE http://localhost:3000/api/meals/1
```

## Troubleshooting

### Server won't start

1. **Check if port 3000 is already in use:**
   ```powershell
   netstat -ano | findstr :3000
   ```
   If something is using port 3000, either:
   - Stop that process, or
   - Change the port in `server.js` (set `PORT` environment variable)

2. **Check Node.js installation:**
   ```powershell
   node --version
   ```

3. **Reinstall dependencies:**
   ```powershell
   cd server
   rm -r node_modules
   npm install
   ```

### Flutter app can't connect to server

1. **Check server is running:**
   - Visit http://localhost:3000/health in browser

2. **Check server URL in Flutter app:**
   - Verify `baseUrl` and `wsUrl` in `server_api_service.dart`

3. **Check network connectivity:**
   - For emulator: Use `10.0.2.2:3000`
   - For physical device: Use your computer's IP address

4. **Check firewall:**
   - Windows Firewall might be blocking connections
   - Allow Node.js through firewall if prompted

### WebSocket connection fails

1. **Check WebSocket URL:**
   - Should be `ws://localhost:3000` (not `http://`)

2. **Check server logs:**
   - Look for WebSocket connection messages in server console

3. **Verify CORS settings:**
   - Server has CORS enabled, but check if your setup needs additional configuration

## Logging

### Server Logs

All server operations are logged with timestamps:
- `[INFO]` - General information
- `[DEBUG]` - Debug information
- `[ERROR]` - Error messages

### Client Logs

All client operations are logged in Flutter debug console:
- Repository operations
- API service calls
- WebSocket messages
- Error details

## Next Steps

1. **Run the server** using `npm start` or `npm run dev`
2. **Run the Flutter app** using `flutter run`
3. **Test the integration** by creating, updating, and deleting meals
4. **Check logs** in both server console and Flutter debug console

## Production Considerations

For production deployment:

1. **Use a database** instead of in-memory storage
2. **Add authentication** (JWT tokens, OAuth, etc.)
3. **Add rate limiting** to prevent abuse
4. **Use environment variables** for configuration
5. **Add HTTPS/WSS** for secure connections
6. **Implement proper error handling** and monitoring
7. **Add database migrations** for schema changes
8. **Use a process manager** like PM2 for Node.js
9. **Set up logging** to files or logging service
10. **Add API versioning** for future compatibility
