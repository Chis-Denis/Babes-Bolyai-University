import random

def generate_expenses(list_of_expenses, number_of_pregenerated_expenses):
    for index in range(number_of_pregenerated_expenses):
        day, category, expense_value = generate_expense()
        list_of_expenses.append((day, category, expense_value))

def generate_expense():
    day = random.randint(1,30)
    category_list = ["housekeeping", "food", "transport", "clothing", "internet", "others"]
    category = random.randint(0,5)
    expense_value = random.randint(1,100) * 100

    return day, category_list[category], expense_value

def add_expense(list_of_expenses, day, expense_value, category):
    list_of_expenses.append((day, category, expense_value))

def insert_expense(list_of_expenses, day, expense_value, category):
    list_of_expenses.append((day, category, expense_value))

def remove_day(list_of_expenses, day):
    first_expense = 0
    for expense in list_of_expenses:
        if expense[first_expense] == day:
            list_of_expenses.remove(expense)

def remove_category(list_of_expenses, category):
    second_expense = 1
    for expense in list_of_expenses:
        if expense[second_expense] == category:
            list_of_expenses.remove(expense)

def remove_start_end(list_of_expenses, start_day, end_day):
    first_expense = 0
    for expense in list_of_expenses:
        if expense[first_expense] >= start_day and expense[first_expense] <= end_day:
            list_of_expenses.remove(expense)

def list_expenses(list_of_expenses):
    for expense in list_of_expenses:
        return(expense)

def list_category(list_of_expenses, category):
    second_expense = 1
    for expense in list_of_expenses:
        if expense[second_expense] == category:
            return(expense)

def list_category_value(list_of_expenses, category, sign, value):
    second_expense = 1
    third_expense = 2
    for expense in list_of_expenses:
        if expense[second_expense] == category:
            if sign == "<":
                if expense[third_expense] < value:
                    print(expense)
            elif sign == "=":
                if expense[third_expense] == value:
                    print(expense)
            elif sign == ">":
                if expense[third_expense] > value:
                    print(expense)

def filter_category(list_of_expenses, category):
    second_expense = 1
    for expense in list_of_expenses:
        if expense[second_expense] != category:
            list_of_expenses.remove(expense)

def filter_category_value(list_of_expenses, category, sign, value):
    second_expense = 1
    third_expense = 2
    for expense in list_of_expenses:
        if expense[second_expense] == category:
            if sign == "<":
                if expense[third_expense] >= value:
                    list_of_expenses.remove(expense)
            elif sign == "=":
                if expense[third_expense] != value:
                    list_of_expenses.remove(expense)
            elif sign == ">":
                if expense[third_expense] <= value:
                    list_of_expenses.remove(expense)

def undo(list_of_expenses, history):
    list_of_expenses.clear()
    for expense in history:
        list_of_expenses.append(expense)
