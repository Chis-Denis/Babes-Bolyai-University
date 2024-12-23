from src.Domain.Domain import Student
import copy

class File_Repository:

    def __init__(self):
        self.__elements = []
        self.__file_name = r"C:\Users\Chisu\PycharmProjects\a7-ChisuulR\src\Repository\students.txt"
        self.load_students()
        self.empty_file()

    def load_students(self):
        with open(self.__file_name, "r") as file:
            lines = file.readlines()
            for line in lines:
                line = line.strip()
                parts = line.split(",")
                student = Student(int(parts[0]), parts[2], int(parts[1]))
                self.__elements.append(student)

    def save_students(self):
        with open(self.__file_name, "w") as file:
            for student in self.__elements:
                file.write(str(student.__str__()) + "\n")

    def empty_file(self):
        with open(self.__file_name, "w") as file:
            pass
    def add_student(self, student_id, student_name, student_group):
        student = Student(student_id, student_name, student_group)
        self.__elements.append(student)
        self.save_students()

    def remove_student(self, student_id):
        for student in self.__elements:
            if student.id == student_id:
                self.__elements.remove(student)
                self.save_students()
                return
        raise ValueError("Student not in the list")

    def update_student(self, student_id, new_student_id, new_student_name, new_student_group):
        for student in self.__elements:
            if student.id == student_id:
                student.set_id(new_student_id)
                student.set_name(new_student_name)
                student.set_group(new_student_group)
        self.save_students()

    def filter_by_group(self, group):
        self.__elements = [student for student in self.__elements if student.group != group]

    def get_students(self):
        return self.__elements

    def __deepcopy__(self, memo):
        # Ensure deepcopy works for the repository
        copy_obj = File_Repository()
        copy_obj.__elements = copy.deepcopy(self.__elements, memo)
        return copy_obj

