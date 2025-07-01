<?php
// Start the session to access user session data
session_start();
require_once 'config.php';

// Redirect to login if user is not logged in
if (!isset($_SESSION['user_id'])) {
    header('Location: login.php');
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Bookmarks</title>
    <!-- Bootstrap CSS for styling -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="css/style.css" rel="stylesheet">
</head>
<body>
    <?php include 'navbar.php'; ?> <!-- Navigation bar -->

    <div class="container mt-4">
        <div class="row">
            <!-- Sidebar: Categories -->
            <div class="col-md-3">
                <div class="card">
                    <div class="card-header">
                        <h5 class="mb-0">Categories</h5>
                    </div>
                    <div class="card-body">
                        <div id="categoriesList"></div> <!-- List of categories will be loaded here -->
                        <button class="btn btn-primary w-100 mt-3" data-bs-toggle="modal" data-bs-target="#addCategoryModal">
                            Add Category
                        </button>
                    </div>
                </div>
            </div>
            <!-- Main Content: Bookmarks -->
            <div class="col-md-9">
                <div class="d-flex justify-content-between align-items-center mb-4">
                    <h2>My Bookmarks</h2>
                    <button class="btn btn-primary" data-bs-toggle="modal" data-bs-target="#addBookmarkModal">
                        Add Bookmark
                    </button>
                </div>
                <div id="bookmarksList"></div> <!-- List of bookmarks will be loaded here -->
                <div class="d-flex justify-content-between mt-3">
                    <button class="btn btn-secondary" id="prevPage" disabled>Previous</button>
                    <span id="pageInfo"></span>
                    <button class="btn btn-secondary" id="nextPage" disabled>Next</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Add/Edit Bookmark Modal -->
    <div class="modal fade" id="addBookmarkModal" tabindex="-1">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title">Add Bookmark</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
                </div>
                <div class="modal-body">
                    <form id="bookmarkForm" class="needs-validation" novalidate>
                        <div class="form-group">
                            <label for="url" class="form-label">URL</label>
                            <input type="url" class="form-control" id="url" required>
                            <div class="invalid-feedback">Please enter a valid URL.</div>
                        </div>
                        <div class="form-group">
                            <label for="title" class="form-label">Title</label>
                            <input type="text" class="form-control" id="title" required>
                            <div class="invalid-feedback">Please enter a title.</div>
                        </div>
                        <div class="form-group">
                            <label for="description" class="form-label">Description</label>
                            <textarea class="form-control" id="description" rows="3"></textarea>
                        </div>
                        <div class="form-group">
                            <label for="category" class="form-label">Category</label>
                            <select class="form-control" id="category" required>
                                <option value="">Select a category</option>
                            </select>
                            <div class="invalid-feedback">Please select a category.</div>
                        </div>
                    </form>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Cancel</button>
                    <button type="button" class="btn btn-primary" id="saveBookmark">Save</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Add Category Modal -->
    <div class="modal fade" id="addCategoryModal" tabindex="-1">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title">Add Category</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
                </div>
                <div class="modal-body">
                    <form id="categoryForm" class="needs-validation" novalidate>
                        <div class="form-group">
                            <label for="categoryName" class="form-label">Name</label>
                            <input type="text" class="form-control" id="categoryName" required>
                            <div class="invalid-feedback">Please enter a category name.</div>
                        </div>
                        <div class="form-group">
                            <label for="categoryDescription" class="form-label">Description</label>
                            <textarea class="form-control" id="categoryDescription" rows="3"></textarea>
                        </div>
                    </form>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Cancel</button>
                    <button type="button" class="btn btn-primary" id="saveCategory">Save</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Bootstrap JS and custom JS for interactivity -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"></script>
    <script src="js/bookmarks.js"></script>
</body>
</html> 