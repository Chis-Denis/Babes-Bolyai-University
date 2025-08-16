<?php
require_once 'includes/utils.php';
require_once 'includes/layout.php';

requireStudent();

$studentId = getCurrentUserId();

$grades = getGradesByStudentId($studentId);

renderHeader('My Grades - Grade Management System');
?>

<h1>My Grades</h1>

<?php if (empty($grades)): ?>
    <p>You don't have any grades yet.</p>
<?php else: ?>
    <table>
        <thead>
            <tr>
                <th>Course</th>
                <th>Professor</th>
                <th>Grade</th>
                <th>Last Updated</th>
            </tr>
        </thead>
        <tbody>
            <?php foreach ($grades as $grade): ?>
                <tr>
                    <td><?= htmlspecialchars($grade['course_name']) ?></td>
                    <td><?= htmlspecialchars($grade['professor_name']) ?></td>
                    <td class="grade <?= getGradeClass($grade['grade']) ?>"><?= number_format($grade['grade'], 2) ?></td>
                    <td><?= date('M d, Y H:i', strtotime($grade['updated_at'])) ?></td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>
<?php endif; ?>

<?php renderFooter(); ?> 