CREATE DATABASE IF NOT EXISTS grade_system;
USE grade_system;

DROP TABLE IF EXISTS grades;
DROP TABLE IF EXISTS students;
DROP TABLE IF EXISTS courses;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS groups;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('student', 'professor') NOT NULL,
    name VARCHAR(100) NOT NULL,
    class_group VARCHAR(50) NULL 
);

CREATE TABLE IF NOT EXISTS courses (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    professor_id INT,
    FOREIGN KEY (professor_id) REFERENCES users(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS grades (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    course_id INT NOT NULL,
    grade DECIMAL(4,2) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    UNIQUE KEY (student_id, course_id)
);

INSERT INTO users (username, password, role, name) VALUES
('prof1', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'professor', 'Professor Smith'),
('prof2', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'professor', 'Professor Johnson');

INSERT INTO courses (name, professor_id) VALUES
('Mathematics', 1),
('Physics', 1),
('Computer Science', 2),
('Biology', 2);

INSERT INTO users (username, password, role, name, class_group) VALUES
('student1', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'John Doe', 'Group A'),
('student2', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Jane Smith', 'Group A'),
('student3', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Mike Johnson', 'Group A'),
('student4', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Sarah Brown', 'Group A'),
('student5', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'David Wilson', 'Group A'),
('student6', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Emily Davis', 'Group B'),
('student7', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Michael Miller', 'Group B'),
('student8', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Emma Taylor', 'Group B'),
('student9', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Daniel Anderson', 'Group B'),
('student10', '$2y$10$d9NLQ6WTW5iURYMpdv2rE.y8daJk8.iLuI1T5IKqpg3f8rCfPGeIC', 'student', 'Olivia Thomas', 'Group B');

INSERT INTO grades (student_id, course_id, grade) VALUES
(3, 1, 8.50), -- John Doe - Mathematics
(3, 2, 7.75), -- John Doe - Physics
(4, 1, 9.00), -- Jane Smith - Mathematics
(4, 3, 8.25), -- Jane Smith - Computer Science
(5, 2, 7.00), -- Mike Johnson - Physics
(5, 4, 9.50), -- Mike Johnson - Biology
(6, 3, 8.75), -- Sarah Brown - Computer Science
(6, 4, 7.50), -- Sarah Brown - Biology
(7, 1, 6.75), -- David Wilson - Mathematics
(8, 2, 9.25); -- Emily Davis - Physics 