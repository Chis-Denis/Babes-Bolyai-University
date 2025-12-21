import random
import hashlib

ALPHABET = " ABCDEFGHIJKLMNOPQRSTUVWXYZ"
CHAR_TO_VALUE = {char: i for i, char in enumerate(ALPHABET)}
VALUE_TO_CHAR = {i: char for i, char in enumerate(ALPHABET)}


def mod_exp(base, exponent, modulus):
    """Modular exponentiation"""
    return pow(base, exponent, modulus)


def mod_inverse(value, modulus):
    """Compute modular inverse using Fermat's little theorem"""
    return pow(value, modulus - 2, modulus)


def is_prime(n, k=10):
    """Miller-Rabin primality test"""
    if n < 2:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False

    # Write n-1 as d * 2^r
    r = 0
    d = n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    # Test k times
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def generate_prime(min_val, max_val):
    while True:
        p = random.randint(min_val, max_val)
        if is_prime(p):
            return p


def find_generator(p):
    # For simplicity, we'll use a small generator
    # In practice, you'd want to find a primitive root
    for g in range(2, min(p, 100)):
        if pow(g, (p - 1) // 2, p) != 1:
            return g
    return 2  # Fallback


def hash_function(u1, u2, e, p):
    # Combine values and hash
    combined = f"{u1}_{u2}_{e}".encode()
    hash_obj = hashlib.sha256(combined)
    hash_int = int(hash_obj.hexdigest(), 16)
    # Return hash mod (p-1) to keep it in valid range
    return hash_int % (p - 1) if p > 1 else 0


def generate_keys():
    # Generate a large prime p
    p = 30803
    
    # Find generators g1 and g2
    g1 = find_generator(p)
    g2 = find_generator(p)
    # Ensure g2 is different from g1
    while g2 == g1:
        g2 = find_generator(p)
        if g2 == g1:
            g2 = (g1 * 2) % p
            if g2 == 0:
                g2 = 3
    
    # Generate private keys: x1, x2, y1, y2, z
    x1 = random.randint(1, p - 2)
    x2 = random.randint(1, p - 2)
    y1 = random.randint(1, p - 2)
    y2 = random.randint(1, p - 2)
    z = random.randint(1, p - 2)
    
    # Compute public key components
    c = (mod_exp(g1, x1, p) * mod_exp(g2, x2, p)) % p
    d = (mod_exp(g1, y1, p) * mod_exp(g2, y2, p)) % p
    h = mod_exp(g1, z, p)
    
    # Public key: (p, g1, g2, c, d, h)
    public_key = (p, g1, g2, c, d, h)
    
    # Private key: (x1, x2, y1, y2, z)
    private_key = (x1, x2, y1, y2, z, p)
    
    return public_key, private_key


def encrypt_message(plaintext, public_key):
    p, g1, g2, c, d, h = public_key
    plaintext = plaintext.upper()
    
    # Validate plaintext
    if any(ch not in ALPHABET for ch in plaintext):
        raise ValueError("ERROR: Only A-Z and SPACE allowed.")
    
    ciphertext = []
    
    for ch in plaintext:
        # Convert character to value
        m = CHAR_TO_VALUE[ch]
        
        # Choose random r in [1, p-2]
        r = random.randint(1, p - 2)
        
        # Compute u1, u2, e
        u1 = mod_exp(g1, r, p)
        u2 = mod_exp(g2, r, p)
        e = (mod_exp(h, r, p) * m) % p
        
        # Compute hash α = H(u1, u2, e)
        alpha = hash_function(u1, u2, e, p)
        
        # Compute v = c^r * d^(r*α) mod p
        c_r = mod_exp(c, r, p)
        d_ralpha = mod_exp(d, (r * alpha) % (p - 1), p)
        v = (c_r * d_ralpha) % p
        
        # Ciphertext: (u1, u2, e, v)
        ciphertext.append((u1, u2, e, v))
    
    return ciphertext


def decrypt_message(ciphertext, private_key):
    x1, x2, y1, y2, z, p = private_key
    decrypted_text = ""
    
    for u1, u2, e, v in ciphertext:
        # Compute hash α = H(u1, u2, e)
        alpha = hash_function(u1, u2, e, p)
        
        # Verify ciphertext validity: v = u1^(x1 + y1*α) * u2^(x2 + y2*α) mod p
        exp1 = (x1 + y1 * alpha) % (p - 1)
        exp2 = (x2 + y2 * alpha) % (p - 1)
        v_check = (mod_exp(u1, exp1, p) * mod_exp(u2, exp2, p)) % p
        
        if v != v_check:
            # Invalid ciphertext - reject
            raise ValueError("ERROR: Invalid ciphertext detected (integrity check failed).")
        
        # Decrypt: m = e * (u1^z)^(-1) mod p
        u1_z = mod_exp(u1, z, p)
        u1_z_inv = mod_inverse(u1_z, p)
        m = (e * u1_z_inv) % p
        
        # Convert value to character
        decrypted_text += VALUE_TO_CHAR[m]
    
    return decrypted_text


if __name__ == "__main__":

    public_key, private_key = generate_keys()
    p, g1, g2, c, d, h = public_key
    x1, x2, y1, y2, z, p_private = private_key
    
    print("• PUBLIC KEY (RANDOMLY GENERATED)")
    print(f"   p = {p}")
    print(f"   g1 = {g1}")
    print(f"   g2 = {g2}")
    print(f"   c = {c}")
    print(f"   d = {d}")
    print(f"   h = {h}")
    
    print("\n• PRIVATE KEY")
    print(f"   x1 = {x1}")
    print(f"   x2 = {x2}")
    print(f"   y1 = {y1}")
    print(f"   y2 = {y2}")
    print(f"   z = {z}")
    
    # Encrypt given plaintext
    user_input = input("\n• Enter PLAINTEXT (A-Z and SPACE only): ")
    encrypted_data = encrypt_message(user_input, public_key)
    print("\n• Ciphertext (GENERATED):")
    print(f"   {encrypted_data}")
    
    # Decrypt generated ciphertext
    decrypted_text = decrypt_message(encrypted_data, private_key)
    print("\n• Decrypted (GENERATED):")
    print(f"   {decrypted_text}")
    
    print("\n• PLAINTEXT VALIDATION:", end=" ")
    print("OK" if decrypted_text == user_input.upper() else "FAILED")
    
    # Decrypt given ciphertext
    raw_numbers = input("\n• Enter ciphertext numbers: ").strip().split()
    
    try:
        # Convert all to integers
        raw_numbers = list(map(int, raw_numbers))
        
        # Must be multiple of 4 (u1, u2, e, v for each character)
        if len(raw_numbers) % 4 != 0:
            raise ValueError("Number of values must be multiple of 4 (u1, u2, e, v for each character).")
        
        # Convert to list of tuples
        given_ciphertext = []
        for i in range(0, len(raw_numbers), 4):
            u1 = raw_numbers[i]
            u2 = raw_numbers[i + 1]
            e = raw_numbers[i + 2]
            v = raw_numbers[i + 3]
            given_ciphertext.append((u1, u2, e, v))
        
        print("\n• Ciphertext (RECEIVED):")
        print(f"   {given_ciphertext}")
        
        # Decrypt it
        decrypted_given = decrypt_message(given_ciphertext, private_key)
        
        print("\n• Decrypted (GIVEN):")
        print(f"   {decrypted_given}")
        
        print("\n• CIPHERTEXT VALIDATION:",
              "OK" if all(ch in ALPHABET for ch in decrypted_given) else "FAILED")
    
    except Exception as e:
        print("\n ERROR:", e)
        print("Example input:   123 456 789 111 222 333 444 555")
