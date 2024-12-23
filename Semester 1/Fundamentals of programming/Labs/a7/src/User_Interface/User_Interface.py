from src.Service.Service import Service
class User_Interface:

    def __init__(self):
        self.__service = ""

    def run_program(self):

        normal_repository_choice = "1"
        binary_repository_choice = "2"
        file_repository_choice = "3"

        self.print_repository_menu()
        repository_choice = input("Choose repository: ")
        if repository_choice == normal_repository_choice:
            self.__service = Service("Repository")
        elif repository_choice == binary_repository_choice:
            self.__service = Service("Binary_Repository")
        elif repository_choice == file_repository_choice:
            self.__service = Service("File_Repository")
        else:
            print("Invalid choice")

        if repository_choice == normal_repository_choice:
            self.__service.generate_students()

        elif repository_choice == binary_repository_choice:
            self.__service.generate_students_pickle()

        while True:

            add_student_choice = "1"
            remove_student_choice = "2"
            update_student_choice = "3"
            filter_by_group_choice = "4"
            print_students_choice = "5"
            undo_choice = "6"
            exit_choice = "7"

            self.print_menu()
            command_inputed_for_menu = input("Enter command: ")
            if command_inputed_for_menu == add_student_choice:
                student_id = input("Enter student id: ")
                student_name = input("Enter student name: ")
                student_group = input("Enter student group: ")
                self.__service.add_student(int(student_id), student_name, int(student_group))

            elif command_inputed_for_menu == remove_student_choice:
                student_id = input("Enter student id: ")
                self.__service.remove_student(int(student_id))

            elif command_inputed_for_menu == update_student_choice:
                student_id = input("Enter student id: ")
                new_student_id = input("Enter new student id: ")
                new_student_name = input("Enter new student name: ")
                new_student_group = input("Enter new student group: ")
                self.__service.update_student(int(student_id), int(new_student_id), new_student_name, int(new_student_group))

            elif command_inputed_for_menu == filter_by_group_choice:
                student_group = int(input("Enter student group: "))
                self.__service.filter_by_group(student_group)

            elif command_inputed_for_menu == print_students_choice:
                students = self.__service.get_students()
                for student in students:
                    print(student)
                print("\n")

            elif command_inputed_for_menu == undo_choice:
                self.__service.undo()

            elif command_inputed_for_menu == exit_choice:
                exit(0)

            else:
                print("Invalid command")

    def print_menu(self):
        print("1. Add student\n"
              "2. Remove student\n"
              "3. Update student\n"
              "4. Filter by group\n"
              "5. Print students\n"
              "6. Undo\n"
              "7. Exit\n")

    def print_repository_menu(self):
        print("1. In memory repository\n"
              "2. Binary repository\n"
              "3. File repository\n")



if __name__ == "__main__":
    ui = User_Interface()
    ui.run_program()