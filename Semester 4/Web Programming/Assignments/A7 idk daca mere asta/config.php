<?php
// Database connection settings
$host = "localhost";     // Database server
$username = "root";      // Database username
$password = "";          // Database password (empty for local XAMPP)

try {
    // Create PDO connection to MySQL database
    $pdo = new PDO("mysql:host=$host;dbname=url_bookmarks", $username, $password);
    
    // Configure PDO to throw exceptions on errors
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch(PDOException $e) {
    // Handle connection errors
    echo "Connection failed: " . $e->getMessage();
    die();
}
?>