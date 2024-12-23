# Generate the largest perfect number smaller than a given natural number n. If such a number does not exist, a message should be displayed. A number is perfect if it is equal to the sum of its divisors, except itself. (e.g. 6 is a perfect number, as 6=1+2+3).

def print_user_interface():
    print("0. Exit \n"
          "1. Enter a number")

def is_perfect_number(number_to_check):
    sum_of_divisors = 0

    for index in range(1, number_to_check):
        if number_to_check % index == 0:
            sum_of_divisors += index

    return sum_of_divisors == number_to_check

def largest_perfect_number_smaller(number_to_check):

    for index in range(number_to_check - 1, 0, -1):
        if is_perfect_number(index):
            return index

    return "No perfect number found"

while (True):
    print_user_interface()
    number_input = int(input())

    if number_input == 0:
        break

    elif number_input == 1:
        number_to_check = int(input("Enter a number: "))
        print(largest_perfect_number_smaller(number_to_check))