<?php
require_once 'includes/utils.php';
require_once 'includes/layout.php';

if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

$pageTitle = "Grade Management System - Home";

renderHeader($pageTitle);
?>

<div class="container">
    <h1>Welcome to the Grade Management System</h1>
    
    <?php if (isLoggedIn()): ?>
        <div class="dashboard">
            <h2>Dashboard</h2>
            <p>Hello, <?= htmlspecialchars($_SESSION['name']) ?>!</p>
            
            <div class="dashboard-links">
                <?php if (isProfessor()): ?>
                    <a href="manage_grades.php" class="btn">Manage Grades</a>
                <?php else: ?>
                    <a href="view_grades.php" class="btn">View Your Grades</a>
                <?php endif; ?>
                
            </div>
        </div>
    <?php else: ?>
        <div class="welcome-section">
            <p>Please <a href="login.php">login</a> to access the grade management system.</p>
        </div>
    <?php endif; ?>
</div>

<?php
renderFooter();
?> 