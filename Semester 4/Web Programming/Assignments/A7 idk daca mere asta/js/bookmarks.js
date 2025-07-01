// bookmarks.js
// This file handles all client-side logic for loading, displaying, and managing bookmarks and categories.

// Track the current page, selected category, and bookmark being edited
let currentPage = 1;
let currentCategory = null;
let currentBookmarkId = null;
const itemsPerPage = 4; // Number of bookmarks per page

// Wait for the DOM to be fully loaded before attaching event listeners
// and loading initial data
document.addEventListener('DOMContentLoaded', function() {
    loadCategories(); // Load categories into the sidebar
    loadBookmarks();  // Load bookmarks for the first page

    // Handle saving a bookmark (add or edit)
    document.getElementById('saveBookmark').addEventListener('click', function() {
        const form = document.getElementById('bookmarkForm');
        if (!form.checkValidity()) {
            form.classList.add('was-validated');
            return;
        }

        // Gather bookmark data from the form
        const bookmarkData = {
            url: document.getElementById('url').value.trim(),
            title: document.getElementById('title').value.trim(),
            description: document.getElementById('description').value.trim(),
            category_id: document.getElementById('category').value
        };

        // If editing, include the bookmark ID
        if (currentBookmarkId) {
            bookmarkData.id = currentBookmarkId;
        }

        const url = '/A7/api/bookmarks.php';
        const method = currentBookmarkId ? 'PUT' : 'POST'; // Use PUT for edit, POST for add

        // Send the bookmark data to the server
        fetch(url, {
            method: method,
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(bookmarkData)
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                loadBookmarks(); // Refresh the bookmarks list
                const modal = bootstrap.Modal.getInstance(document.getElementById('addBookmarkModal'));
                modal.hide();
                form.reset();
                form.classList.remove('was-validated');
                currentBookmarkId = null;
            } else {
                alert('Error: ' + data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while saving the bookmark.');
        });
    });

    // Handle saving a new category
    document.getElementById('saveCategory').addEventListener('click', function() {
        const form = document.getElementById('categoryForm');
        if (!form.checkValidity()) {
            form.classList.add('was-validated');
            return;
        }

        // Gather category data from the form
        const categoryData = {
            name: document.getElementById('categoryName').value.trim(),
            description: document.getElementById('categoryDescription').value.trim()
        };

        // Send the category data to the server
        fetch('/A7/api/categories.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(categoryData)
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                loadCategories(); // Refresh the categories list
                const modal = bootstrap.Modal.getInstance(document.getElementById('addCategoryModal'));
                modal.hide();
                form.reset();
                form.classList.remove('was-validated');
            } else {
                alert('Error: ' + data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while saving the category.');
        });
    });

    // Pagination: go to previous page
    document.getElementById('prevPage').addEventListener('click', function() {
        if (currentPage > 1) {
            currentPage--;
            loadBookmarks();
        }
    });

    // Pagination: go to next page
    document.getElementById('nextPage').addEventListener('click', function() {
        currentPage++;
        loadBookmarks();
    });

    // Reset forms and modal state when modals are closed
    document.getElementById('addBookmarkModal').addEventListener('hidden.bs.modal', function () {
        document.getElementById('bookmarkForm').reset();
        document.getElementById('bookmarkForm').classList.remove('was-validated');
        currentBookmarkId = null;
        document.querySelector('#addBookmarkModal .modal-title').textContent = 'Add Bookmark';
    });

    document.getElementById('addCategoryModal').addEventListener('hidden.bs.modal', function () {
        document.getElementById('categoryForm').reset();
        document.getElementById('categoryForm').classList.remove('was-validated');
    });
});

// Load bookmarks from the server and display them
function loadBookmarks() {
    const url = new URL('/A7/api/bookmarks.php', window.location.origin);
    url.searchParams.append('page', currentPage);
    url.searchParams.append('per_page', itemsPerPage);
    if (currentCategory) {
        url.searchParams.append('category_id', currentCategory);
    }

    fetch(url)
        .then(response => response.json())
        .then(data => {
            const bookmarksList = document.getElementById('bookmarksList');
            const prevButton = document.getElementById('prevPage');
            const nextButton = document.getElementById('nextPage');
            const pageInfo = document.getElementById('pageInfo');

            // If no bookmarks, show a message
            if (data.bookmarks.length === 0) {
                bookmarksList.innerHTML = '<div class="alert alert-info">No bookmarks found.</div>';
                prevButton.disabled = true;
                nextButton.disabled = true;
                pageInfo.textContent = '';
                return;
            }

            // Render each bookmark as a card
            bookmarksList.innerHTML = data.bookmarks.map(bookmark => `
                <div class="card mb-3 bookmark-card">
                    <div class="card-body">
                        <h5 class="card-title">
                            <a href="${escapeHtml(bookmark.url)}" target="_blank">${escapeHtml(bookmark.title)}</a>
                        </h5>
                        <p class="card-text">${escapeHtml(bookmark.description || '')}</p>
                        <p class="card-text">
                            <small class="text-muted">
                                Category: ${escapeHtml(bookmark.category_name)}<br>
                                Added: ${new Date(bookmark.created_at).toLocaleDateString()}
                            </small>
                        </p>
                        <div class="btn-group">
                            <button class="btn btn-sm btn-primary" onclick="editBookmark(${bookmark.id})">Edit</button>
                            <button class="btn btn-sm btn-danger" onclick="confirmDelete(${bookmark.id})">Delete</button>
                        </div>
                    </div>
                </div>
            `).join('');

            // Update pagination controls
            prevButton.disabled = currentPage === 1;
            nextButton.disabled = currentPage >= data.total_pages;
            pageInfo.textContent = `Page ${currentPage} of ${data.total_pages}`;
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while loading bookmarks.');
        });
}

// Filter bookmarks by category and reset to first page
function filterByCategory(categoryId) {
    currentCategory = categoryId;
    currentPage = 1;
    loadBookmarks();
}

// Load a bookmark's details into the form for editing
function editBookmark(bookmarkId) {
    fetch(`/A7/api/bookmarks.php?id=${bookmarkId}`)
        .then(response => response.json())
        .then(bookmark => {
            document.getElementById('url').value = bookmark.url;
            document.getElementById('title').value = bookmark.title;
            document.getElementById('description').value = bookmark.description;
            document.getElementById('category').value = bookmark.category_id;
            
            currentBookmarkId = bookmark.id;
            document.querySelector('#addBookmarkModal .modal-title').textContent = 'Edit Bookmark';
            
            const modal = new bootstrap.Modal(document.getElementById('addBookmarkModal'));
            modal.show();
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while loading the bookmark details.');
        });
}

// Ask for confirmation before deleting a bookmark
function confirmDelete(bookmarkId) {
    if (confirm('Are you sure you want to delete this bookmark? This action cannot be undone.')) {
        deleteBookmark(bookmarkId);
    }
}

// Delete a bookmark by ID
function deleteBookmark(bookmarkId) {
    fetch(`/A7/api/bookmarks.php?id=${bookmarkId}`, {
        method: 'DELETE'
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            loadBookmarks();
        } else {
            alert('Error: ' + data.message);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('An error occurred while deleting the bookmark.');
    });
}

function loadCategories() {
    fetch('/A7/api/categories.php')
        .then(response => response.json())
        .then(data => {
            const categoriesList = document.getElementById('categoriesList');
            const categorySelect = document.getElementById('category');
            
            // Update categories list
            categoriesList.innerHTML = `
                <div class="list-group">
                    <a href="#" class="list-group-item list-group-item-action ${!currentCategory ? 'active' : ''}"
                       onclick="filterByCategory(null)">All Categories</a>
                    ${data.map(category => `
                        <a href="#" class="list-group-item list-group-item-action ${currentCategory === category.id ? 'active' : ''}"
                           onclick="filterByCategory(${category.id})">${escapeHtml(category.name)}</a>
                    `).join('')}
                </div>
            `;

            // Update category select
            categorySelect.innerHTML = `
                <option value="">Select a category</option>
                ${data.map(category => `
                    <option value="${category.id}">${escapeHtml(category.name)}</option>
                `).join('')}
            `;
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while loading categories.');
        });
}

function escapeHtml(unsafe) {
    return unsafe
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
} 