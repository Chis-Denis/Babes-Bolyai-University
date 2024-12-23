import math
from bisect import bisect_left

def create_complex_number_list(real, imaginary):
    return [real, imaginary]


def get_real_list(complex_number):
    first_part_of_the_number = 0
    return complex_number[first_part_of_the_number]


def get_imaginary_list(complex_number):
    second_part_of_the_number = 1
    return complex_number[second_part_of_the_number]


def set_real_list(complex_number, real):
    first_part_of_the_number = 0
    complex_number[first_part_of_the_number] = real


def set_imaginary_list(complex_number, imaginary):
    second_part_of_the_number = 1
    complex_number[second_part_of_the_number] = imaginary


def complex_to_str_list(complex_number):
    real = get_real_list(complex_number)
    imaginary = get_imaginary_list(complex_number)
    sign = '+' if imaginary >= 0 else ''
    return f"{real} {sign}{imaginary}i"

def create_complex_number_dict(real, imaginary):
    return {'real': real, 'imaginary': imaginary}


def get_real_dict(complex_number):
    return complex_number['real']


def get_imaginary_dict(complex_number):
    return complex_number['imaginary']


def set_real_dict(complex_number, real):
    complex_number['real'] = real


def set_imaginary_dict(complex_number, imaginary):
    complex_number['imaginary'] = imaginary


def complex_to_str_dict(complex_number):
    real = get_real_dict(complex_number)
    imaginary = get_imaginary_dict(complex_number)
    sign = '+' if imaginary >= 0 else ''
    return f"{real} {sign}{imaginary}i"


def read_complex_number():
    real = int(input("Enter the real part: "))
    imaginary = int(input("Enter the imaginary part: "))
    return real, imaginary


def display_complex_numbers(complex_numbers, to_str_func):
    for i, complex_number in enumerate(complex_numbers):
        print(f"{i + 1}. {to_str_func(complex_number)}")

def modulus_list(complex_number):
    real = get_real_list(complex_number)
    imaginary = get_imaginary_list(complex_number)
    return math.sqrt(real**2 + imaginary**2)

def longest_subarray_same_modulus(complex_numbers):
    if not complex_numbers:
        return []

    longest_subarray = []
    current_subarray = [complex_numbers[0]]
    current_modulus = modulus_list(complex_numbers[0])

    for i in range(1, len(complex_numbers)):
        if math.isclose(modulus_list(complex_numbers[i]), current_modulus):
            current_subarray.append(complex_numbers[i])
        else:
            if len(current_subarray) > len(longest_subarray):
                longest_subarray = current_subarray
            current_subarray = [complex_numbers[i]]
            current_modulus = modulus_list(complex_numbers[i])

    if len(current_subarray) > len(longest_subarray):
        longest_subarray = current_subarray

    return longest_subarray

def longest_increasing_subsequence_dynamic(complex_numbers):
    if not complex_numbers:
        return []

    # List to store the end elements of potential subsequences
    lis_end_elements = []
    # List to track the index of each element's predecessor in the LIS
    predecessors = [-1] * len(complex_numbers)
    # List to store the indices of elements in the LIS
    lis_indices = []

    for i in range(len(complex_numbers)):
        modulus = modulus_list(complex_numbers[i])
        pos = bisect_left(lis_end_elements, modulus)

        # If pos is equal to the length of lis_end_elements, append the modulus
        if pos == len(lis_end_elements):
            lis_end_elements.append(modulus)
            lis_indices.append(i)
        else:
            lis_end_elements[pos] = modulus
            lis_indices[pos] = i

        # Update the predecessor index
        if pos > 0:
            predecessors[i] = lis_indices[pos - 1]

    # Reconstruct the longest increasing subsequence
    lis = []
    index = lis_indices[-1]
    while index != -1:
        lis.append(complex_numbers[index])
        index = predecessors[index]

    return lis[::-1]


def user_interface():
    create_complex_number = create_complex_number_list  # or create_complex_number_dict
    get_real = get_real_list  # or get_real_dict
    get_imaginary = get_imaginary_list  # or get_imaginary_dict
    set_real = set_real_list  # or set_real_dict
    set_imaginary = set_imaginary_list  # or set_imaginary_dict
    complex_to_str = complex_to_str_list  # or complex_to_str_dict

    complex_numbers = []

    while True:
        print_menu()
        option = input("Choose an option: ")

        if option == "1":
            while True:
                real, imaginary = read_complex_number()
                complex_number = create_complex_number(real, imaginary)
                complex_numbers.append(complex_number)

                if input("Do you want to add another complex number? (yes/no): ").lower() == "no":
                    break

        elif option == "2":
            if complex_numbers:
                display_complex_numbers(complex_numbers, complex_to_str)
            else:
                print("No complex numbers to display.")

        elif option == "3":
            subarray = longest_subarray_same_modulus(complex_numbers)
            if subarray:
                print("Longest subarray with same modulus:")
                display_complex_numbers(subarray, complex_to_str)
            else:
                print("No subarray found.")

        elif option == "4":
            lis = longest_increasing_subsequence_dynamic(complex_numbers)
            if lis:
                print("Longest increasing subsequence based on modulus:")
                display_complex_numbers(lis, complex_to_str)
            else:
                print("No increasing subsequence found.")

        elif option == "5":
            print("Exiting the program.")
            exit(0)

        else:
            print("Invalid option. Please try again.")


def print_menu():
    print("\nMenu:")
    print("1. Read complex numbers")
    print("2. Display all complex numbers")
    print("3. Display longest subarray with the same modulus")
    print("4. Display longest increasing subsequence based on modulus (Dynamic Implementation)")
    print("5. Exit")


if __name__ == "__main__":
    user_interface()
