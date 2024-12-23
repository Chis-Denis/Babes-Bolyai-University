from src.domain.Student import Student
import pickle

class RepoStudent:
    def __init__(self):
        self.__students = []

    def add_student(self, student):
        self.__students.append(student)

    def remove_student(self, student_id):
        for i in range(len(self.__students)):
            if self.__students[i].student_id == student_id:
                del self.__students[i]
                return

    def update_student(self, student_id, new_name):
        for student in self.__students:
            if student.student_id == student_id:
                student.set_name(new_name)
                return

    def get_all_students(self):
        return self.__students

    def get_student_by_id(self, student_id):
        for student in self.__students:
            if student.student_id == student_id:
                return student
        return None


class RepoStudentFile:
    def __init__(self, file_name):
        self.__file_name = file_name
        self.__students = []
        self.__load_from_file()

    def add_student(self, student):
        self.__students.append(student)
        self.__save_to_file()

    def remove_student(self, student_id):
        for i in range(len(self.__students)):
            if self.__students[i].student_id == student_id:
                del self.__students[i]
                self.__save_to_file()
                return

    def update_student(self, student_id, new_name):
        for student in self.__students:
            if student.student_id == student_id:
                student.set_name(new_name)
                self.__save_to_file()
                return

    def get_all_students(self):
        return self.__students

    def get_student_by_id(self, student_id):
        for student in self.__students:
            if student.student_id == student_id:
                return student
        return None

    def __load_from_file(self):
        with open(self.__file_name, "r") as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip()
                if line == "":
                    continue
                parts = line.split(",")
                student_id = int(parts[0])
                name = parts[1]
                student = Student(student_id, name)
                self.__students.append(student)

    def __save_to_file(self):
        with open(self.__file_name, "w") as f:
            for student in self.__students:
                f.write(str(student.student_id) + "," + student.name + "\n")


class RepoStudentPickle:
    def __init__(self, file_name):
        self.__file_name = file_name
        self.__students = []
        self.__load_from_file()

    def add_student(self, student):
        self.__students.append(student)
        self.__save_to_file()

    def remove_student(self, student_id):
        for i in range(len(self.__students)):
            if self.__students[i].student_id == student_id:
                del self.__students[i]
                self.__save_to_file()
                return

    def update_student(self, student_id, new_name):
        for student in self.__students:
            if student.student_id == student_id:
                student.set_name(new_name)
                self.__save_to_file()
                return

    def get_all_students(self):
        return self.__students

    def get_student_by_id(self, student_id):
        for student in self.__students:
            if student.student_id == student_id:
                return student
        return None

    def __load_from_file(self):
        with open(self.__file_name, "rb") as f:
            self.__students = pickle.load(f)

    def __save_to_file(self):
        with open(self.__file_name, "wb") as f:
            pickle.dump(self.__students, f)