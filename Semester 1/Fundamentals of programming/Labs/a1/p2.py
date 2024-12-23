# The palindrome of a number is the number obtained by reversing the order of its digits (e.g. the palindrome of 237 is 732). For a given natural number n, determine its palindrome.

def print_user_interface():
    print("0. Exit \n"
          "1. Enter a number")

def find_palindrome(number_to_check):
    return int(str(number_to_check)[::-1])

while (True):

    print_user_interface()
    number_input = int(input())

    if number_input == 0:
        break

    elif number_input == 1:
        number_to_check = int(input("Enter a number: "))
        print(find_palindrome(number_to_check))

