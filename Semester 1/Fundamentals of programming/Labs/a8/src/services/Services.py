from src.repository.RepoStudent import RepoStudent
from src.repository.RepoDiscipline import RepoDiscipline
from src.repository.RepoGrade import RepoGrade

from src.repository.RepoStudent import RepoStudentFile
from src.repository.RepoDiscipline import RepoDisciplineFile
from src.repository.RepoGrade import RepoGradeFile

from src.repository.RepoStudent import RepoStudentPickle
from src.repository.RepoDiscipline import RepoDisciplinePickle
from src.repository.RepoGrade import RepoGradePickle


class Services:
    def __init__(self, data):
        self.__data = self.read_settings()
        self.choose_repo(data)
        self.__repo_student = None
        self.__repo_discipline = None
        self.__repo_grade = None


    def get_data(self):
        return self.__data

    def read_settings(self):
        with open("settings.properties", "r") as file:
            lines = file.readlines()
            data = {}
            for line in lines:
                line = line.strip()
                key, value = line.split("=")
                data[key] = value
            return data

    def choose_repo(self, data):
        if data == "inmemory":
            self.__repo_student = RepoStudent()
            self.__repo_discipline = RepoDiscipline()
            self.__repo_grade = RepoGrade()
        elif data == "file":
            self.__repo_student = RepoStudentFile(self.__data["students"])
            self.__repo_discipline = RepoDisciplineFile(self.__data["disciplines"])
            self.__repo_grade = RepoGradeFile(self.__data["grades"])
            gradeFile = self.__data["grades"]
            studentFile = self.__data["students"]
            disciplineFile = self.__data["disciplines"]
        elif data == "pickle":
            self.__repo_student = RepoStudentPickle(self.__data["students"])
            self.__repo_discipline = RepoDisciplinePickle(self.__data["disciplines"])
            self.__repo_grade = RepoGradePickle(self.__data["grades"])
            gradeFile = self.__data["grades"]
            studentFile = self.__data["students"]
            disciplineFile = self.__data["disciplines"]
        else:
            raise ValueError("Invalid repository type")

    def add_student(self, student):
        self.__repo_student.add_student(student)

    def remove_student(self, student_id):
        self.__repo_student.remove_student(student_id)

    def update_student(self, student_id, new_name):
        self.__repo_student.update_student(student_id, new_name)

    def get_all_students(self):
        return self.__repo_student.get_all_students()

    def get_student_by_id(self, student_id):
        return self.__repo_student.get_student_by_id(student_id)

