import random
import sys

def generate_graph(n, edge_probability=0.3):
    adj = [[0 for _ in range(n)] for _ in range(n)]
    
    for i in range(n - 1):
        adj[i][i + 1] = 1
    
    adj[n - 1][0] = 1
    
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            
            if adj[i][j] == 1:
                continue
            
            if random.random() < edge_probability:
                adj[i][j] = 1
    
    return adj

def print_graph(adj):
    n = len(adj)
    for i in range(n):
        print(' '.join(str(adj[i][j]) for j in range(n)))

def main():
    if len(sys.argv) < 2:
        sys.exit(1)
    
    try:
        n = int(sys.argv[1])
        if n < 1:
            sys.exit(1)
        
        edge_prob = 0.3
        if len(sys.argv) > 2:
            edge_prob = float(sys.argv[2])
            if edge_prob < 0 or edge_prob > 1:
                sys.exit(1)
        
        adj = generate_graph(n, edge_prob)
        print_graph(adj)
        
    except ValueError:
        sys.exit(1)

if __name__ == "__main__":
    main()
