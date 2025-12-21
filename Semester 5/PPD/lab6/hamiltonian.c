#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

// Graph structure containing adjacency matrix and shared search state
typedef struct {
    int **adj;              // Adjacency matrix
    int n;                  // Number of vertices
    int *path;              // Current Hamiltonian path found
    bool found;             // Flag indicating if a cycle was found
    pthread_mutex_t found_mutex;  // Mutex
} Graph;

// Data structure passed to each thread for parallel search
typedef struct {
    Graph *graph;           // Shared graph structure
    int *path;              // Path to explore in this thread
    int pos;                // Current position in the path
    int threads;            // Number of threads available for this branch
} ThreadData;

// Prints the Hamiltonian cycle
void print_path(int *path, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", path[i]);
    }
    printf("%d\n", path[0]);  // Close the cycle
}

// Checks if vertex v can be added to the path at position pos
bool is_safe(Graph *g, int *path, int pos, int v) {
    // Check if there's an edge from the previous vertex to v
    if (g->adj[path[pos-1]][v] == 0) return false;
    // Check if v is already in the path
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) return false;
    }
    return true;
}

void* search_thread(void *arg);

// Recursive function to find Hamiltonian cycle using parallel threads
bool hamiltonian_recursive(Graph *g, int *path, int pos, int threads) {
    // Early exit if another thread already found a solution
    if (g->found) return true;
    
    // Base case: path is complete, check if it forms a cycle
    if (pos == g->n) {
        // Check if last vertex connects back to the first
        if (g->adj[path[pos-1]][path[0]]) {
            // Thread-safe update of the found cycle
            pthread_mutex_lock(&g->found_mutex);
            if (!g->found) {
                g->found = true;
                memcpy(g->path, path, g->n * sizeof(int));
            }
            pthread_mutex_unlock(&g->found_mutex);
            return true;
        }
        return false;
    }
    
    // Find all valid neighbors
    int *neighbors = (int*)malloc(g->n * sizeof(int));
    int count = 0;
    
    for (int i = 0; i < g->n; i++) {
        if (is_safe(g, path, pos, i)) {
            neighbors[count++] = i;
        }
    }
    
    // No valid neighbors, backtrack
    if (count == 0) {
        free(neighbors);
        return false;
    }
    
    // Sequential search if only one thread or one neighbor
    if (threads == 1 || count == 1) {
        for (int i = 0; i < count && !g->found; i++) {
            path[pos] = neighbors[i];
            if (hamiltonian_recursive(g, path, pos + 1, threads)) {
                free(neighbors);
                return true;
            }
        }
    } else {
        // Parallel search: create threads for each neighbor branch
        pthread_t *threads_arr = (pthread_t*)malloc(count * sizeof(pthread_t));
        ThreadData *data = (ThreadData*)malloc(count * sizeof(ThreadData));
        int active = 0;
        
        // Distribute threads evenly among neighbors
        int per_neighbor = threads / count;
        int extra = threads % count;
        
        for (int i = 0; i < count && !g->found; i++) {
            // Calculate threads for this neighbor
            int t = per_neighbor + (i < extra ? 1 : 0);
            
            // Create a copy of the path for this thread
            int *path_copy = (int*)malloc(g->n * sizeof(int));
            memcpy(path_copy, path, g->n * sizeof(int));
            path_copy[pos] = neighbors[i];
            
            // Prepare thread data
            data[active].graph = g;
            data[active].path = path_copy;
            data[active].pos = pos + 1;
            data[active].threads = t;
            
            // Create thread if we have multiple threads and more vertices to explore
            if (t > 1 && pos < g->n - 1) {
                pthread_create(&threads_arr[active], NULL, search_thread, &data[active]);
                active++;
            } else {
                // Sequential search for this branch (handles t=0, t=1, or last position)
                hamiltonian_recursive(g, path_copy, pos + 1, t);
                free(path_copy);
            }
        }
        
        // Wait for all threads to complete and clean up
        for (int i = 0; i < active; i++) {
            pthread_join(threads_arr[i], NULL);
            free(data[i].path);
        }
        
        free(threads_arr);
        free(data);
    }
    
    free(neighbors);
    return false;
}

// Thread entry point for parallel search
void* search_thread(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    hamiltonian_recursive(data->graph, data->path, data->pos, data->threads);
    return NULL;
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    if (argc < 3) {
        printf("Usage: %s <num_vertices> <num_threads> [file]\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    
    // Initialize graph structure
    Graph g;
    g.n = n;
    g.found = false;
    g.path = (int*)malloc(n * sizeof(int));
    pthread_mutex_init(&g.found_mutex, NULL);
    
    // Allocate and initialize adjacency matrix
    g.adj = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g.adj[i] = (int*)calloc(n, sizeof(int));
    }
    
    // Read graph from file or stdin
    if (argc > 3) {
        FILE *f = fopen(argv[3], "r");
        if (f) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    fscanf(f, "%d", &g.adj[i][j]);
                }
            }
            fclose(f);
        }
    } else {
        // Read from standard input
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &g.adj[i][j]);
            }
        }
    }
    
    // Start search from vertex 0
    int *path = (int*)malloc(n * sizeof(int));
    path[0] = 0;
    
    // Begin recursive search with parallel threads
    hamiltonian_recursive(&g, path, 1, num_threads);
    
    // Print results
    if (g.found) {
        printf("Hamiltonian cycle found: ");
        print_path(g.path, n);
    } else {
        printf("No Hamiltonian cycle found.\n");
    }
    
    // Clean up memory
    for (int i = 0; i < n; i++) {
        free(g.adj[i]);
    }
    free(g.adj);
    free(g.path);
    free(path);
    pthread_mutex_destroy(&g.found_mutex);
    
    return 0;
}
