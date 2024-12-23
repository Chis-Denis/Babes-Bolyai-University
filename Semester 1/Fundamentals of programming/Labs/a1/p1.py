#Generate the first prime number larger than a given natural number n.

def print_user_interface():
    print("0. Exit \n"
          "1. Enter a number")


def first_prime_number_larger(number_to_check):
    if number_to_check < 2:
        return 2

    while True:

        number_to_check += 1

        for index in range(2, number_to_check):
            if number_to_check % index == 0:
                break
        else:
            return number_to_check


while (True):

    print_user_interface()
    number_input = int(input())

    if number_input == 0:
        break

    elif number_input == 1:
        number_to_check = int(input("Enter a number: "))
        print(first_prime_number_larger(number_to_check))