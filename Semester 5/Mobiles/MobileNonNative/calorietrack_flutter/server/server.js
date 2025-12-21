const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const cors = require('cors');
const bodyParser = require('body-parser');
const DatabaseHelper = require('./database');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

const dbHelper = new DatabaseHelper();

app.use(cors({
  origin: '*',
  methods: ['GET', 'POST', 'PUT', 'DELETE', 'OPTIONS'],
  allowedHeaders: ['Content-Type', 'Authorization'],
}));
app.use(bodyParser.json());

// Logging utility
const log = {
  info: (message, data = null) => {
    const timestamp = new Date().toISOString();
    const logMessage = `[${timestamp}] [INFO] ${message}`;
    console.log(logMessage);
    if (data) {
      console.log(JSON.stringify(data, null, 2));
    }
  },
  error: (message, error = null) => {
    const timestamp = new Date().toISOString();
    const logMessage = `[${timestamp}] [ERROR] ${message}`;
    console.error(logMessage);
    if (error) {
      console.error(error);
    }
  },
  debug: (message, data = null) => {
    const timestamp = new Date().toISOString();
    const logMessage = `[${timestamp}] [DEBUG] ${message}`;
    console.log(logMessage);
    if (data) {
      console.log(JSON.stringify(data, null, 2));
    }
  }
};

function broadcast(data) {
  const message = JSON.stringify(data);
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  });
  log.debug('Broadcasted message to WebSocket clients', data);
}

wss.on('connection', (ws) => {
  log.info('New WebSocket client connected');
  
  ws.on('close', () => {
    log.info('WebSocket client disconnected');
  });
  
  ws.on('error', (error) => {
    log.error('WebSocket error', error);
  });
});

app.get('/api/meals', (req, res) => {
  log.info('GET /api/meals - Retrieving all meals');
  try {
    const meals = dbHelper.getAllMeals();
    log.debug('Returning meals', { count: meals.length });
    res.json(meals);
  } catch (error) {
    log.error('Error retrieving meals', error);
    res.status(500).json({ 
      error: 'Failed to retrieve meals',
      message: error.message 
    });
  }
});

app.get('/api/meals/:id', (req, res) => {
  const id = parseInt(req.params.id);
  log.info(`GET /api/meals/${id} - Retrieving meal`);
  try {
    const meal = dbHelper.getMealById(id);
    if (!meal) {
      log.debug(`Meal with id ${id} not found`);
      return res.status(404).json({ 
        error: 'Meal not found',
        message: `No meal found with id ${id}` 
      });
    }
    log.debug('Meal found', meal);
    res.json(meal);
  } catch (error) {
    log.error('Error retrieving meal', error);
    res.status(500).json({ 
      error: 'Failed to retrieve meal',
      message: error.message 
    });
  }
});

app.post('/api/meals', (req, res) => {
  log.info('POST /api/meals - Creating new meal');
  try {
    const { mealName, calories, protein, carbs, fat, date, notes } = req.body;
    
    if (!mealName || calories === undefined || protein === undefined || 
        carbs === undefined || fat === undefined || !date) {
      log.error('Validation failed: Missing required fields', req.body);
      return res.status(400).json({ 
        error: 'Validation failed',
        message: 'Missing required fields: mealName, calories, protein, carbs, fat, date' 
      });
    }
    
    const mealData = {
      mealName,
      calories: parseFloat(calories),
      protein: parseFloat(protein),
      carbs: parseFloat(carbs),
      fat: parseFloat(fat),
      date: new Date(date).toISOString(),
      notes: notes || null
    };
    
    const newMeal = dbHelper.insertMeal(mealData);
    log.info('Meal created successfully', newMeal);
    
    broadcast({ type: 'meal_created', meal: newMeal });
    
    res.status(201).json(newMeal);
  } catch (error) {
    log.error('Error creating meal', error);
    res.status(500).json({ 
      error: 'Failed to create meal',
      message: error.message 
    });
  }
});

app.put('/api/meals/:id', (req, res) => {
  const id = parseInt(req.params.id);
  log.info(`PUT /api/meals/${id} - Updating meal`);
  try {
    const { mealName, calories, protein, carbs, fat, date, notes } = req.body;
    
    if (!mealName || calories === undefined || protein === undefined || 
        carbs === undefined || fat === undefined || !date) {
      log.error('Validation failed: Missing required fields', req.body);
      return res.status(400).json({ 
        error: 'Validation failed',
        message: 'Missing required fields: mealName, calories, protein, carbs, fat, date' 
      });
    }
    
    const mealData = {
      mealName,
      calories: parseFloat(calories),
      protein: parseFloat(protein),
      carbs: parseFloat(carbs),
      fat: parseFloat(fat),
      date: new Date(date).toISOString(),
      notes: notes || null
    };
    
    const updatedMeal = dbHelper.updateMeal(id, mealData);
    
    if (!updatedMeal) {
      log.debug(`Meal with id ${id} not found for update`);
      return res.status(404).json({ 
        error: 'Meal not found',
        message: `No meal found with id ${id}` 
      });
    }
    
    log.info('Meal updated successfully', updatedMeal);
    
    broadcast({ type: 'meal_updated', meal: updatedMeal });
    
    res.json(updatedMeal);
  } catch (error) {
    log.error('Error updating meal', error);
    res.status(500).json({ 
      error: 'Failed to update meal',
      message: error.message 
    });
  }
});

app.delete('/api/meals/:id', (req, res) => {
  const id = parseInt(req.params.id);
  log.info(`DELETE /api/meals/${id} - Deleting meal`);
  try {
    const deleted = dbHelper.deleteMeal(id);
    
    if (!deleted) {
      log.debug(`Meal with id ${id} not found for deletion`);
      return res.status(404).json({ 
        error: 'Meal not found',
        message: `No meal found with id ${id}` 
      });
    }
    
    log.info('Meal deleted successfully', { mealId: id });
    
    broadcast({ type: 'meal_deleted', mealId: id });
    
    res.status(200).json({ 
      message: 'Meal deleted successfully',
      mealId: id 
    });
  } catch (error) {
    log.error('Error deleting meal', error);
    log.error('Delete operation failed', {
      mealId: id,
      error: error.message,
      stack: error.stack
    });
    
    res.status(500).json({ 
      error: 'Failed to delete meal',
      message: error.message 
    });
  }
});

app.delete('/api/meals', (req, res) => {
  log.info('DELETE /api/meals - Deleting all meals');
  try {
    const deletedCount = dbHelper.deleteAllMeals();
    log.info(`Deleted ${deletedCount} meals from database`);
    
    broadcast({ type: 'all_meals_deleted' });
    
    res.status(200).json({ 
      message: 'All meals deleted successfully',
      deletedCount: deletedCount
    });
  } catch (error) {
    log.error('Error deleting all meals', error);
    res.status(500).json({ 
      error: 'Failed to delete all meals',
      message: error.message 
    });
  }
});

app.get('/health', (req, res) => {
  try {
    const mealCount = dbHelper.getMealCount();
    res.json({ 
      status: 'ok', 
      timestamp: new Date().toISOString(),
      mealCount: mealCount
    });
  } catch (error) {
    res.status(500).json({ 
      status: 'error',
      message: error.message 
    });
  }
});

process.on('SIGINT', () => {
  log.info('Shutting down server...');
  dbHelper.close();
  server.close(() => {
    log.info('Server closed');
    process.exit(0);
  });
});

const PORT = process.env.PORT || 3000;
const HOST = process.env.HOST || '0.0.0.0';
server.listen(PORT, HOST, () => {
  log.info(`Server started on ${HOST}:${PORT}`);
  log.info(`REST API available at http://${HOST}:${PORT}/api`);
  log.info(`REST API available at http://localhost:${PORT}/api (local access)`);
  log.info(`WebSocket available at ws://${HOST}:${PORT}`);
  log.info(`WebSocket available at ws://localhost:${PORT} (local access)`);
  log.info(`Database initialized: calorie_track.db`);
  log.info(`Server is accessible from multiple devices on the same network`);
});
