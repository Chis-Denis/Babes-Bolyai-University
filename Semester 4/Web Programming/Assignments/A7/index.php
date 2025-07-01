<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require_once 'config/database.php';
requireLogin();

$db = new Database();
$conn = $db->getConnection();
$userId = getCurrentUserId();

// Get all categories for the current user
$stmt = $conn->prepare("SELECT id, name FROM categories WHERE user_id = ? ORDER BY name");
$stmt->execute([$UserId]);
$categories = $stmt->fetchAll();
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bookmark Manager</title>
    <link rel="stylesheet" href="css/style.css">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <nav class="navbar navbar-expand-lg navbar-dark bg-primary">
        <div class="container">
            <a class="navbar-brand" href="#">Bookmark Manager</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarNav">
                <ul class="navbar-nav ms-auto">
                    <li class="nav-item">
                        <a class="nav-link" href="logout.php">Logout</a>
                    </li>
                </ul>
            </div>
        </div>
    </nav>

    <div class="container mt-4">
        <div class="row">
            <div class="col-md-12">
                <div class="card">
                    <div class="card-header d-flex justify-content-between align-items-center">
                        <h3>My Bookmarks</h3>
                        <button type="button" class="btn btn-primary" data-bs-toggle="modal" data-bs-target="#bookmarkModal">
                            Add New Bookmark
                        </button>
                    </div>
                    <div class="card-body">
                        <div class="row">
                            <div class="col-md-3">
                                <div class="list-group" id="categoryList">
                                    <?php foreach ($categories as $category): ?>
                                    <a href="#" class="list-group-item list-group-item-action" 
                                       data-category-id="<?php echo $category['id']; ?>">
                                        <?php echo htmlspecialchars($category['name']); ?>
                                    </a>
                                    <?php endforeach; ?>
                                </div>
                            </div>
                            <div class="col-md-9">
                                <div id="bookmarksContainer">
                                    <div class="alert alert-info">Select a category to view bookmarks</div>
                                </div>
                                <nav aria-label="Bookmark pagination" class="mt-3">
                                    <ul class="pagination justify-content-center" id="pagination">
                                    </ul>
                                </nav>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Bookmark Modal -->
    <div class="modal fade" id="bookmarkModal" tabindex="-1">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title">Bookmark Information</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
                </div>
                <form id="bookmarkForm" class="needs-validation" novalidate>
                    <div class="modal-body">
                        <input type="hidden" name="action" value="create">
                        <input type="hidden" name="id">
                        
                        <div class="mb-3">
                            <label for="url" class="form-label">URL</label>
                            <input type="url" class="form-control" id="url" name="url" required>
                            <div class="invalid-feedback">Please enter a valid URL.</div>
                        </div>
                        
                        <div class="mb-3">
                            <label for="description" class="form-label">Description</label>
                            <textarea class="form-control" id="description" name="description" rows="3"></textarea>
                        </div>
                        
                        <div class="mb-3">
                            <label for="category_id" class="form-label">Category</label>
                            <select class="form-select" id="category_id" name="category_id" required>
                                <option value="">Select Category</option>
                                <?php foreach ($categories as $category): ?>
                                <option value="<?php echo $category['id']; ?>">
                                    <?php echo htmlspecialchars($category['name']); ?>
                                </option>
                                <?php endforeach; ?>
                            </select>
                            <div class="invalid-feedback">Please select a category.</div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="submit" class="btn btn-primary">Save</button>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
    <script src="js/main.js"></script>
    <script>
        let currentCategoryId = null;
        let currentPage = 1;
        const itemsPerPage = 4;

        // Handle category selection
        document.querySelectorAll('#categoryList a').forEach(link => {
            link.addEventListener('click', function(e) {
                e.preventDefault();
                currentCategoryId = this.getAttribute('data-category-id');
                currentPage = 1;
                loadBookmarks();
            });
        });

        // Load bookmarks for current category and page
        function loadBookmarks() {
            if (!currentCategoryId) return;

            fetch(`api/bookmarks.php?category_id=${currentCategoryId}&page=${currentPage}&limit=${itemsPerPage}`)
                .then(response => response.json())
                .then(data => {
                    const container = document.getElementById('bookmarksContainer');
                    if (data.success) {
                        container.innerHTML = data.html;
                        updatePagination(data.total, data.pages);
                    } else {
                        container.innerHTML = `<div class="alert alert-danger">${data.message}</div>`;
                    }
                })
                .catch(error => {
                    showAlert('danger', 'Error loading bookmarks');
                    console.error('Error:', error);
                });
        }

        // Update pagination
        function updatePagination(total, pages) {
            const pagination = document.getElementById('pagination');
            pagination.innerHTML = '';

            if (pages <= 1) return;

            // Previous button
            pagination.innerHTML += `
                <li class="page-item ${currentPage === 1 ? 'disabled' : ''}">
                    <a class="page-link" href="#" data-page="${currentPage - 1}">Previous</a>
                </li>
            `;

            // Page numbers
            for (let i = 1; i <= pages; i++) {
                pagination.innerHTML += `
                    <li class="page-item ${currentPage === i ? 'active' : ''}">
                        <a class="page-link" href="#" data-page="${i}">${i}</a>
                    </li>
                `;
            }

            // Next button
            pagination.innerHTML += `
                <li class="page-item ${currentPage === pages ? 'disabled' : ''}">
                    <a class="page-link" href="#" data-page="${currentPage + 1}">Next</a>
                </li>
            `;

            // Add event listeners
            document.querySelectorAll('.page-link').forEach(link => {
                link.addEventListener('click', function(e) {
                    e.preventDefault();
                    currentPage = parseInt(this.getAttribute('data-page'));
                    loadBookmarks();
                });
            });
        }

        // Handle bookmark modal form
        handleModalForm('bookmarkModal', 'bookmarkForm');

        // Handle delete button clicks
        document.addEventListener('click', function(e) {
            if (e.target.classList.contains('delete-btn')) {
                if (confirm('Are you sure you want to delete this bookmark?')) {
                    const formData = new FormData();
                    formData.append('action', 'delete');
                    formData.append('id', e.target.getAttribute('data-id'));

                    fetch('api/bookmarks.php', {
                        method: 'POST',
                        body: formData
                    })
                    .then(response => response.json())
                    .then(data => {
                        if (data.success) {
                            showAlert('success', data.message);
                            loadBookmarks();
                        } else {
                            showAlert('danger', data.message);
                        }
                    })
                    .catch(error => {
                        showAlert('danger', 'An error occurred while deleting the bookmark.');
                        console.error('Error:', error);
                    });
                }
            }
        });
    </script>
</body>
</html> 