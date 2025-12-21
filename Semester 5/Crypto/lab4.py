import random

ALPHABET = " ABCDEFGHIJKLMNOPQRSTUVWXYZ"
CHAR_TO_VALUE = {char: i for i, char in enumerate(ALPHABET)}
VALUE_TO_CHAR = {i: char for i, char in enumerate(ALPHABET)}

def mod_exp(base, exponent, modulus):
    return pow(base, exponent, modulus)

def mod_inverse(value, modulus):
    return pow(value, modulus - 2, modulus)

def generate_keys():
    prime_modulus = 30803
    generator = random.randint(2, prime_modulus - 2)
    private_key = random.randint(2, prime_modulus - 2)
    public_value = mod_exp(generator, private_key, prime_modulus)
    public_key = (prime_modulus, generator, public_value)
    return public_key, private_key

def encrypt_message(plaintext, public_key):
    p, g, y = public_key
    plaintext = plaintext.upper()

    if any(ch not in ALPHABET for ch in plaintext):
        raise ValueError("ERROR: Only A-Z and SPACE allowed.")

    ciphertext = []
    for ch in plaintext:
        m = CHAR_TO_VALUE[ch]
        session_key = random.randint(2, p - 2)
        a = mod_exp(g, session_key, p)
        b = (m * mod_exp(y, session_key, p)) % p
        ciphertext.append((a, b))

    return ciphertext

def decrypt_message(ciphertext, private_key, prime_modulus):
    decrypted_text = ""
    for a, b in ciphertext:
        shared_secret = mod_exp(a, private_key, prime_modulus)
        m = (b * mod_inverse(shared_secret, prime_modulus)) % prime_modulus
        decrypted_text += VALUE_TO_CHAR[m]
    return decrypted_text

if __name__ == "__main__":

    public_key, private_key = generate_keys()
    p, g, y = public_key

    print("• PUBLIC KEY (RANDOMLY GENERATED)")
    print(f"   p = {p}")
    print(f"   g = {g}")
    print(f"   y = {y}")

    print("\n• PRIVATE KEY")
    print(f"   x = {private_key}")

    # Encrypt given plaintext
    user_input = input("\n• Enter PLAINTEXT (A-Z and SPACE only): ")
    encrypted_data = encrypt_message(user_input, public_key)
    print("\n• Ciphertext (GENERATED):")
    print(f"   {encrypted_data}")

    # Decrypt generated ciphertext
    decrypted_text = decrypt_message(encrypted_data, private_key, p)
    print("\n• Decrypted (GENERATED):")
    print(f"   {decrypted_text}")

    print("\n• PLAINTEXT VALIDATION:", end=" ")
    print("OK" if decrypted_text == user_input.upper() else "FAILED")


    raw_numbers = input("\n• Enter ciphertext numbers: ").strip().split()

    try:
        # Convert all to integers
        raw_numbers = list(map(int, raw_numbers))

        # Must be even number of elements
        if len(raw_numbers) % 2 != 0:
            raise ValueError("Number of values must be EVEN (pairs of a,b).")

        # Convert to list of tuples
        given_ciphertext = []
        for i in range(0, len(raw_numbers), 2):
            a = raw_numbers[i]
            b = raw_numbers[i + 1]
            given_ciphertext.append((a, b))

        print("\n• Ciphertext (RECEIVED):")
        print(f"   {given_ciphertext}")

        # Decrypt it
        decrypted_given = decrypt_message(given_ciphertext, private_key, p)

        print("\n• Decrypted (GIVEN):")
        print(f"   {decrypted_given}")

        print("\n• CIPHERTEXT VALIDATION:",
              "OK" if all(ch in ALPHABET for ch in decrypted_given) else "FAILED")

    except Exception as e:
        print("\n ERROR:", e)
        print("Example input:   123 456 789 111 555 999")

