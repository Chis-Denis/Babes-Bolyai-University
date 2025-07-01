import numpy as np

def rastrigin(x, y):
    """
    Rastrigin function (2D):
    A non-convex function used as a performance test problem for optimization algorithms.
    Domain: x, y in [-5.12, 5.12]
    Global minimum at (0,0) with value 0
    The function has many local minima, making it challenging for optimization algorithms.
    """
    A = 10
    return 2 * A + (x ** 2 - A * np.cos(2 * np.pi * x)) + (y ** 2 - A * np.cos(2 * np.pi * y))

def himmelblau(x, y):
    """
    Himmelblau's function (2D):
    A multi-modal function commonly used for testing optimization algorithms.
    Domain: x, y in [-5, 5]
    Four global minima at:
    - (3, 2)
    - (-2.81, 3.13)
    - (-3.78, -3.28)
    - (3.58, -1.85)
    All minima have a function value of 0.
    """
    return (x ** 2 + y - 11) ** 2 + (x + y ** 2 - 7) ** 2

def get_domains():
    """
    Returns the domains for the benchmark functions as dictionaries.
    Each function has its own domain constraints for x and y coordinates.
    Returns:
        dict: A dictionary containing the domain bounds for each function
    """
    return {
        'rastrigin': {'x': (-5.12, 5.12), 'y': (-5.12, 5.12)},
        'himmelblau': {'x': (-5, 5), 'y': (-5, 5)}
    } 