<?php
$host = 'localhost';
$username = 'root';
$password = '';

try {
    $pdo = new PDO("mysql:host=$host", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
    $pdo->exec("DROP DATABASE IF EXISTS grade_system");
    
    $sql = file_get_contents('sql/setup.sql');
    $pdo->exec($sql);
    
    echo "<div style='font-family: Arial, sans-serif; max-width: 600px; margin: 20px auto; padding: 20px; background: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0,0,0,0.1);'>";
    echo "<h1>Database Initialized</h1>";
    echo "<p>The database has been successfully initialized with sample data.</p>";
    echo "<p><strong>Sample accounts:</strong></p>";
    echo "<ul>";
    echo "<li>Professor: username '<code>prof1</code>', password '<code>password123</code>'</li>";
    echo "<li>Student: username '<code>student1</code>', password '<code>password123</code>'</li>";
    echo "</ul>";
    echo "<p><a href='login.php' style='display: inline-block; padding: 10px 15px; background: #0066cc; color: #fff; text-decoration: none; border-radius: 3px;'>Go to Login Page</a></p>";
    echo "</div>";
    
} catch(PDOException $e) {
    echo "<div style='font-family: Arial, sans-serif; max-width: 600px; margin: 20px auto; padding: 20px; background: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0,0,0,0.1);'>";
    echo "<h1>Database Error</h1>";
    echo "<p>Error: " . $e->getMessage() . "</p>";
    echo "<p><a href='index.php' style='display: inline-block; padding: 10px 15px; background: #0066cc; color: #fff; text-decoration: none; border-radius: 3px;'>Back to Home</a></p>";
    echo "</div>";
}
?> 