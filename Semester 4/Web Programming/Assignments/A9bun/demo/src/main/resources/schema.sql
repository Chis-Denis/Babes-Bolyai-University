CREATE DATABASE IF NOT EXISTS snake_game;
USE snake_game;

CREATE TABLE IF NOT EXISTS users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS game_sessions (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    start_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    end_time TIMESTAMP NULL,
    score INT DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS moves (
    id INT PRIMARY KEY AUTO_INCREMENT,
    game_session_id INT NOT NULL,
    move_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    x_coordinate INT NOT NULL,
    y_coordinate INT NOT NULL,
    direction VARCHAR(10) NOT NULL,
    FOREIGN KEY (game_session_id) REFERENCES game_sessions(id)
); 