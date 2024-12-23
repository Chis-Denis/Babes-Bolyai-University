from src.domain.Discipline import Discipline
from src.domain.Student import Student
class Grade(Student, Discipline):

    def __init__(self, student_id, discipline_id, grade_value):
        self.__student_id = student_id
        self.__discipline_id = discipline_id
        self.__grade_value = grade_value
        super().__init__(student_id, discipline_id)

    @property
    def grade_value(self):
        return self.__grade_value

    @property
    def student_id(self):
        return self.__student_id

    @property
    def discipline_id(self):
        return self.__discipline_id

    def set_grade_value(self, grade_value):
        self.__grade_value = grade_value

    def __str__(self):
        return f"{self.__student_id} - {self.__discipline_id} - {self.__grade_value}"