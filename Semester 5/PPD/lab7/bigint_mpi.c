#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Big integer represented as array of digits (base 10)
// digits[0] is the least significant digit

typedef struct {
    int *digits;
    int size;
    int capacity;
} BigInt;

// Initialize a BigInt
BigInt* bigint_init(int capacity) {
    BigInt *bi = (BigInt *)malloc(sizeof(BigInt));
    bi->digits = (int *)calloc(capacity, sizeof(int));
    bi->size = 0;
    bi->capacity = capacity;
    return bi;
}

// Free a BigInt
void bigint_free(BigInt *bi) {
    if (bi) {
        free(bi->digits);
        free(bi);
    }
}

// Create BigInt from string
BigInt* bigint_from_string(const char *str) {
    int len = strlen(str);
    BigInt *bi = bigint_init(len + 10);
    bi->size = len;
    
    // Reverse the string to store least significant digit first
    for (int i = 0; i < len; i++) {
        bi->digits[i] = str[len - 1 - i] - '0';
    }
    
    return bi;
}

// Create BigInt from integer
BigInt* bigint_from_int(int value) {
    BigInt *bi = bigint_init(20);
    
    if (value == 0) {
        bi->size = 1;
        bi->digits[0] = 0;
        return bi;
    }
    
    int idx = 0;
    while (value > 0) {
        bi->digits[idx++] = value % 10;
        value /= 10;
    }
    bi->size = idx;
    
    return bi;
}

// Convert BigInt to string
char* bigint_to_string(BigInt *bi) {
    char *str = (char *)malloc((bi->size + 1) * sizeof(char));
    
    for (int i = 0; i < bi->size; i++) {
        str[i] = bi->digits[bi->size - 1 - i] + '0';
    }
    str[bi->size] = '\0';
    
    return str;
}

// Normalize BigInt (remove leading zeros, handle carries)
void bigint_normalize(BigInt *bi) {
    // Handle carries
    for (int i = 0; i < bi->capacity - 1; i++) {
        if (bi->digits[i] >= 10) {
            bi->digits[i + 1] += bi->digits[i] / 10;
            bi->digits[i] %= 10;
        }
    }
    
    // Update size
    int new_size = bi->capacity;
    while (new_size > 1 && bi->digits[new_size - 1] == 0) {
        new_size--;
    }
    bi->size = new_size;
}

// Regular O(n²) multiplication using MPI
void bigint_multiply_mpi(BigInt *a, BigInt *b, BigInt *result, int rank, int size) {
    int result_capacity = a->size + b->size + 10;
    
    // Initialize result
    memset(result->digits, 0, result->capacity * sizeof(int));
    result->size = 0;
    
    // Distribute work: each process multiplies a chunk of a with all of b
    int chunk_size = (a->size + size - 1) / size;
    int start_idx = rank * chunk_size;
    int end_idx = (start_idx + chunk_size < a->size) ? start_idx + chunk_size : a->size;
    
    // Each process computes partial result
    for (int i = start_idx; i < end_idx; i++) {
        for (int j = 0; j < b->size; j++) {
            result->digits[i + j] += a->digits[i] * b->digits[j];
        }
    }
}

// Regular sequential multiplication (for comparison)
void bigint_multiply_seq(BigInt *a, BigInt *b, BigInt *result) {
    int result_capacity = a->size + b->size + 10;
    
    memset(result->digits, 0, result->capacity * sizeof(int));
    result->size = 0;
    
    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < b->size; j++) {
            result->digits[i + j] += a->digits[i] * b->digits[j];
        }
    }
    
    bigint_normalize(result);
}

// Karatsuba algorithm for big integers
void bigint_karatsuba_mpi(BigInt *a, BigInt *b, BigInt *result, int rank, int size) {
    // Base case: use regular multiplication for small numbers
    if (a->size <= 16 || b->size <= 16) {
        bigint_multiply_seq(a, b, result);
        bigint_normalize(result);
        return;
    }
    
    // Make both numbers same length
    int max_len = (a->size > b->size) ? a->size : b->size;
    
    // Pad to even length
    if (max_len % 2 == 1) {
        max_len++;
    }
    
    // Split at midpoint
    int mid = max_len / 2;
    
    // Create split BigInts
    BigInt *a_low = bigint_init(mid + 10);
    BigInt *a_high = bigint_init(mid + 10);
    BigInt *b_low = bigint_init(mid + 10);
    BigInt *b_high = bigint_init(mid + 10);
    
    // Copy digits
    int a_low_size = (mid < a->size) ? mid : a->size;
    int a_high_size = (a->size > mid) ? a->size - mid : 0;
    int b_low_size = (mid < b->size) ? mid : b->size;
    int b_high_size = (b->size > mid) ? b->size - mid : 0;
    
    memcpy(a_low->digits, a->digits, a_low_size * sizeof(int));
    a_low->size = a_low_size;
    if (a_high_size > 0) {
        memcpy(a_high->digits, a->digits + mid, a_high_size * sizeof(int));
        a_high->size = a_high_size;
    } else {
        a_high->size = 1;
        a_high->digits[0] = 0;
    }
    
    memcpy(b_low->digits, b->digits, b_low_size * sizeof(int));
    b_low->size = b_low_size;
    if (b_high_size > 0) {
        memcpy(b_high->digits, b->digits + mid, b_high_size * sizeof(int));
        b_high->size = b_high_size;
    } else {
        b_high->size = 1;
        b_high->digits[0] = 0;
    }
    
    // Allocate space for z0, z1, z2
    int z_size = 2 * mid + 10;
    BigInt *z0 = bigint_init(z_size);
    BigInt *z2 = bigint_init(z_size);
    
    // Distribute work: rank 0 does z0, rank 1 does z2
    if (rank == 0) {
        bigint_karatsuba_mpi(a_low, b_low, z0, rank, size);
        // If only one process, also compute z2
        if (size == 1) {
            bigint_karatsuba_mpi(a_high, b_high, z2, rank, size);
        }
    } else if (rank == 1 && size > 1) {
        bigint_karatsuba_mpi(a_high, b_high, z2, rank, size);
    }
    
    // Compute z1 at rank 0
    BigInt *z1 = NULL;
    if (rank == 0) {
        // (a_low + a_high) * (b_low + b_high)
        BigInt *a_sum = bigint_init(mid + 10);
        BigInt *b_sum = bigint_init(mid + 10);
        
        // Add a_low + a_high
        int max_a = (a_low->size > a_high->size) ? a_low->size : a_high->size;
        for (int i = 0; i < max_a; i++) {
            int val = 0;
            if (i < a_low->size) val += a_low->digits[i];
            if (i < a_high->size) val += a_high->digits[i];
            a_sum->digits[i] = val;
        }
        a_sum->size = max_a;
        bigint_normalize(a_sum);
        
        // Add b_low + b_high
        int max_b = (b_low->size > b_high->size) ? b_low->size : b_high->size;
        for (int i = 0; i < max_b; i++) {
            int val = 0;
            if (i < b_low->size) val += b_low->digits[i];
            if (i < b_high->size) val += b_high->digits[i];
            b_sum->digits[i] = val;
        }
        b_sum->size = max_b;
        bigint_normalize(b_sum);
        
        BigInt *z1_sum = bigint_init(z_size);
        bigint_karatsuba_mpi(a_sum, b_sum, z1_sum, rank, size);
        
        // z1 = z1_sum - z0 - z2
        z1 = bigint_init(z_size);
        memcpy(z1->digits, z1_sum->digits, z1_sum->size * sizeof(int));
        z1->size = z1_sum->size;
        
        // Subtract z0
        for (int i = 0; i < z0->size; i++) {
            z1->digits[i] -= z0->digits[i];
        }
        
        // Subtract z2
        for (int i = 0; i < z2->size; i++) {
            z1->digits[i] -= z2->digits[i];
        }
        
        bigint_normalize(z1);
        
        bigint_free(a_sum);
        bigint_free(b_sum);
        bigint_free(z1_sum);
    }
    
    // Gather z2 from rank 1
    if (size > 1) {
        if (rank == 1) {
            MPI_Send(&z2->size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(z2->digits, z2->size, MPI_INT, 0, 1, MPI_COMM_WORLD);
        } else if (rank == 0) {
            MPI_Recv(&z2->size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (z2->size > z2->capacity) {
                z2->digits = (int *)realloc(z2->digits, z2->size * sizeof(int));
                z2->capacity = z2->size;
            }
            MPI_Recv(z2->digits, z2->size, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    
    // Combine results at rank 0: result = z0 + z1*10^mid + z2*10^(2*mid)
    if (rank == 0) {
        memset(result->digits, 0, result->capacity * sizeof(int));
        result->size = 0;
        
        // Add z0
        for (int i = 0; i < z0->size; i++) {
            result->digits[i] += z0->digits[i];
        }
        
        // Add z1 shifted by mid
        if (z1) {
            for (int i = 0; i < z1->size; i++) {
                result->digits[i + mid] += z1->digits[i];
            }
        }
        
        // Add z2 shifted by 2*mid
        for (int i = 0; i < z2->size; i++) {
            result->digits[i + 2 * mid] += z2->digits[i];
        }
        
        bigint_normalize(result);
        
        if (z1) {
            bigint_free(z1);
        }
    }
    
    bigint_free(a_low);
    bigint_free(a_high);
    bigint_free(b_low);
    bigint_free(b_high);
    bigint_free(z0);
    bigint_free(z2);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc < 3) {
        if (rank == 0) {
            printf("Usage: %s <num1> <num2> [algorithm]\n", argv[0]);
            printf("  num1, num2: big integers as strings\n");
            printf("  algorithm: 'regular' or 'karatsuba' (default: regular)\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    char *num1_str = argv[1];
    char *num2_str = argv[2];
    char *algorithm = (argc > 3) ? argv[3] : "regular";
    
    BigInt *a = NULL;
    BigInt *b = NULL;
    BigInt *result = NULL;
    
    if (rank == 0) {
        a = bigint_from_string(num1_str);
        b = bigint_from_string(num2_str);
        
        printf("Number 1: %s\n", num1_str);
        printf("Number 2: %s\n", num2_str);
    }
    
    // Broadcast sizes
    int a_size, b_size;
    if (rank == 0) {
        a_size = a->size;
        b_size = b->size;
    }
    MPI_Bcast(&a_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Allocate and broadcast BigInts
    if (rank != 0) {
        a = bigint_init(a_size + 10);
        b = bigint_init(b_size + 10);
        a->size = a_size;
        b->size = b_size;
    }
    
    MPI_Bcast(a->digits, a_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b->digits, b_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    int result_capacity = a_size + b_size + 100;
    result = bigint_init(result_capacity);
    
    // Time the computation
    double start_time = MPI_Wtime();
    
    if (strcmp(algorithm, "karatsuba") == 0) {
        if (rank == 0) {
            printf("Using Karatsuba algorithm with %d processes\n", size);
        }
        bigint_karatsuba_mpi(a, b, result, rank, size);
    } else {
        if (rank == 0) {
            printf("Using Regular O(n²) algorithm with %d processes\n", size);
        }
        
        BigInt *partial_result = bigint_init(result_capacity);
        bigint_multiply_mpi(a, b, partial_result, rank, size);
        
        // Reduce partial results
        MPI_Reduce(partial_result->digits, result->digits, result_capacity, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        if (rank == 0) {
            bigint_normalize(result);
        }
        
        bigint_free(partial_result);
    }
    
    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;
    
    if (rank == 0) {
        char *result_str = bigint_to_string(result);
        printf("\nResult: %s\n", result_str);
        printf("Time: %.6f seconds\n", elapsed);
        free(result_str);
    }
    
    bigint_free(a);
    bigint_free(b);
    bigint_free(result);
    
    MPI_Finalize();
    return 0;
}

