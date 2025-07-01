<?php
require_once 'config.php';
$newPassword = password_hash('newpassword123', PASSWORD_DEFAULT);
$conn->query("UPDATE users SET password = '$newPassword' WHERE username = 'testuser'");
echo 'Password updated!'; 