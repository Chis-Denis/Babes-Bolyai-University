
class Student:

    def __init__(self, student_id, name,):
        self.__name = name
        self.__student_id = student_id

    @property
    def student_id(self):
        return self.__student_id

    @property
    def name(self):
        return self

    def set_name(self, name):
        self.__name = name


    def __str__(self):
        return f"{self.__student_id} - {self.__name}"


