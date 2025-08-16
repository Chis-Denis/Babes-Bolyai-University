<?php
require_once 'includes/utils.php';

$_SESSION = array();

session_destroy();

header('Location: login.php');
exit();
?>