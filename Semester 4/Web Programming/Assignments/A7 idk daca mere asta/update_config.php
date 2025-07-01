<?php
$config_content = '<?php
$host = "localhost";
$username = "root";
$password = "";

try {
    $pdo = new PDO("mysql:host=$host;dbname=url_bookmarks", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch(PDOException $e) {
    echo "Connection failed: " . $e->getMessage();
    die();
}
?>';

file_put_contents('config.php', $config_content);
echo "Config file updated successfully!";
?> 