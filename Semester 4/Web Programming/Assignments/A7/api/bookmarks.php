<?php
require_once '../config/database.php';
requireLogin();

header('Content-Type: application/json');

$db = new Database();
$conn = $db->getConnection();
$userId = getCurrentUserId();

// Handle GET requests (list bookmarks)
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    if (!isset($_GET['category_id'])) {
        echo json_encode(['success' => false, 'message' => 'Category ID is required']);
        exit;
    }

    $categoryId = $_GET['category_id'];
    $page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
    $limit = isset($_GET['limit']) ? (int)$_GET['limit'] : 4;
    $offset = ($page - 1) * $limit;

    // Verify category belongs to user
    $stmt = $conn->prepare("SELECT id FROM categories WHERE id = ? AND user_id = ?");
    $stmt->execute([$categoryId, $userId]);
    if (!$stmt->fetch()) {
        echo json_encode(['success' => false, 'message' => 'Category not found']);
        exit;
    }

    // Get total count
    $stmt = $conn->prepare("SELECT COUNT(*) FROM bookmarks WHERE category_id = ? AND user_id = ?");
    $stmt->execute([$categoryId, $userId]);
    $total = $stmt->fetchColumn();
    $pages = ceil($total / $limit);

    // Get bookmarks
    $stmt = $conn->prepare("
        SELECT id, url, description, created_at 
        FROM bookmarks 
        WHERE category_id = ? AND user_id = ? 
        ORDER BY created_at DESC 
        LIMIT ? OFFSET ?
    ");
    $stmt->execute([$categoryId, $userId, $limit, $offset]);
    $bookmarks = $stmt->fetchAll();

    // Generate HTML
    $html = '';
    if (empty($bookmarks)) {
        $html = '<div class="alert alert-info">No bookmarks found in this category</div>';
    } else {
        foreach ($bookmarks as $bookmark) {
            $html .= '
                <div class="card mb-3">
                    <div class="card-body">
                        <h5 class="card-title">
                            <a href="' . htmlspecialchars($bookmark['url']) . '" target="_blank">
                                ' . htmlspecialchars($bookmark['url']) . '
                            </a>
                        </h5>
                        <p class="card-text">' . htmlspecialchars($bookmark['description']) . '</p>
                        <div class="d-flex justify-content-between align-items-center">
                            <small class="text-muted">
                                Added on ' . date('M d, Y', strtotime($bookmark['created_at'])) . '
                            </small>
                            <div>
                                <button class="btn btn-sm btn-primary edit-btn" 
                                        data-id="' . $bookmark['id'] . '"
                                        data-url="' . htmlspecialchars($bookmark['url']) . '"
                                        data-description="' . htmlspecialchars($bookmark['description']) . '"
                                        data-category-id="' . $categoryId . '">
                                    Edit
                                </button>
                                <button class="btn btn-sm btn-danger delete-btn" 
                                        data-id="' . $bookmark['id'] . '">
                                    Delete
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
            ';
        }
    }

    echo json_encode([
        'success' => true,
        'html' => $html,
        'total' => $total,
        'pages' => $pages
    ]);
    exit;
}

// Handle POST requests (create/update/delete)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'] ?? '';
    
    switch ($action) {
        case 'create':
        case 'update':
            if (!isset($_POST['url']) || !isset($_POST['category_id'])) {
                echo json_encode(['success' => false, 'message' => 'Missing required fields']);
                exit;
            }

            $url = $_POST['url'];
            $description = $_POST['description'] ?? '';
            $categoryId = $_POST['category_id'];

            // Verify category belongs to user
            $stmt = $conn->prepare("SELECT id FROM categories WHERE id = ? AND user_id = ?");
            $stmt->execute([$categoryId, $userId]);
            if (!$stmt->fetch()) {
                echo json_encode(['success' => false, 'message' => 'Category not found']);
                exit;
            }

            if ($action === 'create') {
                $stmt = $conn->prepare("
                    INSERT INTO bookmarks (url, description, category_id, user_id) 
                    VALUES (?, ?, ?, ?)
                ");
                $stmt->execute([$url, $description, $categoryId, $userId]);
                $message = 'Bookmark created successfully';
            } else {
                if (!isset($_POST['id'])) {
                    echo json_encode(['success' => false, 'message' => 'Bookmark ID is required for update']);
                    exit;
                }
                $id = $_POST['id'];
                
                // Verify bookmark belongs to user
                $stmt = $conn->prepare("SELECT id FROM bookmarks WHERE id = ? AND user_id = ?");
                $stmt->execute([$id, $userId]);
                if (!$stmt->fetch()) {
                    echo json_encode(['success' => false, 'message' => 'Bookmark not found']);
                    exit;
                }

                $stmt = $conn->prepare("
                    UPDATE bookmarks 
                    SET url = ?, description = ?, category_id = ? 
                    WHERE id = ? AND user_id = ?
                ");
                $stmt->execute([$url, $description, $categoryId, $id, $userId]);
                $message = 'Bookmark updated successfully';
            }
            break;

        case 'delete':
            if (!isset($_POST['id'])) {
                echo json_encode(['success' => false, 'message' => 'Bookmark ID is required']);
                exit;
            }
            $id = $_POST['id'];

            // Verify bookmark belongs to user
            $stmt = $conn->prepare("SELECT id FROM bookmarks WHERE id = ? AND user_id = ?");
            $stmt->execute([$id, $userId]);
            if (!$stmt->fetch()) {
                echo json_encode(['success' => false, 'message' => 'Bookmark not found']);
                exit;
            }

            $stmt = $conn->prepare("DELETE FROM bookmarks WHERE id = ? AND user_id = ?");
            $stmt->execute([$id, $userId]);
            $message = 'Bookmark deleted successfully';
            break;

        default:
            echo json_encode(['success' => false, 'message' => 'Invalid action']);
            exit;
    }

    echo json_encode(['success' => true, 'message' => $message]);
    exit;
}

echo json_encode(['success' => false, 'message' => 'Invalid request method']); 