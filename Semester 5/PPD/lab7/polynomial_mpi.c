#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Regular O(n²) polynomial multiplication using MPI
void regular_multiply_mpi(int *poly1, int n1, int *poly2, int n2, int *result, int result_size, int rank, int size) {
    // Initialize result to zero
    memset(result, 0, result_size * sizeof(int));
    
    // Calculate chunk size for this process
    int chunk_size = (n1 + size - 1) / size;  // Ceiling division
    int start_idx = rank * chunk_size;
    int end_idx = (start_idx + chunk_size < n1) ? start_idx + chunk_size : n1;
    
    // Each process computes its chunk: poly1[start_idx:end_idx] * poly2
    for (int i = start_idx; i < end_idx; i++) {
        for (int j = 0; j < n2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

// Karatsuba algorithm helper functions
void add_polynomials(int *poly1, int n1, int *poly2, int n2, int *result) {
    int max_len = (n1 > n2) ? n1 : n2;
    memset(result, 0, max_len * sizeof(int));
    
    for (int i = 0; i < n1; i++) {
        result[i] += poly1[i];
    }
    for (int i = 0; i < n2; i++) {
        result[i] += poly2[i];
    }
}

void subtract_polynomials(int *poly1, int n1, int *poly2, int n2, int *result) {
    int max_len = (n1 > n2) ? n1 : n2;
    memset(result, 0, max_len * sizeof(int));
    
    for (int i = 0; i < n1; i++) {
        result[i] += poly1[i];
    }
    for (int i = 0; i < n2; i++) {
        result[i] -= poly2[i];
    }
}

// Regular sequential multiplication (used as base case for Karatsuba)
void regular_multiply_seq(int *poly1, int n1, int *poly2, int n2, int *result) {
    int result_size = n1 + n2 - 1;
    memset(result, 0, result_size * sizeof(int));
    
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

// Karatsuba algorithm using MPI
void karatsuba_mpi(int *poly1, int n1, int *poly2, int n2, int *result, int rank, int size) {
    // Base case
    if (n1 <= 16 || n2 <= 16) {
        regular_multiply_seq(poly1, n1, poly2, n2, result);
        return;
    }
    
    // Pad to same length
    int max_len = (n1 > n2) ? n1 : n2;
    int *p1 = (int *)calloc(max_len, sizeof(int));
    int *p2 = (int *)calloc(max_len, sizeof(int));
    memcpy(p1, poly1, n1 * sizeof(int));
    memcpy(p2, poly2, n2 * sizeof(int));
    
    if (max_len % 2 == 1) {
        p1 = (int *)realloc(p1, (max_len + 1) * sizeof(int));
        p2 = (int *)realloc(p2, (max_len + 1) * sizeof(int));
        p1[max_len] = 0;
        p2[max_len] = 0;
        max_len++;
    }
    
    int mid = max_len / 2;
    int result_size = 2 * max_len - 1;
    memset(result, 0, result_size * sizeof(int));
    
    // Split
    int *low1 = p1;
    int *high1 = p1 + mid;
    int *low2 = p2;
    int *high2 = p2 + mid;
    
    // Allocate results
    int z0_size = 2 * mid - 1;
    int z2_size = 2 * mid - 1;
    int *z0 = (int *)calloc(z0_size, sizeof(int));
    int *z2 = (int *)calloc(z2_size, sizeof(int));
    
    // Initialize z2 to zero on all processes
    memset(z2, 0, z2_size * sizeof(int));
    
    // Distribute work: rank 0 does z0, rank 1 does z2, rank 0 also does z1
    if (rank == 0) {
        karatsuba_mpi(low1, mid, low2, mid, z0, rank, size);
        // If only one process, also compute z2
        if (size == 1) {
            karatsuba_mpi(high1, mid, high2, mid, z2, rank, size);
        }
    } else if (rank == 1 && size > 1) {
        karatsuba_mpi(high1, mid, high2, mid, z2, rank, size);
    }
    
    // Compute z1 at rank 0
    int *z1 = NULL;
    int z1_size = 2 * mid - 1;
    if (rank == 0) {
        int *sum1 = (int *)calloc(mid, sizeof(int));
        int *sum2 = (int *)calloc(mid, sizeof(int));
        add_polynomials(low1, mid, high1, mid, sum1);
        add_polynomials(low2, mid, high2, mid, sum2);
        
        int *z1_sum = (int *)calloc(z1_size, sizeof(int));
        karatsuba_mpi(sum1, mid, sum2, mid, z1_sum, rank, size);
        
        z1 = (int *)calloc(z1_size, sizeof(int));
        subtract_polynomials(z1_sum, z1_size, z0, z0_size, z1);
        subtract_polynomials(z1, z1_size, z2, z2_size, z1);
        
        free(sum1);
        free(sum2);
        free(z1_sum);
    }
    
    // Gather z2 from rank 1 if needed
    if (size > 1) {
        if (rank == 1) {
            MPI_Send(z2, z2_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else if (rank == 0) {
            MPI_Recv(z2, z2_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // Synchronize all processes
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    // Combine at rank 0
    if (rank == 0) {
        for (int i = 0; i < z0_size; i++) {
            result[i] += z0[i];
        }
        if (z1) {
            for (int i = 0; i < z1_size; i++) {
                result[i + mid] += z1[i];
            }
            free(z1);
        }
        for (int i = 0; i < z2_size; i++) {
            result[i + 2 * mid] += z2[i];
        }
    } else {
        // Other ranks: ensure result buffer is zeroed (already done by memset)
        // No additional work needed
    }
    
    free(z0);
    free(z2);
    free(p1);
    free(p2);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int n1, n2;
    char *algorithm;
    int example_mode = 1;  // Always use example mode by default
    
    // Default: always run example (1000x1000, all coefficients = 1, Karatsuba)
    n1 = 1000;
    n2 = 1000;
    algorithm = "karatsuba";
    
    if (rank == 0) {
        printf("Running example: Karatsuba with two 1000-element polynomials (all coefficients = 1)\n");
    }
    
    // Generate random polynomials
    int *poly1 = NULL;
    int *poly2 = NULL;
    
    if (rank == 0) {
        poly1 = (int *)malloc(n1 * sizeof(int));
        poly2 = (int *)malloc(n2 * sizeof(int));
        
        if (example_mode) {
            // Example mode: all coefficients = 1
            for (int i = 0; i < n1; i++) {
                poly1[i] = 1;
            }
            for (int i = 0; i < n2; i++) {
                poly2[i] = 1;
            }
        } else {
            srand(time(NULL));
            for (int i = 0; i < n1; i++) {
                poly1[i] = rand() % 21 - 10;  // -10 to 10
            }
            for (int i = 0; i < n2; i++) {
                poly2[i] = rand() % 21 - 10;
            }
        }
        
        printf("Polynomial 1 (degree %d): ", n1 - 1);
        for (int i = 0; i < n1 && i < 10; i++) {
            printf("%d ", poly1[i]);
        }
        if (n1 > 10) printf("...");
        printf("\n");
        
        printf("Polynomial 2 (degree %d): ", n2 - 1);
        for (int i = 0; i < n2 && i < 10; i++) {
            printf("%d ", poly2[i]);
        }
        if (n2 > 10) printf("...");
        printf("\n");
    }
    
    // Broadcast polynomial sizes
    MPI_Bcast(&n1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n2, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Allocate space for polynomials on all processes
    if (rank != 0) {
        poly1 = (int *)malloc(n1 * sizeof(int));
        poly2 = (int *)malloc(n2 * sizeof(int));
    }
    
    // Broadcast polynomials
    MPI_Bcast(poly1, n1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(poly2, n2, MPI_INT, 0, MPI_COMM_WORLD);
    
    int result_size = n1 + n2 - 1;
    // Allocate extra space for Karatsuba padding (max_len can be padded to even)
    int max_padded = ((n1 > n2 ? n1 : n2) % 2 == 1) ? (n1 > n2 ? n1 : n2) + 1 : (n1 > n2 ? n1 : n2);
    int safe_result_size = 2 * max_padded;  // Extra safety margin
    int *result = (int *)calloc(safe_result_size, sizeof(int));
    int *partial_result = (int *)calloc(safe_result_size, sizeof(int));
    
    // Time the computation
    double start_time = MPI_Wtime();
    
    if (strcmp(algorithm, "karatsuba") == 0) {
        if (rank == 0) {
            printf("Using Karatsuba algorithm with %d processes\n", size);
        }
        karatsuba_mpi(poly1, n1, poly2, n2, partial_result, rank, size);
    } else {
        if (rank == 0) {
            printf("Using Regular O(n²) algorithm with %d processes\n", size);
        }
        regular_multiply_mpi(poly1, n1, poly2, n2, partial_result, result_size, rank, size);
    }
    
    // Reduce all partial results
    MPI_Reduce(partial_result, result, result_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;
    
    if (rank == 0) {
        if (example_mode) {
            // Example mode: show ALL result elements
            printf("\nResult (all %d coefficients):\n", result_size);
            for (int i = 0; i < result_size; i++) {
                printf("%d ", result[i]);
                // Print newline every 20 elements for readability
                if ((i + 1) % 20 == 0) printf("\n");
            }
            if (result_size % 20 != 0) printf("\n");
        } else {
            // Normal mode: show first 20 only
            printf("\nResult (first 20 coefficients): ");
            for (int i = 0; i < result_size && i < 20; i++) {
                printf("%d ", result[i]);
            }
            if (result_size > 20) printf("...");
            printf("\n");
        }
        printf("Time: %.6f seconds\n", elapsed);
    }
    
    free(poly1);
    free(poly2);
    free(result);
    free(partial_result);
    
    MPI_Finalize();
    return 0;
}

