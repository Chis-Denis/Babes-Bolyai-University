from src.Repository.Repository import Repository
from src.Repository.Binary_Repository import Binary_Repository
from src.Repository.File_Repository import File_Repository
from src.Domain.Domain import Student
import pickle
import random
import copy

class Service:

    generated_students = ["Cristi", "Andrei", "Mihai", "Alex", "Ionut", "Vlad", "George", "Marius", "Catalin", "Razvan"]
    generated_groups = ["911", "912", "913", "914", "915", "916", "917", "918", "919", "920"]
    generated_ids = [index for index in range(1, 11)]
    def __init__(self, type_of_repository):
        if type_of_repository == "Repository":
            self.__type_of_repository = Repository()
        elif type_of_repository == "Binary_Repository":
            self.__type_of_repository = Binary_Repository()
        elif type_of_repository == "File_Repository":
            self.__type_of_repository = File_Repository()

        self.__undo_stack = []

    def save_state(self):
        self.__undo_stack.append(copy.deepcopy(self.__type_of_repository))

    def add_student(self, student_id, student_name, student_group):
        self.save_state()
        self.__type_of_repository.add_student(student_id, student_name, student_group)

    def remove_student(self, student_id):
        self.save_state()
        self.__type_of_repository.remove_student(student_id)

    def update_student(self, student_id, new_id, new_name, new_group):
        self.save_state()
        self.__type_of_repository.update_student(student_id, new_id, new_name, new_group)

    def filter_by_group(self, group):
        self.save_state()
        self.__type_of_repository.filter_by_group(group)

    def get_students(self):
        return self.__type_of_repository.get_students()

    def undo(self):
        if len(self.__undo_stack) == 0:
            print("No more undos")
            return
        self.__type_of_repository = self.__undo_stack.pop()

    def generate_students(self):
        for i in range(10):
            student_id = random.choice(self.generated_ids)
            self.generated_ids.remove(student_id)
            student_name = random.choice(self.generated_students)
            student_group = random.choice(self.generated_groups)
            self.add_student(student_id, student_name, student_group)

    def generate_students_pickle(self):
        # Define initial list of students
        students = [
            Student(1, "Cristi", 911),
            Student(2, "Andrei", 912),
            Student(3, "Mihai", 913),
            Student(4, "Alex", 914),
            Student(5, "Ionut", 915),
            Student(6, "Vlad", 916),
            Student(7, "George", 917),
            Student(8, "Marius", 918),
            Student(9, "Catalin", 919),
            Student(10, "Razvan", 920)
        ]

        file_path = r"C:\Users\Chisu\PycharmProjects\a7-ChisuulR\src\Repository\students.pyc"

        with open(file_path, "wb") as file:
            pickle.dump(students, file)
