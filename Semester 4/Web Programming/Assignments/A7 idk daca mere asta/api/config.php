<?php
// Database configuration
$host = 'localhost';
$username = 'root';
$password = '';
$database = 'url_bookmarks';

// Create connection
$conn = new mysqli($host, $username, $password, $database);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// JWT Secret key - Change this to a secure random string in production
define('JWT_SECRET', 'your-secret-key-here'); 