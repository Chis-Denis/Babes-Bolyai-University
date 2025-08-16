<?php
require_once 'includes/utils.php';
require_once 'includes/layout.php';

// Redirect if already logged in
if (isLoggedIn()) {
    header('Location: index.php');
    exit();
}

$error = '';

// Process login form
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = trim($_POST['username'] ?? '');
    $password = $_POST['password'] ?? '';
    
    if (empty($username) || empty($password)) {
        $error = 'Please enter both username and password.';
    } else {
        $user = getUserByUsername($username);
        
        if ($user && password_verify($password, $user['password'])) {
            $_SESSION['user_id'] = $user['id'];
            $_SESSION['username'] = $user['username'];
            $_SESSION['name'] = $user['name'];
            $_SESSION['role'] = $user['role'];
            
            if ($user['role'] === 'student') {
                $_SESSION['class_group'] = $user['class_group'];
            }
            
            header('Location: index.php');
            exit();
        } else {
            $error = 'Invalid username or password.';
        }
    }
}

renderHeader('Login - Grade Management System');
?>

<h1>Grade Management System</h1>

<div class="login-form">
    <h2>Login</h2>
    
    <?php if (!empty($error)): ?>
        <div class="error"><?= htmlspecialchars($error) ?></div>
    <?php endif; ?>
    
    <form method="post" action="">
        <div class="form-group">
            <label for="username">Username:</label>
            <input type="text" id="username" name="username" required>
        </div>
        
        <div class="form-group">
            <label for="password">Password:</label>
            <input type="password" id="password" name="password" required>
        </div>
        
        <div class="form-group">
            <button type="submit">Login</button>
        </div>
    </form>
    
    
    <p><a href="init_db.php">Initialize/Reset Database</a></p>
</div>

<?php renderFooter(); ?> 