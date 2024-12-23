import random
import timeit

def print_initial_menu():

    print("1.Generate a list of n numbers\n"
          "2.Search for a number in the list\n"
          "3.Sort with selection sort\n"
          "4.Sort with gnome sort\n"
          "5.Print the list\n"
          "6.Complexity\n"
          "7.Exit\n")

def print_complexity_menu():

        print("1.Best case\n"
            "2.Worst case\n"
            "3.Average case\n")

def generate_list(size):

    return [random.randint(0, 100) for index in range(size)]

def binary_search(generated_list, number_to_search):

    left = 0
    right = len(generated_list) - 1
    while left <= right:
        mid = (left + right) // 2
        if generated_list[mid] == number_to_search:
            return True
        elif generated_list[mid] < number_to_search:
            left = mid + 1
        else:
            right = mid - 1
    return False

def selection_sort(generated_list):

        for index in range(len(generated_list)):
            min_index = index

            for inner_index in range(index + 1, len(generated_list)):
                if generated_list[inner_index] < generated_list[min_index]:
                    min_index = inner_index
            generated_list[index], generated_list[min_index] = generated_list[min_index], generated_list[index]

        return generated_list

def selection_sort_with_step(generated_list, step):

    cpy_of_step = step

    while check_if_sorted(generated_list) is False:

        for index in range(len(generated_list)):
            min_index = index

            step = step - 1
            if step == 0:
                print(generated_list)
                step = cpy_of_step

            for inner_index in range(index + 1, len(generated_list)):
                if generated_list[inner_index] < generated_list[min_index]:
                    min_index = inner_index
            generated_list[index], generated_list[min_index] = generated_list[min_index], generated_list[index]

    return generated_list

def gnome_sort_with_step(generated_list, step):

        cpy_of_step = step
        index = 0

        while check_if_sorted(generated_list) is False:

            if index == 0:
                index = index + 1

            step = step - 1
            if step == 0:
                print(generated_list)
                step = cpy_of_step

            if generated_list[index] >= generated_list[index - 1]:
                index = index + 1
            else:
                generated_list[index], generated_list[index - 1] = generated_list[index - 1], generated_list[index]
                index = index - 1

        return generated_list

def gnome_sort(generated_list):

        index = 0

        while check_if_sorted(generated_list) is False:

            if index == 0:
                index = index + 1

            if generated_list[index] >= generated_list[index - 1]:
                index = index + 1
            else:
                generated_list[index], generated_list[index - 1] = generated_list[index - 1], generated_list[index]
                index = index - 1

        return generated_list

def check_if_sorted(generated_list):

    for index in range(len(generated_list) - 1):
        if generated_list[index] > generated_list[index + 1]:
            return False
    return True

#same thing for binary_search as selection sort and gnome sort
def best_worst_average_search(choice):
    index = 500
    generated_list = generate_list(index)
    best_number_to_search = generated_list[int(index/2)]
    worst_number_to_search = generated_list[int(index-1)]
    average_number_to_search = generated_list[int(index/4)]
    while index <= 8000:
        if choice == 1:
            print("Best case:", timeit.timeit(lambda: binary_search(generated_list, best_number_to_search), number=10))
        if choice == 2:
            print("Worst case:", timeit.timeit(lambda: binary_search(generated_list, worst_number_to_search), number=10))
        if choice == 3:
            print("Average case:", timeit.timeit(lambda: binary_search(generated_list, average_number_to_search), number=10))

        index = index * 2

def best_sort_selection():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        selection_sort(generated_list)
        print("Best case:", timeit.timeit(lambda: selection_sort(generated_list), number=10))
        index = index * 2

def best_sort_gnome():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        gnome_sort(generated_list)
        print("Best case:", timeit.timeit(lambda: gnome_sort(generated_list), number=10))
        index = index * 2

def worst_sort_selection():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        generated_list = selection_sort(generated_list)
        generated_list.reverse()
        print("Worst case:", timeit.timeit(lambda: selection_sort(generated_list), number=10))
        index = index * 2

def worst_sort_gnome():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        generated_list = gnome_sort(generated_list)
        generated_list.reverse()
        print("Worst case:", timeit.timeit(lambda: gnome_sort(generated_list), number=10))
        index = index * 2

def average_sort_selection():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        print("Average case:", timeit.timeit(lambda: selection_sort(generated_list), number=10))
        index = index * 2

def average_sort_gnome():
    index = 500
    while index <= 8000:
        generated_list = generate_list(index)
        print("Average case:", timeit.timeit(lambda: gnome_sort(generated_list), number=10))
        index = index * 2


def start():

    generated_list = []
    generate_a_list_option = "1"
    search_for_a_number_option = "2"
    sort_with_selection_sort_option = "3"
    sort_with_gnome_sort_option = "4"
    print_the_list_option = "5"
    complexity_option = "6"
    exit_option = "7"

    while True:

        print_initial_menu()
        choice = input("Enter your choice: ")

        if choice == generate_a_list_option:

            size = int(input("Enter the size of the list: "))
            generated_list = generate_list(size)
            print(generated_list)
            print("\n")

        elif choice == search_for_a_number_option:

            if not check_if_sorted(generated_list):
                print("List is not sorted. Please sort the list first")
                continue

            number_to_search = int(input("Enter the number to search: "))
            number_exists = binary_search(generated_list, number_to_search)

            if number_exists:
                print("Number exists in the list")
            else:
                print("Number does not exist in the list")

        elif choice == sort_with_selection_sort_option:

            step = int(input("Enter the value of step: "))
            sorted_list = selection_sort_with_step(generated_list, step)
            print(sorted_list)
            print("\n")

        elif choice == sort_with_gnome_sort_option:

            step = int(input("Enter the value of step: "))
            sorted_list = gnome_sort_with_step(generated_list, step)
            print(sorted_list)
            print("\n")

        elif choice == print_the_list_option:

            print(generated_list)
            print("\n")

        elif choice == complexity_option:

            complexity_gnome_sort_option = "1"
            complexity_selection_sort_option = "2"
            complexity_search_option = "3"

            print("1.Gnome sort\n"
                  "2.Selection sort\n"
                  "3.Search\n")

            choice = input("Enter your choice: ")
            if choice == complexity_gnome_sort_option:

                best_sort_option = "1"
                worst_sort_option = "2"
                average_sort_option = "3"

                print_complexity_menu()
                choice = input("Enter your choice: ")
                if choice == best_sort_option:
                    best_sort_gnome()
                elif choice == worst_sort_option:
                    worst_sort_gnome()
                elif choice == average_sort_option:
                    average_sort_gnome()

                print("For gnome sort\n")

            elif choice == complexity_selection_sort_option:

                best_sort_option = "1"
                worst_sort_option = "2"
                average_sort_option = "3"

                print_complexity_menu()
                choice = input("Enter your choice: ")
                if choice == best_sort_option:
                    best_sort_selection()
                elif choice == worst_sort_option:
                    worst_sort_selection()
                elif choice == average_sort_option:
                    average_sort_selection()

                print("For selection sort\n")

            elif choice == complexity_search_option:

                best_sort_option = "1"
                worst_sort_option = "2"
                average_sort_option = "3"

                print_complexity_menu()
                choice = input("Enter your choice: ")
                if choice == best_sort_option:
                    best_worst_average_search(1)
                elif choice == worst_sort_option:
                    best_worst_average_search(2)
                elif choice == average_sort_option:
                    best_worst_average_search(3)

        elif choice == exit_option:

            exit(0)


start()
