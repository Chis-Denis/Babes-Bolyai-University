<?php
require_once 'includes/utils.php';
require_once 'includes/layout.php';

requireProfessor();

$message = '';
$errorMessage = '';

$groups = getAllGroups();

$courses = getCoursesByProfessorId(getCurrentUserId());

$selectedGroupName = $_GET['group_id'] ?? null;
$selectedCourseId = $_GET['course_id'] ?? null;
$page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
$studentsPerPage = 4; 

$offset = ($page - 1) * $studentsPerPage;

$totalStudents = $selectedGroupName ? getTotalStudentsInGroup($selectedGroupName) : 0;
$totalPages = $totalStudents > 0 ? ceil($totalStudents / $studentsPerPage) : 1;
$students = $selectedGroupName ? getStudentsByGroup($selectedGroupName, $offset, $studentsPerPage) : [];

if ($page < 1) $page = 1;
if ($page > $totalPages) $page = $totalPages;

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['save_grades'])) {
    $courseId = $_POST['course_id'] ?? null;
    $studentGrades = $_POST['grades'] ?? [];
    
    if ($courseId && !empty($studentGrades)) {
        $success = true;
        
        foreach ($studentGrades as $studentId => $grade) {
            if (empty($grade)) {
                continue;
            }
            
            if (is_numeric($grade)) {
                $grade = (float)$grade;
                
                if ($grade < 1 || $grade > 10) {
                    $success = false;
                    $errorMessage = "Grades must be between 1 and 10.";
                    break;
                }
                
                $grade = round($grade, 2);
                
                if (!upsertGrade($studentId, $courseId, $grade)) {
                    $success = false;
                    $errorMessage = "Failed to save grade for one or more students.";
                    break;
                }
            } else {
                $success = false;
                $errorMessage = "Invalid grade value. Please enter a valid number.";
                break;
            }
        }
        
        if ($success) {
            $message = "Grades saved successfully!";
        }
    } else {
        $errorMessage = "Missing course or student data.";
    }
}

renderHeader('Manage Grades - Grade Management System');
?>

<h1>Manage Student Grades</h1>

<div class="form-group">
    <label for="group">Select a Group:</label>
    <select id="group">
        <option value="">-- Select Group --</option>
        <?php foreach ($groups as $group): ?>
            <option value="<?= htmlspecialchars($group['name']) ?>" <?= $selectedGroupName == $group['name'] ? 'selected' : '' ?>>
                <?= htmlspecialchars($group['name']) ?>
            </option>
        <?php endforeach; ?>
    </select>
</div>

<div class="form-group" id="courseSelectionContainer" style="display: <?= $selectedGroupName ? 'block' : 'none' ?>;">
        <label for="course">Select a Course:</label>
    <select id="course">
            <option value="">-- Select Course --</option>
            <?php foreach ($courses as $course): ?>
                <option value="<?= $course['id'] ?>" <?= $selectedCourseId == $course['id'] ? 'selected' : '' ?>>
                    <?= htmlspecialchars($course['name']) ?>
                </option>
            <?php endforeach; ?>
        </select>
    </div>
    
<div id="messageContainer"></div>

<div id="studentsContainer"></div>

<script>
function loadStudents(groupId, courseId, page = 1) {
    if (!groupId) {
        document.getElementById('studentsContainer').innerHTML = '';
        return;
    }

    document.getElementById('studentsContainer').innerHTML = '<p>Loading students...</p>';
    
    const url = new URL(window.location);
    url.searchParams.group_id', groupId);
    if (courseId) url.searset('chParams.set('course_id', courseId);
    else url.searchParams.delete('course_id');
    url.searchParams.set('page', page);
    window.history.pushState({}, '', url);
    
    fetch(`includes/ajax_handler.php?action=getStudentsByGroup&group_id=${encodeURIComponent(groupId)}&course_id=${courseId || ''}&page=${page}`)
        .then(response => response.json())
        .then(data => {
            if (data.students.length === 0) {
                document.getElementById('studentsContainer').innerHTML = '<p>No students found in this group.</p>';
                return;
            }
            
            let html = `
                <h2>Students in ${groupId}</h2>
        <form method="post" action="" id="gradeForm">
                    <input type="hidden" name="course_id" value="${courseId || ''}">
            <table>
                <thead>
                    <tr>
                        <th>Student Name</th>
                        <th>Username</th>
                                ${courseId ? '<th>Grade</th>' : ''}
                    </tr>
                </thead>
                <tbody>
            `;
            
            data.students.forEach(student => {
                html += `
                        <tr>
                        <td>${student.name}</td>
                        <td>${student.username}</td>
                `;
                
                if (courseId) {
                    html += `
                                <td>
                            <input type="text" name="grades[${student.id}]" 
                                value="${student.grade || ''}" 
                                placeholder="Enter grade (1-10)"
                                class="grade-input">
                                </td>
                    `;
                }
                
                html += '</tr>';
            });
            
            html += `
                </tbody>
            </table>
            `;
            
            if (courseId) {
                html += `
                <div class="form-group">
                    <button type="submit" name="save_grades">Save Grades</button>
                </div>
                `;
            }
            
            html += '</form>';
            
            if (data.pagination.total_pages > 1) {
                html += '<div class="pagination">';
                
                if (data.pagination.current_page > 1) {
                    html += `<a href="#" onclick="loadStudents('${groupId}', '${courseId || ''}', ${data.pagination.current_page - 1}); return false;">&laquo; Previous</a>`;
                }
                
                for (let i = 1; i <= data.pagination.total_pages; i++) {
                    if (i === data.pagination.current_page) {
                        html += `<span class="active">${i}</span>`;
                    } else {
                        html += `<a href="#" onclick="loadStudents('${groupId}', '${courseId || ''}', ${i}); return false;">${i}</a>`;
                    }
                }
                
                if (data.pagination.current_page < data.pagination.total_pages) {
                    html += `<a href="#" onclick="loadStudents('${groupId}', '${courseId || ''}', ${data.pagination.current_page + 1}); return false;">Next &raquo;</a>`;
                }
                
                html += '</div>';
            }
            
            document.getElementById('studentsContainer').innerHTML = html;
            
    const gradeForm = document.getElementById('gradeForm');
    if (gradeForm) {
        gradeForm.addEventListener('submit', function(e) {
                    const inputs = this.querySelectorAll('.grade-input');
            let valid = true;
            
            inputs.forEach(function(input) {
                        if (input.value) {
                            const gradeValue = parseFloat(input.value);
                            if (isNaN(gradeValue)) {
                                valid = false;
                                input.classList.add('error');
                            } else if (gradeValue < 1 || gradeValue > 10) {
                    valid = false;
                    input.classList.add('error');
                } else {
                    input.classList.remove('error');
                            }
                }
            });
            
            if (!valid) {
                e.preventDefault();
                alert('Please enter valid grades between 1 and 10.');
            }
        });
    }
        })
        .catch(error => {
            console.error('Error loading students:', error);
            document.getElementById('studentsContainer').innerHTML = '<p class="error">Error loading students. Please try again.</p>';
        });
}

document.addEventListener('DOMContentLoaded', function() {
    const groupSelect = document.getElementById('group');
    const courseSelect = document.getElementById('course');
    const courseContainer = document.getElementById('courseSelectionContainer');
    
    groupSelect.addEventListener('change', function() {
        const groupId = this.value;
        
        if (groupId) {
            courseContainer.style.display = 'block';
            courseSelect.selectedIndex = 0; // Reset course selection
        } else {
            courseContainer.style.display = 'none';
        }
        
        loadStudents(groupId, '');
    });
    
    courseSelect.addEventListener('change', function() {
        const groupId = groupSelect.value;
        const courseId = this.value;
        
        loadStudents(groupId, courseId);
    });
    
    if (groupSelect.value) {
        loadStudents(groupSelect.value, courseSelect.value);
    }
    
    <?php if (!empty($message)): ?>
        document.getElementById('messageContainer').innerHTML = '<div class="success"><?= $message ?></div>';
    <?php endif; ?>
    
    <?php if (!empty($errorMessage)): ?>
        document.getElementById('messageContainer').innerHTML = '<div class="error"><?= $errorMessage ?></div>';
    <?php endif; ?>
});
</script>

<?php renderFooter(); ?> 