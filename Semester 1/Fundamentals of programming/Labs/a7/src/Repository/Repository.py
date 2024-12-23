from src.Domain.Domain import Student
import copy

class Repository:

    def __init__(self):
        self.__elements = []

    def get_students(self):
        return self.__elements

    def add_student(self, student_id, student_name, student_group):
        student = Student(student_id, student_name, student_group)
        self.__elements.append(student)

    def remove_student(self, student_id):
        for student in self.__elements:
            if student.id == student_id:
                self.__elements.remove(student)
                return
        raise ValueError("Student not in the list")
    def update_student(self, student_id, new_id, new_name, new_group):
        for student in self.__elements:
            if student.id == student_id:
                student.set_id(new_id)
                student.set_name(new_name)
                student.set_group(new_group)

    def filter_by_group(self, group):
        self.__elements = [student for student in self.__elements if student.group != group]

    def __deepcopy__(self, memo):
        # Ensure deepcopy works for the repository
        copy_obj = Repository()
        copy_obj.__elements = copy.deepcopy(self.__elements, memo)
        return copy_obj
