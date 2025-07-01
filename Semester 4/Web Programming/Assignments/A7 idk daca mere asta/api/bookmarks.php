<?php
// Set CORS headers for cross-origin requests
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type, Authorization");
// session_start();
require_once '../config.php';

// if (!isset($_SESSION['user_id'])) {
//     http_response_code(401);
//     echo json_encode(['success' => false, 'message' => 'Unauthorized']);
//     exit;
// }

// Set response type to JSON
header('Content-Type: application/json');

// For development, use fixed user_id (1)
$user_id = 1;

// Get HTTP request method
$method = $_SERVER['REQUEST_METHOD'];

// Handle preflight OPTIONS request
if ($method === 'OPTIONS') {
    http_response_code(200);
    exit();
}

try {
    // Handle different HTTP methods
    switch ($method) {
        case 'GET':
            // Get pagination parameters
            $page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
            $per_page = isset($_GET['per_page']) ? (int)$_GET['per_page'] : 4;
            $category_id = isset($_GET['category_id']) ? (int)$_GET['category_id'] : null;

            // If ID is provided, get single bookmark
            if (isset($_GET['id'])) {
                $stmt = $pdo->prepare("
                    SELECT b.*, c.name as category_name 
                    FROM bookmarks b 
                    LEFT JOIN categories c ON b.category_id = c.id 
                    WHERE b.id = ? AND b.user_id = ?
                ");
                $stmt->execute([$_GET['id'], $user_id]);
                $bookmark = $stmt->fetch(PDO::FETCH_ASSOC);
                if ($bookmark) {
                    echo json_encode($bookmark);
                } else {
                    http_response_code(404);
                    echo json_encode(['success' => false, 'message' => 'Bookmark not found']);
                }
                exit;
            }

            // Build WHERE clause for filtering
            $where = "WHERE b.user_id = ?";
            $params = [$user_id];
            if ($category_id) {
                $where .= " AND b.category_id = ?";
                $params[] = $category_id;
            }

            // Get total count for pagination
            $countStmt = $pdo->prepare("
                SELECT COUNT(*) 
                FROM bookmarks b 
                $where
            ");
            $countStmt->execute($params);
            $total = $countStmt->fetchColumn();

            // Calculate pagination values
            $offset = ($page - 1) * $per_page;
            $limit = (int)$per_page;
            $offset = (int)$offset;

            // Get paginated bookmarks
            $sql = "
                SELECT b.*, c.name as category_name 
                FROM bookmarks b 
                LEFT JOIN categories c ON b.category_id = c.id 
                $where
                ORDER BY b.created_at DESC 
                LIMIT $limit OFFSET $offset
            ";
            $stmt = $pdo->prepare($sql);
            $stmt->execute($params);
            $bookmarks = $stmt->fetchAll(PDO::FETCH_ASSOC);

            // Return paginated results
            echo json_encode([
                'bookmarks' => $bookmarks,
                'total_pages' => ceil($total / $per_page),
                'current_page' => $page
            ]);
            break;

        case 'POST':
            // Get POST data
            $data = json_decode(file_get_contents('php://input'), true);

            // Validate required fields
            if (!isset($data['url']) || !isset($data['title'])) {
                http_response_code(400);
                echo json_encode(['success' => false, 'message' => 'URL and title are required']);
                exit;
            }

            // Insert new bookmark
            $stmt = $pdo->prepare("
                INSERT INTO bookmarks (user_id, category_id, url, title, description, created_at)
                VALUES (?, ?, ?, ?, ?, NOW())
            ");
            $stmt->execute([
                $user_id,
                isset($data['category_id']) ? $data['category_id'] : null,
                $data['url'],
                $data['title'],
                isset($data['description']) ? $data['description'] : null
            ]);
            echo json_encode(['success' => true, 'message' => 'Bookmark created successfully']);
            break;

        case 'PUT':
            // Get PUT data
            $data = json_decode(file_get_contents('php://input'), true);

            // Validate required fields
            if (!isset($data['id']) || !isset($data['url']) || !isset($data['title'])) {
                http_response_code(400);
                echo json_encode(['success' => false, 'message' => 'ID, URL, and title are required']);
                exit;
            }

            // Check if bookmark exists and belongs to user
            $checkStmt = $pdo->prepare("SELECT id FROM bookmarks WHERE id = ? AND user_id = ?");
            $checkStmt->execute([$data['id'], $user_id]);
            if (!$checkStmt->fetch()) {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Bookmark not found']);
                exit;
            }

            // Update bookmark
            $stmt = $pdo->prepare("
                UPDATE bookmarks 
                SET category_id = ?, url = ?, title = ?, description = ?
                WHERE id = ? AND user_id = ?
            ");
            $stmt->execute([
                isset($data['category_id']) ? $data['category_id'] : null,
                $data['url'],
                $data['title'],
                isset($data['description']) ? $data['description'] : null,
                $data['id'],
                $user_id
            ]);
            echo json_encode(['success' => true, 'message' => 'Bookmark updated successfully']);
            break;

        case 'DELETE':
            // Validate bookmark ID
            if (!isset($_GET['id'])) {
                http_response_code(400);
                echo json_encode(['success' => false, 'message' => 'Bookmark ID is required']);
                exit;
            }

            // Delete bookmark
            $stmt = $pdo->prepare("DELETE FROM bookmarks WHERE id = ? AND user_id = ?");
            $stmt->execute([$_GET['id'], $user_id]);
            if ($stmt->rowCount() === 0) {
                http_response_code(404);
                echo json_encode(['success' => false, 'message' => 'Bookmark not found']);
                exit;
            }
            echo json_encode(['success' => true, 'message' => 'Bookmark deleted successfully']);
            break;

        default:
            // Handle unsupported methods
            http_response_code(405);
            echo json_encode(['success' => false, 'message' => 'Method not allowed']);
            break;
    }
} catch (PDOException $e) {
    // Handle database errors
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
} 