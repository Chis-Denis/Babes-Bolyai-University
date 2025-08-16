<?php
session_start();

$host = 'localhost';
$db_name = 'grade_system';
$username = 'root';
$password = '';
$charset = 'utf8mb4';

$dsn = "mysql:host=$host;dbname=$db_name;charset=$charset";
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES => false,
];

try {
    $pdo = new PDO($dsn, $username, $password, $options);
} catch (PDOException $e) {
    die("Database connection failed: " . $e->getMessage());
}


function isLoggedIn() {
    return isset($_SESSION['user_id']);
}

function isProfessor() {
    return isLoggedIn() && $_SESSION['role'] === 'professor';
}

function isStudent() {
    return isLoggedIn() && $_SESSION['role'] === 'student';
}

function requireLogin() {
    if (!isLoggedIn()) {
        header('Location: login.php');
        exit();
    }
}

function requireProfessor() {
    requireLogin();
    if (!isProfessor()) {
        header('Location: index.php');
        exit();
    }
}

function requireStudent() {
    requireLogin();
    if (!isStudent()) {
        header('Location: index.php');
        exit();
    }
}

function getCurrentUserId() {
    return $_SESSION['user_id'] ?? null;
}

function getCurrentUserName() {
    return $_SESSION['name'] ?? null;
}

function getCurrentUserRole() {
    return $_SESSION['role'] ?? null;
}


function getUserByUsername($username) {
    global $pdo;
    $stmt = $pdo->prepare("SELECT * FROM users WHERE username = ?");
    $stmt->execute([$username]);
    return $stmt->fetch();
}


function getAllGroups() {
    global $pdo;
    $stmt = $pdo->prepare("SELECT DISTINCT class_group as name FROM users WHERE class_group IS NOT NULL ORDER BY class_group");
    $stmt->execute();
    return $stmt->fetchAll();
}

function getTotalStudentsInGroup($groupName) {
    global $pdo;
    $stmt = $pdo->prepare("SELECT COUNT(*) as count FROM users WHERE class_group = ? AND role = 'student'");
    $stmt->execute([$groupName]);
    $result = $stmt->fetch();
    return $result['count'];
}

function getStudentsByGroup($groupName, $offset, $limit) {
    global $pdo;
    $stmt = $pdo->prepare("
        SELECT id, name, username
        FROM users
        WHERE class_group = ? AND role = 'student'
        ORDER BY name
        LIMIT ?, ?
    ");
    $stmt->bindParam(1, $groupName, PDO::PARAM_STR);
    $stmt->bindParam(2, $offset, PDO::PARAM_INT);
    $stmt->bindParam(3, $limit, PDO::PARAM_INT);
    $stmt->execute();
    return $stmt->fetchAll();
}


function getCoursesByProfessorId($professorId) {
    global $pdo;
    $stmt = $pdo->prepare("SELECT * FROM courses WHERE professor_id = ? ORDER BY name");
    $stmt->execute([$professorId]);
    return $stmt->fetchAll();
}

// Grade functions
function getStudentGradeForCourse($studentId, $courseId) {
    global $pdo;
    $stmt = $pdo->prepare("SELECT * FROM grades WHERE student_id = ? AND course_id = ?");
    $stmt->execute([$studentId, $courseId]);
    return $stmt->fetch();
}

function getGradesByStudentId($studentId) {
    global $pdo;
    $stmt = $pdo->prepare("
        SELECT g.*, c.name as course_name, u.name as professor_name
        FROM grades g
        JOIN courses c ON g.course_id = c.id
        LEFT JOIN users u ON c.professor_id = u.id
        WHERE g.student_id = ?
        ORDER BY c.name
    ");
    $stmt->execute([$studentId]);
    return $stmt->fetchAll();
}

function upsertGrade($studentId, $courseId, $grade) {
    global $pdo;
    
    $existingGrade = getStudentGradeForCourse($studentId, $courseId);
    
    if ($existingGrade) {
        $stmt = $pdo->prepare("UPDATE grades SET grade = ? WHERE student_id = ? AND course_id = ?");
        return $stmt->execute([$grade, $studentId, $courseId]);
    } else {
        $stmt = $pdo->prepare("INSERT INTO grades (student_id, course_id, grade) VALUES (?, ?, ?)");
        return $stmt->execute([$studentId, $courseId, $grade]);
    }
}

function getGradeClass($grade) {
    if (empty($grade)) {
        return '';
    }
    
    if ($grade >= 8.5) {
        return 'grade-high';
    } elseif ($grade >= 6.5) {
        return 'grade-medium';
    } else {
        return 'grade-low';
    }
}
?> 