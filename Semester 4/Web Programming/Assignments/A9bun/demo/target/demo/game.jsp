<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
<head>
    <title>Snake Game</title>
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            background-color: #f0f0f0;
            font-family: Arial, sans-serif;
        }
        #game-container {
            margin-top: 20px;
        }
        canvas {
            border: 2px solid #333;
            background-color: #fff;
        }
        #score {
            font-size: 24px;
            margin: 10px 0;
        }
        #game-over {
            display: none;
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background-color: rgba(0, 0, 0, 0.8);
            color: white;
            padding: 20px;
            border-radius: 5px;
            text-align: center;
        }
        button {
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin-top: 10px;
        }
        button:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <h1>Snake Game</h1>
    <div id="score">Score: 0</div>
    <div id="game-container">
        <canvas id="gameCanvas" width="400" height="400"></canvas>
    </div>
    <div id="game-over">
        <h2>Game Over!</h2>
        <p>Your score: <span id="final-score">0</span></p>
        <button onclick="restartGame()">Play Again</button>
    </div>

    <script>
        const canvas = document.getElementById('gameCanvas');
        const ctx = canvas.getContext('2d');
        const gridSize = 20;
        const tileCount = canvas.width / gridSize;
        
        let snake = [];
        let food = {};
        let obstacles = [];
        let direction = 'right';
        let score = 0;
        
        let gameSessionId;

        function initGame() {
            console.log("initGame called");
            snake = [
                {x: 10, y: 10}
            ];
            generateFood();
            generateObstacles();
            direction = 'right';
            score = 0;
            updateScore();
            console.log("Game initialized. Snake:", snake, "Food:", food);
        }

        function generateFood() {
            food = {
                x: Math.floor(Math.random() * tileCount),
                y: Math.floor(Math.random() * tileCount)
            };
            // Make sure food doesn't spawn on snake or obstacles
            while (isCollision(food.x, food.y)) {
                food = {
                    x: Math.floor(Math.random() * tileCount),
                    y: Math.floor(Math.random() * tileCount)
                };
            }
        }

        function generateObstacles() {
            obstacles = [];
            const obstacleCount = 5;
            for (let i = 0; i < obstacleCount; i++) {
                let obstacle;
                do {
                    obstacle = {
                        x: Math.floor(Math.random() * tileCount),
                        y: Math.floor(Math.random() * tileCount)
                    };
                } while (isCollision(obstacle.x, obstacle.y));
                obstacles.push(obstacle);
            }
        }

        function isCollision(x, y) {
            // Check collision with snake
            for (let segment of snake) {
                if (segment.x === x && segment.y === y) {
                    return true;
                }
            }
            // Check collision with obstacles
            for (let obstacle of obstacles) {
                if (obstacle.x === x && obstacle.y === y) {
                    return true;
                }
            }
            return false;
        }

        function updateScore() {
            document.getElementById('score').textContent = `Score: ${score}`;
        }

        function draw() {
            // Clear canvas
            ctx.fillStyle = 'white';
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            // Draw snake
            ctx.fillStyle = 'green';
            for (let segment of snake) {
                ctx.fillRect(segment.x * gridSize, segment.y * gridSize, gridSize - 2, gridSize - 2);
            }

            // Draw food
            ctx.fillStyle = 'red';
            ctx.fillRect(food.x * gridSize, food.y * gridSize, gridSize - 2, gridSize - 2);

            // Draw obstacles
            ctx.fillStyle = 'gray';
            for (let obstacle of obstacles) {
                ctx.fillRect(obstacle.x * gridSize, obstacle.y * gridSize, gridSize - 2, gridSize - 2);
            }
        }

        function move() {
            const head = {x: snake[0].x, y: snake[0].y};

            switch(direction) {
                case 'up': head.y--; break;
                case 'down': head.y++; break;
                case 'left': head.x--; break;
                case 'right': head.x++; break;
            }

            // Check for collisions
            if (head.x < 0 || head.x >= tileCount || head.y < 0 || head.y >= tileCount || isCollision(head.x, head.y)) {
                gameOver();
                return;
            }

            snake.unshift(head);

            // Check if food is eaten
            if (head.x === food.x && head.y === food.y) {
                score += 10;
                updateScore();
                generateFood();
            } else {
                snake.pop();
            }

            // Record move
            fetch('game/move', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `x=${head.x}&y=${head.y}&direction=${direction}`
            });
        }

        let gameLoopInterval;

        function gameLoop() {
            move();
            draw();
        }

        function gameOver() {
            console.log("gameOver called. Clearing interval:", gameLoopInterval);
            clearInterval(gameLoopInterval);
            document.getElementById('final-score').textContent = score;
            document.getElementById('game-over').style.display = 'block';

            // Send final score to server
            fetch('game/end', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `score=${score}`
            });
        }

        function restartGame() {
            console.log("restartGame called");
            document.getElementById('game-over').style.display = 'none';
            initGame();
            gameLoopInterval = setInterval(gameLoop, 150);
            console.log("New game loop interval ID:", gameLoopInterval);
        }

        // Start new game session
        fetch('game/start', {
            method: 'POST'
        })
        .then(response => response.json())
        .then(data => {
            gameSessionId = data.gameSessionId;
            console.log("Game session started with ID:", gameSessionId);
            initGame();
            gameLoopInterval = setInterval(gameLoop, 150);
            console.log("Initial game loop interval ID:", gameLoopInterval);
        });

        // Handle keyboard input
        document.addEventListener('keydown', (event) => {
            switch(event.key) {
                case 'ArrowUp':
                    if (direction !== 'down') direction = 'up';
                    break;
                case 'ArrowDown':
                    if (direction !== 'up') direction = 'down';
                    break;
                case 'ArrowLeft':
                    if (direction !== 'right') direction = 'left';
                    break;
                case 'ArrowRight':
                    if (direction !== 'left') direction = 'right';
                    break;
            }
        });
    </script>
</body>
</html> 