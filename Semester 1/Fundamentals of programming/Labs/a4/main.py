
#backtracking problem A number of n coins are given, with values of a1, ..., an and a value s. Display all payment modalities for the sum s. If no payment modality exists print a message.
#dynamic programming Given a set of integers A, determine if it can be partitioned into two subsets with equal sum. For example, set A = { 1, 1, 1, 1, 2, 3, 5 } can be partitioned into sets A1 = { 1, 1, 2, 3 } and A2 = { 1, 1, 5 }, each of them having sum 7. Display one such possibility.

def display_all_payment_modalities_for_a_sum_iterative(number_of_coins, list_of_coins, sum_target):

    for index in range(1 << number_of_coins):
        sum = 0
        for coin in range(number_of_coins):
            if index & (1 << coin):
                sum += list_of_coins[coin]
        if sum == sum_target:
            print("Payment modality: ", end = "")
            for coin in range(number_of_coins):
                if index & (1 << coin):
                    print(list_of_coins[coin], end = " ")
            print()

def display_all_payment_modalities_for_a_sum_recursive(number_of_coins, list_of_coins, sum_target):

    def display_all_payment_modalities_for_a_sum_utility(coins, sum_target, current_sum, current_index, current_solution):

        if current_sum == sum_target:
            print("Payment modality: ", end = "")
            for coin in current_solution:
                print(coin, end = " ")
            print()
            return

        if current_index == len(coins) or current_sum > sum_target:
            return

        current_solution.append(coins[current_index])
        display_all_payment_modalities_for_a_sum_utility(coins, sum_target, current_sum + coins[current_index], current_index + 1, current_solution)
        current_solution.pop()
        display_all_payment_modalities_for_a_sum_utility(coins, sum_target, current_sum, current_index + 1, current_solution)

    display_all_payment_modalities_for_a_sum_utility(list_of_coins, sum_target, 0, 0, [])

def display_a_partition_of_the_same_sum_from_a_given_list_naive(set_to_be_partitioned):

    def display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index, current_sum, current_solution, solution):

        if current_sum == sum(set_to_be_partitioned) // 2:
            solution.append(current_solution[:])
            return

        if current_index == len(set_to_be_partitioned) or current_sum > sum(set_to_be_partitioned) // 2:
            return

        current_solution.append(set_to_be_partitioned[current_index])
        display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index + 1, current_sum + set_to_be_partitioned[current_index], current_solution, solution)
        current_solution.pop()
        display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index + 1, current_sum, current_solution, solution)

    solution = []
    display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, 0, 0, [], solution)

    if len(solution) == 0:
        print("No such partition exists.")
    else:
        print("Partition: ", end = "")
        for element in solution[0]:
            print(element, end = " ")
        print()

def display_a_partition_of_the_same_sum_from_a_given_list_optimized(set_to_be_partitioned):

    def display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index, current_sum, current_solution, solution, dictionary_solutions):

        if current_sum == sum(set_to_be_partitioned) // 2:
            solution.append(current_solution[:])
            return

        if current_index == len(set_to_be_partitioned) or current_sum > sum(set_to_be_partitioned) // 2:
            return

        if dictionary_solutions[current_index][current_sum] == 0:
            return

        current_solution.append(set_to_be_partitioned[current_index])
        display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index + 1, current_sum + set_to_be_partitioned[current_index], current_solution, solution, dictionary_solutions)
        current_solution.pop()
        display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, current_index + 1, current_sum, current_solution, solution, dictionary_solutions)

    dictionary_solutions = [[1 for _ in range(sum(set_to_be_partitioned) // 2 + 1)] for _ in range(len(set_to_be_partitioned))]
    solution = []
    display_a_partition_of_the_same_sum_from_a_given_list_utility(set_to_be_partitioned, 0, 0, [], solution, dictionary_solutions)

    if len(solution) == 0:
        print("No such partition exists.")
    else:
        print("Partition: ", end = "")
        for element in solution[0]:
            print(element, end = " ")
        print()

def user_interface():

    print("1. Backtracking iterative\n"
          "2. Backtracking recursive\n"
          "3. Dynamic programming naive\n"
          "4. Dynamic programming non-optimized\n"
          "5. Dynamic programming optimized\n"
          "6. Exit\n")

    input_choice = input("Enter your choice: ")

    if input_choice == "1":

        number_of_coins = int(input("Enter the number of coins: "))
        list_of_coins = list(map(int, input("Enter the values of the coins: ").split()))
        sum_target = int(input("Enter the sum target: "))
        display_all_payment_modalities_for_a_sum_iterative(number_of_coins, list_of_coins, sum_target)

    elif input_choice == "2":

        number_of_coins = int(input("Enter the number of coins: "))
        list_of_coins = list(map(int, input("Enter the values of the coins: ").split()))
        sum_target = int(input("Enter the sum target: "))
        display_all_payment_modalities_for_a_sum_recursive(number_of_coins, list_of_coins, sum_target)

    elif input_choice == "3":

        set_to_be_partitioned = list(map(int, input("Enter the set to be partitioned: ").split()))
        display_a_partition_of_the_same_sum_from_a_given_list_naive(set_to_be_partitioned)

    elif input_choice == "4":

        pass

    elif input_choice == "5":

        set_to_be_partitioned = list(map(int, input("Enter the set to be partitioned: ").split()))
        display_a_partition_of_the_same_sum_from_a_given_list_optimized(set_to_be_partitioned)

    elif input_choice == "6":
        exit(0)

if __name__ == "__main__":
    user_interface()
