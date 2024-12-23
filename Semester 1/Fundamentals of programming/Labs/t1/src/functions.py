


def test_create_coffee():
    assert create_coffee("Espresso", "Italy", 3.5) == ["Espresso", "Italy", 3.5]
    assert create_coffee("Americano", "USA", 4.0) == ["Americano", "USA", 4.0]

def test_add_coffee():
    coffees = []
    add_coffee(coffees, ["Espresso", "Italy", 3.5])
    assert coffees == [["Espresso", "Italy", 3.5]]
    add_coffee(coffees, ["Americano", "USA", 4.0])
    assert coffees == [["Espresso", "Italy", 3.5], ["Americano", "USA", 4.0]]


# create a coffee with the given name, country, and price
def create_coffee(coffee_name, country, price):
    return [coffee_name, country, price]

# add a coffee to the coffees list
def add_coffee(coffees_list, coffee):

    for coffe in coffees_list:
        if coffe[0] == coffee[0]:
            raise ValueError("Name already exists")

    for coffe in coffees_list:
        if int(coffe[2]) <= 0:
            raise ValueError("Price must be greater than 0")
    coffees_list.append(coffee)

# sort the coffees by price in ascending order
def sort_coffees(coffees_list):
    coffees_list.sort(key=lambda x: (x[1], x[2]))

# filter the coffees by country and price
def filter_coffees(coffees_list, country, price):

    if country == "":
        #return all coffees with price less than or equal to the given price
        return [coffee for coffee in coffees_list if coffee[2] <= price]

    if price == "":
        #return all coffees from the given country
        return [coffee for coffee in coffees_list if coffee[1] == country]

    #if there are no coffees matching the given country and price, return msg
    coffes_auxiliary = [coffee for coffee in coffees_list if coffee[1] == country and coffee[2] <= price]
    if len(coffes_auxiliary) == 0:
        return "No such coffees"
    else:
        return coffes_auxiliary

def get_coffee(coffees_list, index):
    return coffees_list[index]

def set_coffee(coffees_list, index, coffee_name, country, price):
    coffees_list[index] = [coffee_name, country, price]

def to_string(coffees_list):
    return "\n".join([f"{index + 1}. {coffee[0]} - {coffee[1]} - {coffee[2]}" for index, coffee in enumerate(coffees_list)])

# delete all coffees from the given country
def delete_coffees_from_country(coffees_list, country):
    coffees_list[:] = [coffee for coffee in coffees_list if coffee[1] != country]


