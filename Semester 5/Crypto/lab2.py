def encrypt(message, key):
    block_size = len(key)
    ciphertext = ""

    while len(message) % block_size != 0:
        message += "X"  # pad if needed

    for i in range(0, len(message), block_size):
        block = message[i:i + block_size]
        new_block = ""
        for index in key:
            new_block += block[index - 1]  # apply permutation
        ciphertext += new_block

    return ciphertext


def decrypt(ciphertext, key):
    block_size = len(key)
    message = ""

    inverse_key = [0] * block_size
    for i, key_pos in enumerate(key):
        inverse_key[key_pos - 1] = i + 1  # build reverse order

    for i in range(0, len(ciphertext), block_size):
        block = ciphertext[i:i + block_size]
        new_block = ""
        for index in inverse_key:
            new_block += block[index - 1]  # reverse permutation
        message += new_block

    return message


# Example
key = [3, 2, 4, 1]
message = "HELLO WORLD".replace(" ", "")  # remove spaces

encrypted = encrypt(message, key)
decrypted = decrypt(encrypted, key)

print("Encrypted:", encrypted)
print("Decrypted:", decrypted)

#1