<?php
require_once 'utils.php';

header('Content-Type: application/json');

$action = $_GET['action'] ?? '';

switch ($action) {
    case 'getStudentsByGroup':
        $groupName = $_GET['group_id'] ?? '';
        $courseId = $_GET['course_id'] ?? '';
        $page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
        $studentsPerPage = 4; 
        $offset = ($page - 1) * $studentsPerPage;
        
        $totalStudents = getTotalStudentsInGroup($groupName);
        $totalPages = $totalStudents > 0 ? ceil($totalStudents / $studentsPerPage) : 1;
        
        $students = getStudentsByGroup($groupName, $offset, $studentsPerPage);
        
        if ($courseId) {
            foreach ($students as &$student) {
                $grade = getStudentGradeForCourse($student['id'], $courseId);
                $student['grade'] = $grade ? $grade['grade'] : '';
            }
        }
        
        echo json_encode([
            'students' => $students,
            'pagination' => [
                'current_page' => $page,
                'total_pages' => $totalPages,
                'total_students' => $totalStudents
            ]
        ]);
        break;
        
    default:
        http_response_code(400);
        echo json_encode(['error' => 'Invalid action']);
        break;
}
?> 