import random


def print_initial_menu():

    print("1.Generate a list of n numbers\n"
          "2.Search for a number in the list\n"
          "3.Sort with selection sort\n"
          "4.Sort with gnome sort\n"
          "5.Print the list\n"
          "6.Exit\n")

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

def selection_sort(generated_list, step):

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

def gnome_sort(generated_list, step):

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

def check_if_sorted(generated_list):

    for index in range(len(generated_list) - 1):
        if generated_list[index] > generated_list[index + 1]:
            return False
    return True

def start():

    generated_list = []

    while True:

        print_initial_menu()
        choice = int(input("Enter your choice: "))

        if choice == 1:

            size = int(input("Enter the size of the list: "))
            generated_list = generate_list(size)
            print(generated_list)
            print("\n")

        elif choice == 2:

            if not check_if_sorted(generated_list):
                print("List is not sorted. Please sort the list first")
                continue

            number_to_search = int(input("Enter the number to search: "))
            number_exists = binary_search(generated_list, number_to_search)

            if number_exists:
                print("Number exists in the list")
            else:
                print("Number does not exist in the list")

        elif choice == 3:

            step = int(input("Enter the value of step: "))
            sorted_list = selection_sort(generated_list, step)
            print(sorted_list)
            print("\n")

        elif choice == 4:

            step = int(input("Enter the value of step: "))
            sorted_list = gnome_sort(generated_list, step)
            print(sorted_list)
            print("\n")

        elif choice == 5:

            print(generated_list)
            print("\n")

        elif choice == 6:

            exit(0)


start()