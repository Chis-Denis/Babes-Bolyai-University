<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");
// Add detailed comments throughout the file to explain the logic and flow
// session_start();
require_once '../config.php';

header('Content-Type: application/json');

// if (!isset($_SESSION['user_id'])) {
//     http_response_code(401);
//     echo json_encode(['success' => false, 'message' => 'Unauthorized']);
//     exit();
// }

// $user_id = $_SESSION['user_id'];
$user_id = 1; // For development, use a fixed user_id

switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
        if (isset($_GET['id'])) {
            // Get single task
            $stmt = $pdo->prepare("SELECT * FROM tasks WHERE id = ? AND user_id = ?");
            $stmt->execute([$_GET['id'], $user_id]);
            $task = $stmt->fetch(PDO::FETCH_ASSOC);
            
            if ($task) {
                echo json_encode($task);
            } else {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Task not found']);
            }
        } else {
            // Get all tasks
            $stmt = $pdo->prepare("SELECT * FROM tasks WHERE user_id = ? ORDER BY created_at DESC");
            $stmt->execute([$user_id]);
            echo json_encode($stmt->fetchAll(PDO::FETCH_ASSOC));
        }
        break;

    case 'POST':
        $data = json_decode(file_get_contents('php://input'), true);
        
        if (!isset($data['title'])) {
            http_response_code(400);
            echo json_encode(['success' => false, 'message' => 'Title is required']);
            exit();
        }

        $stmt = $pdo->prepare("INSERT INTO tasks (user_id, title, description, status, due_date) VALUES (?, ?, ?, ?, ?)");
        try {
            $stmt->execute([
                $user_id,
                $data['title'],
                $data['description'] ?? '',
                $data['status'] ?? 'pending',
                $data['due_date'] ?? null
            ]);
            echo json_encode(['success' => true, 'message' => 'Task created successfully']);
        } catch (PDOException $e) {
            http_response_code(500);
            echo json_encode(['success' => false, 'message' => 'Database error']);
        }
        break;

    case 'PUT':
        $data = json_decode(file_get_contents('php://input'), true);
        
        if (!isset($data['id']) || !isset($data['title'])) {
            http_response_code(400);
            echo json_encode(['success' => false, 'message' => 'ID and title are required']);
            exit();
        }

        $stmt = $pdo->prepare("UPDATE tasks SET title = ?, description = ?, status = ?, due_date = ? WHERE id = ? AND user_id = ?");
        try {
            $stmt->execute([
                $data['title'],
                $data['description'] ?? '',
                $data['status'] ?? 'pending',
                $data['due_date'] ?? null,
                $data['id'],
                $user_id
            ]);
            echo json_encode(['success' => true, 'message' => 'Task updated successfully']);
        } catch (PDOException $e) {
            http_response_code(500);
            echo json_encode(['success' => false, 'message' => 'Database error']);
        }
        break;

    case 'DELETE':
        if (!isset($_GET['id'])) {
            http_response_code(400);
            echo json_encode(['success' => false, 'message' => 'Task ID is required']);
            exit();
        }

        $stmt = $pdo->prepare("DELETE FROM tasks WHERE id = ? AND user_id = ?");
        try {
            $stmt->execute([$_GET['id'], $user_id]);
            if ($stmt->rowCount() > 0) {
                echo json_encode(['success' => true, 'message' => 'Task deleted successfully']);
            } else {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Task not found']);
            }
        } catch (PDOException $e) {
            http_response_code(500);
            echo json_encode(['success' => false, 'message' => 'Database error']);
        }
        break;

    default:
        http_response_code(405);
        echo json_encode(['success' => false, 'message' => 'Method not allowed']);
        break;
} 