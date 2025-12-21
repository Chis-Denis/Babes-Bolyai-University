import time

# ---------- GCD Algorithms ----------

def gcd_subtraction(num1, num2):
    """GCD using subtraction method."""
    while num1 != num2:
        if num1 > num2:
            num1 -= num2
        else:
            num2 -= num1
    return num1


def gcd_modulo(num1, num2):
    """GCD using modulo method."""
    while num2 != 0:
        num1, num2 = num2, num1 % num2
    return num1


def gcd_binary(num1, num2):
    """GCD using binary (Stein's) method."""
    if num1 == 0:
        return num2
    if num2 == 0:
        return num1

    power_of_two = 0

    # Remove common factors of 2
    while (num1 % 2 == 0) and (num2 % 2 == 0):
        num1 //= 2
        num2 //= 2
        power_of_two += 1

    # Make num1 odd
    while num1 % 2 == 0:
        num1 //= 2

    while num2 != 0:
        # Remove factors of 2 from num2
        while num2 % 2 == 0:
            num2 //= 2

        # Ensure num1 ≤ num2
        if num1 > num2:
            num1, num2 = num2, num1

        num2 -= num1

    return num1 * (2 ** power_of_two)


# ---------- Timing Helper ----------
def measure_execution_time(gcd_function, number_a, number_b, repetitions=100):
    total_time_ns = 0

    for _ in range(repetitions):
        start_time = time.perf_counter_ns()
        gcd_function(number_a, number_b)
        end_time = time.perf_counter_ns()
        total_time_ns += end_time - start_time

    average_time_ns = total_time_ns / repetitions
    return average_time_ns / 1e6  # convert nanoseconds → milliseconds


# ---------- Predefined Test Pairs ----------
test_pairs = [
    (120, 180), (480, 1800), (2700, 1920), (810, 1530),
    (3910, 2990), (12340, 56780), (98760, 543210),
    (999999, 1111111), (4567899, 1234566), (32123222222, 22222222222)
]
# One large test pair
test_pairs.append((123456789123456789123456789, 987654321987654321987654321))


# ---------- Menu ----------
def display_menu():
    print("\n=== GCD CALCULATOR ===")
    print("1. Run subtraction method manually")
    print("2. Run modulo method manually")
    print("3. Run binary (Stein) method manually")
    print("4. Compare all methods for predefined pairs")
    print("0. Exit")


# ---------- Main Program ----------
while True:
    display_menu()
    user_choice = input("Choose an option: ")

    if user_choice == "0":
        print("Ciao")
        break

    # Manual mode for single pair
    if user_choice in ["1", "2", "3"]:
        number_a = int(input("Enter first number: "))
        number_b = int(input("Enter second number: "))

        gcd_methods = {
            "1": ("Subtraction", gcd_subtraction),
            "2": ("Modulo", gcd_modulo),
            "3": ("Binary", gcd_binary)
        }

        method_name, gcd_function = gcd_methods[user_choice]
        execution_time_ms = measure_execution_time(gcd_function, number_a, number_b)
        gcd_result = gcd_function(number_a, number_b)

        print(f"\n{method_name} Method - GCD = {gcd_result}, Time = {execution_time_ms:.6f} ms")

    # Compare all 3 algorithms for predefined pairs
    elif user_choice == "4":
        print("\nComparing all methods for predefined pairs:\n")
        print(f"{'Test':<5} {'Algorithm':<15} {'Time (ms)':<15} {'GCD Result'}")
        print("-" * 60)

        for index, (number_a, number_b) in enumerate(test_pairs):
            for method_name, gcd_function in [
                ("Subtraction", gcd_subtraction),
                ("Modulo", gcd_modulo),
                ("Binary", gcd_binary)
            ]:
                execution_time_ms = measure_execution_time(gcd_function, number_a, number_b)
                gcd_result = gcd_function(number_a, number_b)
                print(f"{index + 1:<5} {method_name:<15} {execution_time_ms:<15.6f} {gcd_result}")
            print("-" * 60)

    else:
        print("Invalid option. Try again.")