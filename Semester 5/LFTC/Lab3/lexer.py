import re

# Load known tokens (keywords, operators, delimiters)
with open("tokens.txt") as f:
    TOKENS = [t.strip() for t in f.readlines() if t.strip()]

# Define regex patterns for identifiers and constants
patterns = {
    "identifier": r"^[a-zA-Z_][a-zA-Z0-9_]*$",
    "int_const": r"^[0-9]+$",
    "str_const": r'^"[^"\n]*"$',
    "char_const": r"^'.'$"
}

# Symbol table (simple Python dicts)
symbol_table = {"identifiers": {}, "constants": {}}
pif = []
errors = []


def add_to_st(category, value):
    """Adds a symbol to the symbol table and returns its index."""
    table = symbol_table[category]
    if value not in table:
        table[value] = len(table)
    return table[value]


def classify_token(token, line_number):
    """Checks the token type and adds it to PIF or errors."""
    # Check for invalid identifier: starts with digit but contains letters
    if re.match(r'^[0-9]+[a-zA-Z_]', token):
        errors.append(f"Lexical error at line {line_number}: Invalid identifier '{token}' (cannot start with a digit)")
        return
    
    if token in TOKENS:
        pif.append((token, -1))
    elif re.match(patterns["identifier"], token):
        idx = add_to_st("identifiers", token)
        pif.append(("id", idx))
    elif (re.match(patterns["int_const"], token) or
          re.match(patterns["str_const"], token) or
          re.match(patterns["char_const"], token)):
        idx = add_to_st("constants", token)
        pif.append(("const", idx))
    else:
        errors.append(f"Lexical error at line {line_number}: {token}")


# Tokenize source program
with open("program.txt") as src:
    print("--- Reading program ---")
    for line_number, line in enumerate(src, start=1):
        print(f"Line {line_number}: {line.strip()}")

        # Add spaces around parentheses, brackets, commas (optional, for clarity)
        line = (line.replace("(", " ( ").replace(")", " ) ")
                    .replace("[", " [ ").replace("]", " ] ")
                    .replace(",", " , "))

        # Regex-based tokenizer to handle strings and custom operators
        # Note: pattern for invalid identifiers (starting with digit) comes before valid number pattern
        tokens = re.findall(
            r'"[^"\n]*"|\'[^\']*\'|@@|%%|\^\^|[0-9]+[a-zA-Z_]|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[><=]+|[^\s]',
            line
        )

        for token in tokens:
            classify_token(token.strip(), line_number)


# Write outputs
with open("PIF.txt", "w") as f:
    for entry in pif:
        f.write(str(entry) + "\n")

with open("ST.txt", "w") as f:
    f.write("Identifiers:\n")
    for k, v in symbol_table["identifiers"].items():
        f.write(f"{v} -> {k}\n")
    f.write("\nConstants:\n")
    for k, v in symbol_table["constants"].items():
        f.write(f"{v} -> {k}\n")

with open("Errors.txt", "w") as f:
    for e in errors:
        f.write(e + "\n")

print("Lexical analysis complete!")
print(f"PIF entries: {len(pif)} | Errors: {len(errors)}")
