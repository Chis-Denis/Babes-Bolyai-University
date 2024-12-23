
class Discipline:

    def __init__(self, discipline_id, name):
        self.__discipline_id = discipline_id
        self.__name = name

    @property
    def discipline_id(self):
        return self.__discipline_id

    @property
    def name(self):
        return self.__name

    def set_name(self, name):
        self.__name = name

    def __str__(self):
        return f"{self.__discipline_id} - {self.__name}"