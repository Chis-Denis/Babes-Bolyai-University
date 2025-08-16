<?php

function renderHeader($title = 'Grade Management System') {
    ?>
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title><?= htmlspecialchars($title) ?></title>
        <link rel="stylesheet" href="css/simple.css">
    </head>
    <body>
        <div class="container">
            <?php if (isLoggedIn()): ?>
                <div class="nav">
                    <div>
                        <a href="index.php">Home</a>
                    </div>
                    <div class="user-info">
                        <span>Welcome, <?= htmlspecialchars(getCurrentUserName()) ?> (<?= htmlspecialchars(getCurrentUserRole()) ?>)</span>
                        <a href="logout.php">Logout</a>
                    </div>
                </div>
            <?php endif; ?>
    <?php
}

function renderFooter() {
    ?>
        </div>
    </body>
    </html>
    <?php
}
?>