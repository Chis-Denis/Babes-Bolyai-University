#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Function to calculate GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to check if a number is prime
bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Function for modular exponentiation (a^b mod m)
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Convert character to number (A=1, B=2, ..., Z=26, _=0)
int charToNum(char c) {
    if (c == '_') return 0;
    return c - 'A' + 1;
}

// Convert number to character
char numToChar(int n) {
    if (n == 0) return '_';
    return 'A' + n - 1;
}

int main() {
    // RSA parameters
    int p = 43;
    int q = 83;
    long long n = (long long)p * q;  // n = 3569
    long long phi_n = (long long)(p - 1) * (q - 1);  // φ(n) = 42 * 82 = 3444
    
    // Find the smallest odd prime e such that gcd(e, φ(n)) = 1
    int e = 3;
    while (true) {
        if (isPrime(e) && gcd(e, phi_n) == 1) {
            break;
        }
        // Find next odd prime
        e += 2;
        while (!isPrime(e)) {
            e += 2;
        }
    }
    
    // Output Values section
    cout << "Values:" << endl;
    cout << "n = " << n << endl;
    cout << "φ(n) = " << phi_n << endl;
    cout << "e = " << e << endl;
    cout << endl;
    
    // Plaintext
    string plaintext = "TOKYO_";
    
    // Convert plaintext to numerical blocks of 2 letters
    vector<long long> blocks;
    vector<string> plaintext_blocks;
    
    for (size_t i = 0; i < plaintext.length(); i += 2) {
        int first = charToNum(plaintext[i]);
        int second = (i + 1 < plaintext.length()) ? charToNum(plaintext[i + 1]) : 0;
        long long block = first * 27 + second;
        blocks.push_back(block);
        
        string block_str = "";
        block_str += plaintext[i];
        if (i + 1 < plaintext.length()) {
            block_str += plaintext[i + 1];
        } else {
            block_str += '_';
        }
        plaintext_blocks.push_back(block_str);
    }
    
    // Output Plaintext section
    cout << "Plaintext:" << endl;
    cout << "Blocks of k letters: ";
    for (size_t i = 0; i < plaintext_blocks.size(); i++) {
        cout << plaintext_blocks[i];
        if (i < plaintext_blocks.size() - 1) cout << " ";
    }
    cout << endl;
    
    cout << "Numerical equivalents: ";
    for (size_t i = 0; i < blocks.size(); i++) {
        cout << "b" << (i + 1) << " = " << blocks[i];
        if (i < blocks.size() - 1) cout << ", ";
    }
    cout << endl;
    cout << endl;
    
    // Encrypt each block
    vector<long long> ciphertext_blocks;
    for (size_t i = 0; i < blocks.size(); i++) {
        long long m = blocks[i];
        long long c = modPow(m, e, n);
        ciphertext_blocks.push_back(c);
    }
    
    // Output Encryption section
    cout << "Encryption:" << endl;
    for (size_t i = 0; i < blocks.size(); i++) {
        cout << "c" << (i + 1) << " = b" << (i + 1) << "^e mod n = " 
             << blocks[i] << "^" << e << " mod " << n 
             << " = " << ciphertext_blocks[i] << endl;
    }
    
    // Convert encrypted numbers to 3-letter blocks (base 27)
    vector<string> ciphertext_letter_blocks;
    string ciphertext = "";
    
    for (size_t i = 0; i < ciphertext_blocks.size(); i++) {
        long long c = ciphertext_blocks[i];
        string block_str = "";
        
        // Convert to base 27 (3 digits)
        int digit1 = c / (27 * 27);
        int remainder = c % (27 * 27);
        int digit2 = remainder / 27;
        int digit3 = remainder % 27;
        
        block_str += numToChar(digit1);
        block_str += numToChar(digit2);
        block_str += numToChar(digit3);
        
        ciphertext_letter_blocks.push_back(block_str);
        ciphertext += block_str;
    }
    
    cout << "Blocks of l letters: ";
    for (size_t i = 0; i < ciphertext_letter_blocks.size(); i++) {
        cout << ciphertext_letter_blocks[i];
        if (i < ciphertext_letter_blocks.size() - 1) cout << " ";
    }
    cout << endl;
    
    cout << "Ciphertext: " << ciphertext << endl;
    
    return 0;
}

