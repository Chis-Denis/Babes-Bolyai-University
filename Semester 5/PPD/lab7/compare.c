#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Regular O(n²) polynomial multiplication using MPI
void regular_multiply_mpi(int *poly1, int n1, int *poly2, int n2, int *result, int result_size, int rank, int size) {
    memset(result, 0, result_size * sizeof(int));
    
    int chunk_size = (n1 + size - 1) / size;
    int start_idx = rank * chunk_size;
    int end_idx = (start_idx + chunk_size < n1) ? start_idx + chunk_size : n1;
    
    for (int i = start_idx; i < end_idx; i++) {
        for (int j = 0; j < n2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

// Regular sequential multiplication (for comparison)
void regular_multiply_seq(int *poly1, int n1, int *poly2, int n2, int *result) {
    int result_size = n1 + n2 - 1;
    memset(result, 0, result_size * sizeof(int));
    
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

// Helper functions for Karatsuba
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

// Karatsuba sequential (for comparison)
void karatsuba_seq(int *poly1, int n1, int *poly2, int n2, int *result) {
    if (n1 <= 16 || n2 <= 16) {
        regular_multiply_seq(poly1, n1, poly2, n2, result);
        return;
    }
    
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
    
    int *low1 = p1;
    int *high1 = p1 + mid;
    int *low2 = p2;
    int *high2 = p2 + mid;
    
    int z0_size = 2 * mid - 1;
    int z1_size = 2 * mid - 1;
    int z2_size = 2 * mid - 1;
    int *z0 = (int *)calloc(z0_size, sizeof(int));
    int *z1 = (int *)calloc(z1_size, sizeof(int));
    int *z2 = (int *)calloc(z2_size, sizeof(int));
    
    karatsuba_seq(low1, mid, low2, mid, z0);
    karatsuba_seq(high1, mid, high2, mid, z2);
    
    int *sum1 = (int *)calloc(mid, sizeof(int));
    int *sum2 = (int *)calloc(mid, sizeof(int));
    add_polynomials(low1, mid, high1, mid, sum1);
    add_polynomials(low2, mid, high2, mid, sum2);
    
    int *z1_sum = (int *)calloc(z1_size, sizeof(int));
    karatsuba_seq(sum1, mid, sum2, mid, z1_sum);
    
    subtract_polynomials(z1_sum, z1_size, z0, z0_size, z1);
    subtract_polynomials(z1, z1_size, z2, z2_size, z1);
    
    for (int i = 0; i < z0_size; i++) {
        result[i] += z0[i];
    }
    for (int i = 0; i < z1_size; i++) {
        result[i + mid] += z1[i];
    }
    for (int i = 0; i < z2_size; i++) {
        result[i + 2 * mid] += z2[i];
    }
    
    free(z0);
    free(z1);
    free(z2);
    free(p1);
    free(p2);
    free(sum1);
    free(sum2);
    free(z1_sum);
}

// Karatsuba MPI
void karatsuba_mpi(int *poly1, int n1, int *poly2, int n2, int *result, int rank, int size) {
    if (n1 <= 16 || n2 <= 16) {
        regular_multiply_seq(poly1, n1, poly2, n2, result);
        return;
    }
    
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
    
    int *low1 = p1;
    int *high1 = p1 + mid;
    int *low2 = p2;
    int *high2 = p2 + mid;
    
    int z0_size = 2 * mid - 1;
    int z2_size = 2 * mid - 1;
    int *z0 = (int *)calloc(z0_size, sizeof(int));
    int *z2 = (int *)calloc(z2_size, sizeof(int));
    
    // Distribute work: rank 0 does z0, rank 1 does z2
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
    
    // Gather z2 from rank 1
    if (size > 1) {
        if (rank == 1) {
            MPI_Send(z2, z2_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else if (rank == 0) {
            MPI_Recv(z2, z2_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
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
    }
    
    free(z0);
    free(z2);
    free(p1);
    free(p2);
}

// Compare results
int compare_results(int *result1, int *result2, int size) {
    for (int i = 0; i < size; i++) {
        if (result1[i] != result2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc < 3) {
        if (rank == 0) {
            printf("Usage: %s <n1> <n2> [num_runs]\n", argv[0]);
            printf("  n1, n2: sizes of the two polynomials\n");
            printf("  num_runs: number of benchmark runs (default: 3)\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    int num_runs = (argc > 3) ? atoi(argv[3]) : 3;
    
    // Generate random polynomials
    int *poly1 = NULL;
    int *poly2 = NULL;
    
    if (rank == 0) {
        poly1 = (int *)malloc(n1 * sizeof(int));
        poly2 = (int *)malloc(n2 * sizeof(int));
        
        srand(42);  // Fixed seed for reproducibility
        for (int i = 0; i < n1; i++) {
            poly1[i] = rand() % 21 - 10;
        }
        for (int i = 0; i < n2; i++) {
            poly2[i] = rand() % 21 - 10;
        }
    }
    
    // Broadcast sizes
    MPI_Bcast(&n1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n2, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        poly1 = (int *)malloc(n1 * sizeof(int));
        poly2 = (int *)malloc(n2 * sizeof(int));
    }
    
    MPI_Bcast(poly1, n1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(poly2, n2, MPI_INT, 0, MPI_COMM_WORLD);
    
    int result_size = n1 + n2 - 1;
    
    if (rank == 0) {
        printf("\n======================================================================\n");
        printf("Polynomial Multiplication Benchmark\n");
        printf("Poly1 degree: %d, Poly2 degree: %d\n", n1 - 1, n2 - 1);
        printf("MPI Processes: %d\n", size);
        printf("======================================================================\n\n");
    }
    
    // Benchmark regular sequential (only on rank 0)
    double regular_seq_time = 0.0;
    int *regular_seq_result = NULL;
    if (rank == 0) {
        regular_seq_result = (int *)calloc(result_size, sizeof(int));
        printf("Testing Regular O(n²) Sequential...\n");
        
        for (int run = 0; run < num_runs; run++) {
            double start = MPI_Wtime();
            regular_multiply_seq(poly1, n1, poly2, n2, regular_seq_result);
            double end = MPI_Wtime();
            regular_seq_time += (end - start);
        }
        regular_seq_time /= num_runs;
        printf("  Average time: %.6f seconds\n\n", regular_seq_time);
    }
    
    // Benchmark regular MPI
    double regular_mpi_time = 0.0;
    int *regular_mpi_result = (int *)calloc(result_size, sizeof(int));
    int *regular_mpi_partial = (int *)calloc(result_size, sizeof(int));
    
    if (rank == 0) {
        printf("Testing Regular O(n²) MPI...\n");
    }
    
    for (int run = 0; run < num_runs; run++) {
        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        
        memset(regular_mpi_partial, 0, result_size * sizeof(int));
        regular_multiply_mpi(poly1, n1, poly2, n2, regular_mpi_partial, result_size, rank, size);
        MPI_Reduce(regular_mpi_partial, regular_mpi_result, result_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        double end = MPI_Wtime();
        if (rank == 0) {
            regular_mpi_time += (end - start);
        }
    }
    
    if (rank == 0) {
        regular_mpi_time /= num_runs;
        printf("  Average time: %.6f seconds\n", regular_mpi_time);
        printf("  Speedup: %.2fx\n\n", regular_seq_time / regular_mpi_time);
    }
    
    // Benchmark Karatsuba sequential (only on rank 0)
    double karatsuba_seq_time = 0.0;
    int *karatsuba_seq_result = NULL;
    if (rank == 0) {
        karatsuba_seq_result = (int *)calloc(result_size, sizeof(int));
        printf("Testing Karatsuba Sequential...\n");
        
        for (int run = 0; run < num_runs; run++) {
            double start = MPI_Wtime();
            karatsuba_seq(poly1, n1, poly2, n2, karatsuba_seq_result);
            double end = MPI_Wtime();
            karatsuba_seq_time += (end - start);
        }
        karatsuba_seq_time /= num_runs;
        printf("  Average time: %.6f seconds\n\n", karatsuba_seq_time);
    }
    
    // Benchmark Karatsuba MPI
    double karatsuba_mpi_time = 0.0;
    int *karatsuba_mpi_result = (int *)calloc(result_size, sizeof(int));
    
    if (rank == 0) {
        printf("Testing Karatsuba MPI...\n");
    }
    
    for (int run = 0; run < num_runs; run++) {
        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        
        memset(karatsuba_mpi_result, 0, result_size * sizeof(int));
        karatsuba_mpi(poly1, n1, poly2, n2, karatsuba_mpi_result, rank, size);
        
        double end = MPI_Wtime();
        if (rank == 0) {
            karatsuba_mpi_time += (end - start);
        }
    }
    
    if (rank == 0) {
        karatsuba_mpi_time /= num_runs;
        printf("  Average time: %.6f seconds\n", karatsuba_mpi_time);
        printf("  Speedup: %.2fx\n\n", karatsuba_seq_time / karatsuba_mpi_time);
    }
    
    // Verify correctness
    if (rank == 0) {
        printf("Verifying correctness...\n");
        int all_match = 1;
        
        if (!compare_results(regular_seq_result, regular_mpi_result, result_size)) {
            printf("  ERROR: Regular MPI result doesn't match sequential!\n");
            all_match = 0;
        }
        
        if (!compare_results(regular_seq_result, karatsuba_seq_result, result_size)) {
            printf("  ERROR: Karatsuba sequential result doesn't match!\n");
            all_match = 0;
        }
        
        if (!compare_results(regular_seq_result, karatsuba_mpi_result, result_size)) {
            printf("  ERROR: Karatsuba MPI result doesn't match!\n");
            all_match = 0;
        }
        
        if (all_match) {
            printf("  [OK] All results match!\n\n");
        }
        
        // Print summary
        printf("======================================================================\n");
        printf("Performance Summary:\n");
        printf("======================================================================\n");
        printf("%-30s %15s %15s\n", "Algorithm", "Time (s)", "Speedup vs Seq");
        printf("----------------------------------------------------------------------\n");
        
        printf("%-30s %15.6f %15s\n", "Regular Sequential", regular_seq_time, "1.00x");
        printf("%-30s %15.6f %15.2fx\n", "Regular MPI", regular_mpi_time, regular_seq_time / regular_mpi_time);
        printf("%-30s %15.6f %15.2fx\n", "Karatsuba Sequential", karatsuba_seq_time, regular_seq_time / karatsuba_seq_time);
        printf("%-30s %15.6f %15.2fx\n", "Karatsuba MPI", karatsuba_mpi_time, regular_seq_time / karatsuba_mpi_time);
        printf("======================================================================\n\n");
        
        free(regular_seq_result);
        free(karatsuba_seq_result);
    }
    
    free(poly1);
    free(poly2);
    free(regular_mpi_result);
    free(regular_mpi_partial);
    free(karatsuba_mpi_result);
    
    MPI_Finalize();
    return 0;
}

