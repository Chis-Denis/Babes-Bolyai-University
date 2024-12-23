import functions

def start_program():
    add_remove_list_filter_insert_option = 0

    command_of_one_word = 1
    command_of_two_words = 2
    command_of_four_words = 4

    list_of_expenses = []
    history = []

    functions.generate_expenses(list_of_expenses, 10)

    while True:
        print_menu()
        command_inputed = input("Insert command: ").strip()
        if not command_inputed:
            print("Error: No command entered. Please try again.")
            continue

        command_parts = command_inputed.split()
        command = command_parts[add_remove_list_filter_insert_option]

        try:
            if command == "add":
                day_inputed = 1
                expense_value_inputed = 2
                category_inputed = 3
                if len(command_parts) != command_of_four_words:
                    raise ValueError("Usage: add <day> <sum> <category>")
                day, expense_value, category = int(command_parts[day_inputed]), int(command_parts[expense_value_inputed]), command_parts[category_inputed]
                functions.add_expense(list_of_expenses, day, expense_value, category)

            elif command == "insert":
                day_inputed = 1
                expense_value_inputed = 2
                category_inputed = 3
                if len(command_parts) != command_of_four_words:
                    raise ValueError("Usage: insert <day> <sum> <category>")
                day, expense_value, category = int(command_parts[day_inputed]), int(command_parts[expense_value_inputed]), command_parts[category_inputed]
                functions.insert_expense(list_of_expenses, day, expense_value, category)

            elif command == "remove":
                day_inputed = 1
                start_day_inputed = 1
                end_day_inputed = 3
                expense_value_inputed = 2
                category_inputed = 2
                if len(command_parts) == command_of_two_words:
                    functions.remove_day(list_of_expenses, int(command_parts[day_inputed]))
                elif len(command_parts) == command_of_four_words and command_parts[expense_value_inputed] == "to":
                    functions.remove_start_end(list_of_expenses, int(command_parts[start_day_inputed]), int(command_parts[end_day_inputed]))
                elif len(command_parts) == command_of_two_words:
                    functions.remove_category(list_of_expenses, command_parts[category_inputed])
                else:
                    raise ValueError("Usage: remove <day> | <start day> to <end day> | <category>")

            elif command == "list":
                category_inputed = 1
                list_condition_inputed = 2
                list_value_condition_inputed = 3
                if len(command_parts) == command_of_one_word:
                    for expense in list_of_expenses:
                        functions.list_expenses(expense)
                        print(expense)
                elif len(command_parts) == command_of_two_words:
                    print(functions.list_category(list_of_expenses, command_parts[category_inputed]))

                elif len(command_parts) == command_of_four_words:
                    print(functions.list_category_value(list_of_expenses, command_parts[category_inputed], command_parts[list_condition_inputed], int(command_parts[list_value_condition_inputed])))
                else:
                    raise ValueError("Usage: list | list <category> | list <category> [ < | = | > ] <value>")

            elif command == "filter":
                category_inputed = 1
                filter_condition_inputed = 2
                filter_value_condition_inputed = 3
                if len(command_parts) == command_of_two_words:
                    functions.filter_category(list_of_expenses, command_parts[category_inputed])
                elif len(command_parts) == command_of_four_words:
                    functions.filter_category_value(list_of_expenses, command_parts[category_inputed], command_parts[filter_condition_inputed], int(command_parts[filter_value_condition_inputed]))
                else:
                    raise ValueError("Usage: filter <category> | filter <category> [ < | = | > ] <value>")

            elif command == "undo":
                if not history:
                    print("Error: No actions to undo.")
                else:
                    list_of_expenses.clear()
                    list_of_expenses.extend(history.pop())

            elif command == "exit":
                print("Exiting the program. Goodbye!")
                break

            else:
                print(f"Error: Unknown command '{command}'. Please try again.")

            # Save state to history for undo functionality
            history.append(list_of_expenses[:])

        except ValueError as ve:
            print(f"Error: {ve}")
        except IndexError:
            print("Error: Incorrect number of arguments. Please check your command and try again.")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")

def print_menu():
    print(
        "               HELP MENU\n",
        "add <day> <sum> <category>\n",
        "insert <day> <sum> <category>\n",
        "remove <day>\n",
        "remove <start day> to <end day>\n",
        "remove <category>\n",
        "list\n",
        "list <category>\n",
        "list <category> [ < | = | > ] <value>\n",
        "filter <category>\n",
        "filter <category> [ < | = | > ] <value>\n",
        "undo\n",
        "exit\n"
    )

if __name__ == "__main__":
    start_program()
