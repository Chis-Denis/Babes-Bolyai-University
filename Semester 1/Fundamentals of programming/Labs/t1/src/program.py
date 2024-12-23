import functions


def print_menu():
    print("1. Add a coffee")
    print("2. View coffees")
    print("3. View sorted coffees")
    print("4. View filtered coffees by country and price")
    print("5. Delete all coffees from a country")
    print("4. Exit")

def user_interface(coffees):

    coffees_list = coffees

    while True:
        print()
        print_menu()

        user_input = input("Please enter a number: ")

        if user_input == "1":
            print()
            try:
                coffee_name = input("Enter the coffee name: ")
                country = input("Enter the country: ")
                price = input("Enter the price: ")

                coffee = functions.create_coffee(coffee_name, country, price)
                functions.add_coffee(coffees_list, coffee)

            except ValueError as ve:
                print(ve)

        elif user_input == "2":
            print()
            print(functions.to_string(coffees_list))

        elif user_input == "3":
            print()
            functions.sort_coffees(coffees_list)
            print(functions.to_string(coffees_list))

        elif user_input == "4":
            print()
            country = input("Enter the country: ")
            price = input("Enter the price: ")
            print(functions.filter_coffees(coffees_list, country, price))

        elif user_input == "5":
            print()
            functions.delete_coffees_from_country(coffees_list, input("Enter the country: "))

        elif user_input == "6":
            exit(0)


def program_start():

    coffees = [
        ["Caffe mi-somn", "France", "5"],
        ["Caffe vreau-sa-dorm", "France", "4"],
        ["Caffe Irish", "Irland", "3"]
    ]

    functions.test_add_coffee()
    functions.test_create_coffee()

    user_interface(coffees)

if __name__ == "__main__":
    program_start()