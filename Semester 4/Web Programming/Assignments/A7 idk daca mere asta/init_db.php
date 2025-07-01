<?php
// Include the database configuration and connection
require_once 'config.php';

try {
    // Create database if it doesn't exist
    $pdo->exec("CREATE DATABASE IF NOT EXISTS url_bookmarks");
    $pdo->exec("USE url_bookmarks");

    // Create users table if it doesn't exist
    $pdo->exec("CREATE TABLE IF NOT EXISTS users (
        id INT AUTO_INCREMENT PRIMARY KEY,
        username VARCHAR(50) NOT NULL UNIQUE,
        password VARCHAR(255) NOT NULL,
        email VARCHAR(100) NOT NULL UNIQUE,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )");

    // Create categories table if it doesn't exist
    $pdo->exec("CREATE TABLE IF NOT EXISTS categories (
        id INT AUTO_INCREMENT PRIMARY KEY,
        name VARCHAR(50) NOT NULL UNIQUE,
        description TEXT,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )");

    // Create bookmarks table if it doesn't exist
    $pdo->exec("CREATE TABLE IF NOT EXISTS bookmarks (
        id INT AUTO_INCREMENT PRIMARY KEY,
        user_id INT NOT NULL,
        category_id INT NOT NULL,
        url VARCHAR(2048) NOT NULL,
        title VARCHAR(255) NOT NULL,
        description TEXT,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
        FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE CASCADE
    )");

    // Insert a sample user (if not already present)
    $stmt = $pdo->prepare("INSERT IGNORE INTO users (username, password, email) VALUES (?, ?, ?)");
    $password = password_hash('test123', PASSWORD_DEFAULT); // Hash the password
    $stmt->execute(['testuser', $password, 'test@example.com']);

    // Insert sample categories (if not already present)
    $categories = [
        ['Web Development', 'Resources for web development'],
        ['Design', 'Design inspiration and tools'],
        ['Programming', 'Programming tutorials and resources'],
        ['Productivity', 'Tools and tips for productivity']
    ];

    $stmt = $pdo->prepare("INSERT IGNORE INTO categories (name, description) VALUES (?, ?)");
    foreach ($categories as $category) {
        $stmt->execute($category);
    }

    // Get user ID and category IDs for sample bookmarks
    $userId = $pdo->query("SELECT id FROM users WHERE username = 'testuser'")->fetchColumn();
    $categoryIds = $pdo->query("SELECT id FROM categories")->fetchAll(PDO::FETCH_COLUMN);

    // Insert sample bookmarks (if not already present)
    $bookmarks = [
        [
            'url' => 'https://developer.mozilla.org',
            'title' => 'MDN Web Docs',
            'description' => 'The best resource for web development documentation',
            'category_id' => $categoryIds[0]
        ],
        [
            'url' => 'https://dribbble.com',
            'title' => 'Dribbble',
            'description' => 'Discover and connect with designers worldwide',
            'category_id' => $categoryIds[1]
        ],
        [
            'url' => 'https://github.com',
            'title' => 'GitHub',
            'description' => 'The world\'s largest software development platform',
            'category_id' => $categoryIds[2]
        ],
        [
            'url' => 'https://trello.com',
            'title' => 'Trello',
            'description' => 'Organize and prioritize your projects',
            'category_id' => $categoryIds[3]
        ],
        [
            'url' => 'https://stackoverflow.com',
            'title' => 'Stack Overflow',
            'description' => 'The largest online community for programmers',
            'category_id' => $categoryIds[2]
        ]
    ];

    $stmt = $pdo->prepare("INSERT IGNORE INTO bookmarks (user_id, category_id, url, title, description) VALUES (?, ?, ?, ?, ?)");
    foreach ($bookmarks as $bookmark) {
        $stmt->execute([$userId, $bookmark['category_id'], $bookmark['url'], $bookmark['title'], $bookmark['description']]);
    }

    // If everything went well, output a success message
    echo "Database and tables initialized successfully!";
} catch (PDOException $e) {
    // Output error message if something goes wrong
    echo "Database initialization failed: " . $e->getMessage();
} 