import random

def is_composite(a, d, n, s):
    # check if a proves n is composite
    x = pow(a, d, n)
    if x == 1 or x == n - 1:
        return False

    for _ in range(s - 1):
        x = pow(x, 2, n)
        if x == n - 1: #if reach -1 -> n-prime
            return False

    return True


def miller_rabin(n, k=10):
    # basic cases
    if n < 2:
        return False
    if n in (2, 3):
        return True
    if n % 2 == 0:
        return False

    #n-1 = 2^s * d  (d odd)
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2 #divide until odd
        s += 1 #count of divisions

    #repeat test k times
    for _ in range(k):
        a = random.randrange(2, n - 1) #choose a random base
        if is_composite(a, d, n, s):
            return False #if one a proves composite the number is composite

    return True


# Example usage
if __name__ == "__main__":
    nums = [17, 20, 561, 1105, 7919]
    for x in nums:
        print(x, "=", "prime" if miller_rabin(x) else "composite")

#3