class FiniteAutomaton:
    """Represents a Finite Automaton"""
    
    def __init__(self, states, alphabet, transitions, initial_state, final_states):
        self.Q = states  # Set of states
        self.Sigma = alphabet  # Alphabet
        self.delta = transitions  # Transition function: dict[state, char] -> set of states
        self.q0 = initial_state  # Initial state
        self.F = final_states  # Set of final states
    
    def is_final_state(self, state):
        """Check if a state is final"""
        return state in self.F
    
    def accepts(self, string):
        """
        Check if the FA accepts the given string
        Returns True if the string is accepted, False otherwise
        """
        current_state = self.q0
        
        for char in string:
            if (current_state, char) in self.delta:
                current_state = self.delta[(current_state, char)]
            else:
                return False  # No transition defined
        
        return self.is_final_state(current_state)


def rg_to_fa_identifier():
    """
    Construct Finite Automaton for identifiers using Theorem 1
    
    Regular Grammar (right-linear form):
    P = {
        S -> aA | bA | ... | zA | A_ | B_ | ... | Z_ | a | b | ... | z | _
        A -> aA | bA | ... | zA | 0A | ... | 9A | _A | a | b | ... | z | 0 | ... | 9 | _
    }
    
    Where S is start symbol, A is non-terminal, letters/digits are terminals.
    
    According to Theorem 1:
    - Q = N ∪ {K} where N = {S, A}, K = "F" (final state)
    - q0 = S
    - F = {K} ∪ {S if S -> ε ∈ P} = {K, S} (since S can produce single char)
    
    Transition rules:
    - if S -> letter A ∈ P then δ(S, letter) = A
    - if S -> _A ∈ P then δ(S, _) = A
    - if S -> letter ∈ P then δ(S, letter) = K
    - if A -> letter A ∈ P then δ(A, letter) = A
    - if A -> digit A ∈ P then δ(A, digit) = A
    - if A -> _A ∈ P then δ(A, _) = A
    - if A -> letter ∈ P then δ(A, letter) = K
    - if A -> digit ∈ P then δ(A, digit) = K
    - if A -> _ ∈ P then δ(A, _) = K
    """
    
    # Build alphabet
    letters = [chr(i) for i in range(ord('a'), ord('z')+1)] + \
              [chr(i) for i in range(ord('A'), ord('Z')+1)]
    digits = [chr(i) for i in range(ord('0'), ord('9')+1)]
    underscore = "_"
    
    # States: {S, A, K} where K is final state
    Q = {"S", "A", "K"}
    alphabet = set(letters + digits + [underscore])
    delta = {}
    
    # From state S:
    # S -> letter A: δ(S, letter) = A (for all letters)
    for letter in letters:
        delta[("S", letter)] = "A"
        # Also: S -> letter: δ(S, letter) = K (terminal production)
        # Since we need non-deterministic behavior, we'll make A the final state
        # and accept when we're in A without further input
    
    # S -> _A: δ(S, _) = A
    delta[("S", underscore)] = "A"
    
    # From state A:
    # A -> letter A: δ(A, letter) = A
    for letter in letters:
        delta[("A", letter)] = "A"
    
    # A -> digit A: δ(A, digit) = A
    for digit in digits:
        delta[("A", digit)] = "A"
    
    # A -> _A: δ(A, _) = A
    delta[("A", underscore)] = "A"
    
    initial_state = "S"
    final_states = {"A"}  # A is the final state (can accept here)
    
    return FiniteAutomaton(Q, alphabet, delta, initial_state, final_states)


def rg_to_fa_integer():
    """
    Construct Finite Automaton for integer constants using Theorem 1
    
    From Lab 1 BNF:
    number ::= digit number_tail | digit
    number_tail ::= digit number_tail | epsilon
    
    Regular Grammar productions:
    S -> 0A | 1A | ... | 9A | 0 | 1 | ... | 9
    A -> 0A | 1A | ... | 9A | 0 | 1 | ... | 9
    
    Where:
    digit = [0-9]
    """
    
    # States: S (start/initial), A (tail state), K (final state)
    Q = {"S", "A", "K"}
    
    # Alphabet: digits
    digits = [chr(i) for i in range(ord('0'), ord('9')+1)]
    alphabet = set(digits)
    
    # Initialize transition function
    delta = {}
    
    # Production: S -> digit A (non-terminal production)
    for digit in digits:
        delta[("S", digit)] = "A"
    
    # State A transitions: A -> digit A
    for digit in digits:
        delta[("A", digit)] = "A"
    
    initial_state = "S"
    final_states = {"A"}  # A is final (can accept any number of digits)
    
    return FiniteAutomaton(Q, alphabet, delta, initial_state, final_states)


def rg_to_fa_string():
    """
    Construct Finite Automaton for string constants using Theorem 1
    
    From Lab 1 BNF:
    string ::= '"' char_seq '"'
    char_seq ::= char char_seq | epsilon
    char ::= letter | digit | symbol
    symbol ::= "+" | "-" | "*" | "/" | " " | "_" | "@" | "%" | "^"
    
    Extended to include ":" for practical usage (used in examples).
    
    Regular Grammar (right-linear form):
    S -> "A
    A -> char A | "K
    
    Where:
    - S is start symbol
    - A is state for reading characters inside quotes
    - K is final state (after closing quote)
    - char can be letter, digit, or symbol (including ":")
    """
    
    # Build valid characters that can appear inside strings
    letters = [chr(i) for i in range(ord('a'), ord('z')+1)] + \
              [chr(i) for i in range(ord('A'), ord('Z')+1)]
    digits = [chr(i) for i in range(ord('0'), ord('9')+1)]
    symbols = ["+", "-", "*", "/", " ", "_", "@", "%", "^", ":"]
    valid_chars = letters + digits + symbols
    
    # States: S (start), A (reading content), K (final after closing quote)
    Q = {"S", "A", "K"}
    
    # Alphabet: all valid chars + double quote
    alphabet = set(valid_chars + ['"'])
    
    # Initialize transition function
    delta = {}
    
    # Production: S -> "A (read opening quote, go to state A)
    delta[("S", '"')] = "A"
    
    # Production: A -> char A (read any valid char, stay in A)
    for char in valid_chars:
        delta[("A", char)] = "A"
    
    # Production: A -> "K (read closing quote, go to final state K)
    delta[("A", '"')] = "K"
    
    initial_state = "S"
    final_states = {"K"}  # K is final (string properly closed)
    
    return FiniteAutomaton(Q, alphabet, delta, initial_state, final_states)


def rg_to_fa_char_literal():
    """
    Construct Finite Automaton for char literals using Theorem 1 from course
    
    From Lab 1 BNF:
    char_literal ::= "'" char "'"
    char ::= letter | digit | symbol
    symbol ::= "+" | "-" | "*" | "/" | " " | "_" | "@" | "%" | "^"
    
    Regular Grammar (right-linear form):
    S -> 'A
    A -> char B
    B -> 'K
    
    Where:
    - S is start symbol
    - A is state after opening quote
    - B is state after reading the character
    - K is final state (after closing quote)
    - char can be letter, digit, or symbol
    """
    
    # Build valid characters that can appear in char literals
    letters = [chr(i) for i in range(ord('a'), ord('z')+1)] + \
              [chr(i) for i in range(ord('A'), ord('Z')+1)]
    digits = [chr(i) for i in range(ord('0'), ord('9')+1)]
    symbols = ["+", "-", "*", "/", " ", "_", "@", "%", "^"]
    valid_chars = letters + digits + symbols
    
    # States: S (start), A (after opening quote), B (after char), K (final)
    Q = {"S", "A", "B", "K"}
    
    # Alphabet: all valid chars + single quote
    alphabet = set(valid_chars + ["'"])
    
    # Initialize transition function
    delta = {}
    
    # Production: S -> 'A (read opening single quote, go to state A)
    delta[("S", "'")] = "A"
    
    # Production: A -> char B (read exactly one valid char, go to state B)
    for char in valid_chars:
        delta[("A", char)] = "B"
    
    # Production: B -> 'K (read closing single quote, go to final state K)
    delta[("B", "'")] = "K"
    
    initial_state = "S"
    final_states = {"K"}  # K is final (char literal properly closed)
    
    return FiniteAutomaton(Q, alphabet, delta, initial_state, final_states)


def test_automata():
    """Test the automata with various inputs"""
    
    print("=" * 60)
    print("Testing Identifier Automaton")
    print("=" * 60)
    
    fa_id = rg_to_fa_identifier()
    
    test_cases = [
        "x",
        "variable",
        "_underscore",
        "var123",
        "camelCase",
        "snake_case",
        "UPPERCASE",
        "v123",
        "0invalid",  # Should fail (starts with digit)
        "var-able",  # Should fail (has dash)
        "var able",  # Should fail (has space)
        "x_y_z",
        "a1b2c3",
    ]
    
    for test in test_cases:
        result = fa_id.accepts(test)
        status = "ACCEPT" if result else "REJECT"
        marker = "[OK]" if result else "[FAIL]"
        print(f"{marker:8} {status:8} '{test}'")
    
    print("\n" + "=" * 60)
    print("Testing Integer Constant Automaton")
    print("=" * 60)
    
    fa_int = rg_to_fa_integer()
    
    test_cases = [
        "0",
        "123",
        "999",
        "5",
        "0102",
        "abc",  # Should fail (not digits)
        "12a",  # Should fail (has letter)
        "12.5",  # Should fail (has dot)
        "",      # Should fail (empty string)
    ]
    
    for test in test_cases:
        result = fa_int.accepts(test)
        status = "ACCEPT" if result else "REJECT"
        marker = "[OK]" if result else "[FAIL]"
        print(f"{marker:8} {status:8} '{test}'")
    
    print("\n" + "=" * 60)
    print("Testing String Constant Automaton")
    print("=" * 60)
    
    fa_str = rg_to_fa_string()
    
    test_cases = [
        '""',           # Empty string - should accept
        '"hello"',      # Simple string
        '"Hello World"', # String with space
        '"a1b2c3"',     # String with letters and digits
        '"x+y"',        # String with symbols
        '"A"',          # Single character string
        '"test"',        # Valid string
        '"GCD:"',       # String with colon - should accept
        '"text: value"', # String with colon and space
        'hello',         # Should fail (no quotes)
        '"unclosed',    # Should fail (missing closing quote)
        'unclosed"',    # Should fail (missing opening quote)
        '"a"b"',        # Should fail (extra quote)
        '',             # Should fail (empty)
    ]
    
    for test in test_cases:
        result = fa_str.accepts(test)
        status = "ACCEPT" if result else "REJECT"
        marker = "[OK]" if result else "[FAIL]"
        print(f"{marker:8} {status:8} '{test}'")
    
    print("\n" + "=" * 60)
    print("Testing Char Literal Automaton")
    print("=" * 60)
    
    fa_char = rg_to_fa_char_literal()
    
    test_cases = [
        "'a'",          # Letter char
        "'5'",          # Digit char
        "'+'",          # Symbol char
        "' '",          # Space char
        "'Z'",          # Uppercase letter
        "'x'",          # Lowercase letter
        "'@'",          # Symbol char
        "a",            # Should fail (no quotes)
        "'ab'",         # Should fail (two chars)
        "'unclosed",    # Should fail (missing closing quote)
        "unclosed'",    # Should fail (missing opening quote)
        "''",           # Should fail (empty char literal)
        '',             # Should fail (empty)
    ]
    
    for test in test_cases:
        result = fa_char.accepts(test)
        status = "ACCEPT" if result else "REJECT"
        marker = "[OK]" if result else "[FAIL]"
        print(f"{marker:8} {status:8} '{test}'")


if __name__ == "__main__":
    test_automata()

