from src.domain.Discipline import Discipline
import pickle

class RepoDiscipline:
    def __init__(self):
        self.__disciplines = []

    def add_discipline(self, discipline):
        self.__disciplines.append(discipline)

    def remove_discipline(self, discipline_id):
        for i in range(len(self.__disciplines)):
            if self.__disciplines[i].discipline_id == discipline_id:
                del self.__disciplines[i]
                return

    def update_discipline(self, discipline_id, new_name):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                discipline.set_name(new_name)
                return

    def get_all_disciplines(self):
        return self.__disciplines

    def get_discipline_by_id(self, discipline_id):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                return discipline


class RepoDisciplineFile:

    def __init__(self, file_name):
        self.__file_name = file_name
        self.__disciplines = []
        self.__load_from_file()

    def add_discipline(self, discipline):
        self.__disciplines.append(discipline)
        self.__save_to_file()

    def remove_discipline(self, discipline_id):
        for i in range(len(self.__disciplines)):
            if self.__disciplines[i].discipline_id == discipline_id:
                del self.__disciplines[i]
                self.__save_to_file()
                return

    def update_discipline(self, discipline_id, new_name):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                discipline.set_name(new_name)
                self.__save_to_file()
                return

    def get_all_disciplines(self):
        return self.__disciplines

    def get_discipline_by_id(self, discipline_id):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                return discipline
        return None

    def __load_from_file(self):
        try:
            with open(self.__file_name, "r") as file:
                lines = file.readlines()
                for line in lines:
                    parts = line.strip().split(",")
                    discipline = Discipline(int(parts[0]), parts[1])
                    self.__disciplines.append(discipline)
        except FileNotFoundError:
            self.__disciplines = []

    def __save_to_file(self):
        with open(self.__file_name, "w") as file:
            for discipline in self.__disciplines:
                file.write(f"{discipline.discipline_id},{discipline.name}\n")

class RepoDisciplinePickle:

    def __init__(self, file_name):
        self.__file_name = file_name
        self.__disciplines = []
        self.__load_from_file()

    def add_discipline(self, discipline):
        self.__disciplines.append(discipline)
        self.__save_to_file()

    def remove_discipline(self, discipline_id):
        for i in range(len(self.__disciplines)):
            if self.__disciplines[i].discipline_id == discipline_id:
                del self.__disciplines[i]
                self.__save_to_file()
                return

    def update_discipline(self, discipline_id, new_name):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                discipline.set_name(new_name)
                self.__save_to_file()
                return

    def get_all_disciplines(self):
        return self.__disciplines

    def get_discipline_by_id(self, discipline_id):
        for discipline in self.__disciplines:
            if discipline.discipline_id == discipline_id:
                return discipline
        return None

    def __load_from_file(self):
        try:
            with open(self.__file_name, "rb") as file:
                self.__disciplines = pickle.load(file)
        except EOFError:
            self.__disciplines = []
        except FileNotFoundError:
            self.__disciplines = []

    def __save_to_file(self):
        with open(self.__file_name, "wb") as file:
            pickle.dump(self.__disciplines, file)