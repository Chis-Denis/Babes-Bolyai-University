class Student:

    def __init__(self, id: int, name: str, group: int):
        self.__id = int(id)
        self.__name = name
        self.__group = int(group)

    @property
    def id(self):
        return self.__id

    @property
    def name(self):
        return self.__name

    @property
    def group(self):
        return self.__group

    def set_id(self, new_id):
        self.__id = new_id

    def set_name(self, new_name):
        self.__name = new_name

    def set_group(self, new_group):
        self.__group = new_group

    def __str__(self):
        return f"ID:{self.id}, Name:{self.name}, Group:{self.group}"

    def __eq__(self, other):
        return self.id == other.id and self.name == other.name and self.group == other.group
