<?php
// Start the session to access user session data
// session_start();
require_once '../config.php';

// $user_id = $_SESSION['user_id'];
$user_id = 1; // For development, use a fixed user_id

// Set the response content type to JSON
header('Content-Type: application/json');

// Get the HTTP request method (GET, POST, etc.)
$method = $_SERVER['REQUEST_METHOD'];

try {
    switch ($method) {
        case 'GET':
            // Get all categories for the current user, including a count of bookmarks in each
            $stmt = $pdo->prepare("
                SELECT c.*, COUNT(b.id) as bookmark_count
                FROM categories c
                LEFT JOIN bookmarks b ON c.id = b.category_id AND b.user_id = ?
                GROUP BY c.id
                ORDER BY c.name ASC
            ");
            $stmt->execute([$user_id]);
            $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);
            
            // Return the categories as JSON
            echo json_encode($categories);
            break;
            
        case 'POST':
            // Create a new category from POSTed JSON data
            $data = json_decode(file_get_contents('php://input'), true);
            
            // Validate required field
            if (!isset($data['name'])) {
                http_response_code(400);
                echo json_encode(['success' => false, 'message' => 'Category name is required']);
                exit;
            }
            
            // Check if category name already exists
            $checkStmt = $pdo->prepare("SELECT id FROM categories WHERE name = ?");
            $checkStmt->execute([$data['name']]);
            
            if ($checkStmt->fetch()) {
                http_response_code(400);
                echo json_encode(['success' => false, 'message' => 'Category name already exists']);
                exit;
            }
            
            // Insert the new category
            $stmt = $pdo->prepare("
                INSERT INTO categories (name, description, created_at)
                VALUES (?, ?, NOW())
            ");
            
            $stmt->execute([
                $data['name'],
                $data['description'] ?: null
            ]);
            
            // Return success and new category ID
            echo json_encode([
                'success' => true,
                'message' => 'Category created successfully',
                'id' => $pdo->lastInsertId()
            ]);
            break;
            
        default:
            // Handle unsupported HTTP methods
            http_response_code(405);
            echo json_encode(['success' => false, 'message' => 'Method not allowed']);
            break;
    }
} catch (PDOException $e) {
    // Handle database errors
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
} 