document.addEventListener('DOMContentLoaded', function() {
    loadTasks();
    let currentTaskId = null;

    document.getElementById('saveTask').addEventListener('click', function() {
        const taskData = {
            title: document.getElementById('title').value.trim(),
            description: document.getElementById('description').value.trim(),
            due_date: document.getElementById('due_date').value,
            status: document.getElementById('status').value
        };

        // Validation
        if (!taskData.title) {
            alert('Title is required');
            return;
        }

        const url = 'api/tasks.php';
        const method = currentTaskId ? 'PUT' : 'POST';
        
        if (currentTaskId) {
            taskData.id = currentTaskId;
        }

        fetch(url, {
            method: method,
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(taskData)
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                loadTasks();
                const modal = bootstrap.Modal.getInstance(document.getElementById('addTaskModal'));
                modal.hide();
                document.getElementById('addTaskForm').reset();
                currentTaskId = null;
            } else {
                alert('Error: ' + data.message);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while saving the task.');
        });
    });

    // Reset form when modal is closed
    document.getElementById('addTaskModal').addEventListener('hidden.bs.modal', function () {
        document.getElementById('addTaskForm').reset();
        currentTaskId = null;
        document.querySelector('.modal-title').textContent = 'Add New Task';
    });
});

function loadTasks() {
    fetch('api/tasks.php')
        .then(response => response.json())
        .then(data => {
            const tasksList = document.getElementById('tasksList');
            tasksList.innerHTML = '';

            if (data.length === 0) {
                tasksList.innerHTML = '<div class="alert alert-info">No tasks found. Click "Add New Task" to create one.</div>';
                return;
            }

            data.forEach(task => {
                const taskElement = createTaskElement(task);
                tasksList.appendChild(taskElement);
            });
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while loading tasks.');
        });
}

function createTaskElement(task) {
    const div = document.createElement('div');
    div.className = 'card mb-3 task-card';
    div.innerHTML = `
        <div class="card-body">
            <h5 class="card-title">${escapeHtml(task.title)}</h5>
            <p class="card-text">${escapeHtml(task.description)}</p>
            <p class="card-text">
                <small class="text-muted">
                    Due Date: ${task.due_date || 'Not set'}<br>
                    Status: <span class="task-status status-${task.status}">${task.status}</span>
                </small>
            </p>
            <div class="btn-group">
                <button class="btn btn-sm btn-primary" onclick="editTask(${task.id})">Edit</button>
                <button class="btn btn-sm btn-danger" onclick="confirmDelete(${task.id})">Delete</button>
            </div>
        </div>
    `;
    return div;
}

function editTask(taskId) {
    fetch(`api/tasks.php?id=${taskId}`)
        .then(response => response.json())
        .then(task => {
            document.getElementById('title').value = task.title;
            document.getElementById('description').value = task.description;
            document.getElementById('due_date').value = task.due_date;
            document.getElementById('status').value = task.status;
            
            currentTaskId = task.id;
            document.querySelector('.modal-title').textContent = 'Edit Task';
            
            const modal = new bootstrap.Modal(document.getElementById('addTaskModal'));
            modal.show();
        })
        .catch(error => {
            console.error('Error:', error);
            alert('An error occurred while loading the task details.');
        });
}

function confirmDelete(taskId) {
    if (confirm('Are you sure you want to delete this task? This action cannot be undone.')) {
        deleteTask(taskId);
    }
}

function deleteTask(taskId) {
    fetch(`api/tasks.php?id=${taskId}`, {
        method: 'DELETE'
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            loadTasks();
        } else {
            alert('Error: ' + data.message);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('An error occurred while deleting the task.');
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