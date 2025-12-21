from collections import defaultdict, deque

# --------- I/O helpers ---------
def load_fa(
    states_file="states.txt",
    alphabet_file="alphabet.txt",
    transitions_file="transitions.txt",
    start_final_file="start_final.txt",
):
    with open(states_file) as f:
        states = f.read().split()
    with open(alphabet_file) as f:
        alphabet = f.read().split()

    delta = defaultdict(set)  # (state, symbol) -> {next_states}
    with open(transitions_file) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            p, a, q = line.split()
            delta[(p, a)].add(q)

    with open(start_final_file) as f:
        lines = [ln.strip() for ln in f if ln.strip()]
    start = lines[0].split()[0]                 # first token on line 1
    finals = lines[1].split() if len(lines) > 1 else []  # line 2 (space-separated)

    return {
        "Q": set(states),
        "Sigma": set(alphabet),
        "delta": delta,
        "q0": start,
        "F": set(finals),
    }

def show_fa(fa):
    print("\n=== Finite Automaton ===")
    print(f"Q      = {sorted(fa['Q'])}")
    print(f"Sigma  = {sorted(fa['Sigma'])}")
    print(f"q0     = {fa['q0']}")
    print(f"F      = {sorted(fa['F'])}")
    print("delta  :")
    items = sorted(fa["delta"].items(), key=lambda kv: (kv[0][0], kv[0][1]))
    for (p, a), nexts in items:
        print(f"  δ({p}, {a}) = {sorted(nexts)}")
    print()

# --------- Acceptance (NFA, no epsilon) ---------
def fa_accepts(fa, word):
    # reject if word has symbols not in alphabet
    for ch in word:
        if ch not in fa["Sigma"]:
            return False
    current = set([fa["q0"]])
    for ch in word:
        nxt = set()
        for state in current:
            nxt |= fa["delta"].get((state, ch), set())
        current = nxt
        if not current:
            return False
    return any(s in fa["F"] for s in current)

# --------- FA -> Regular Grammar (right-linear) ---------
def fa_to_rg(fa):
    N = set(fa["Q"])
    Sigma = set(fa["Sigma"])
    S = fa["q0"]
    P = defaultdict(set)  # nonterminal -> set of RHS strings like "aQ" or "a" or "ε"

    for (p, a), nexts in fa["delta"].items():
        for q in nexts:
            P[p].add(f"{a}{q}")     # p -> a q
            if q in fa["F"]:
                P[p].add(f"{a}")    # p -> a

    if S in fa["F"]:
        P[S].add("ε")               # S -> ε

    return {"N": N, "Sigma": Sigma, "P": P, "S": S}

def show_rg(G):
    print("\n=== Regular Grammar (right-linear) ===")
    print(f"N     = {sorted(G['N'])}")
    print(f"Σ     = {sorted(G['Sigma'])}")
    print(f"S     = {G['S']}")
    print("P     :")
    for A in sorted(G["N"]):
        rhss = sorted(G["P"].get(A, []))
        if rhss:
            print(f"  {A} -> " + " | ".join(rhss))
    print()

# --------- Menu ---------
def main():
    fa = None
    grammar = None
    while True:
        print("MENU")
        print("1. Load FA from files")
        print("2. Show FA")
        print("3. Convert FA -> Regular Grammar and show")
        print("4. Test if a word is accepted by FA")
        print("5. Exit")
        choice = input("Select: ").strip()

        if choice == "1":
            try:
                fa = load_fa()
                grammar = None
                print("OK: FA loaded.\n")
            except Exception as e:
                print(f"Error loading FA: {e}\n")

        elif choice == "2":
            if not fa:
                print("Load FA first (option 1).\n")
            else:
                show_fa(fa)

        elif choice == "3":
            if not fa:
                print("Load FA first (option 1).\n")
            else:
                grammar = fa_to_rg(fa)
                show_rg(grammar)

        elif choice == "4":
            if not fa:
                print("Load FA first (option 1).\n")
            else:
                w = input("Word: ")
                print("ACCEPTED\n" if fa_accepts(fa, w) else "REJECTED\n")

        elif choice == "5":
            print("Bye.")
            break
        else:
            print("Invalid option.\n")

if __name__ == "__main__":
    main()
