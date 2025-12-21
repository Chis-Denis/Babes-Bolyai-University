#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    size_t rows;
    size_t cols;
    double *data; /* row-major, length rows*cols */
} Matrix;

#define MATRIX_SIZE 1000
#define NUM_THREADS 16
#define PRINT_MATRICES 1
/* Random seed: set 0 */
#define RNG_SEED 42

static Matrix allocate_matrix(size_t rows, size_t cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (double*)malloc(sizeof(double) * rows * cols);
    if (!m.data) { 
        fprintf(stderr, "Failed to allocate matrix %zux%zu\n", rows, cols);
        exit(1);
    }
    return m;
}

static void free_matrix(Matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

static inline double* elem_ptr(Matrix *m, size_t r, size_t c) {
    return &m->data[r * m->cols + c];
}

static inline const double* elem_cptr(const Matrix *m, size_t r, size_t c) {
    return &m->data[r * m->cols + c];
}

static void fill_matrix_random(Matrix *m, unsigned int seed) {
    srand(seed);
    for (size_t r = 0; r < m->rows; ++r) {
        for (size_t c = 0; c < m->cols; ++c) {
            /* integers in [0,9] as doubles */
            m->data[r * m->cols + c] = (double)(rand() % 10);
        }
    }
}


/* Compute a single element of C = A x B at (row, col) */
static double compute_element(const Matrix *A, const Matrix *B, size_t row, size_t col) {
    double sum = 0.0;
    size_t K = A->cols;
    for (size_t k = 0; k < K; ++k) {
        sum += (*elem_cptr(A, row, k)) * (*elem_cptr(B, k, col));
    }
    return sum;
}

typedef enum {
    STRAT_ROW_CHUNKS = 1, /* consecutive elements row-wise */
    STRAT_COL_CHUNKS = 2, /* consecutive columns assigned */
    STRAT_STRIDED    = 3  /* k-th element striding */
} Strategy;

typedef struct {
    const Matrix *A;
    const Matrix *B;
    Matrix *C;
    int thread_id;
    int num_threads;
    Strategy strategy;
} WorkerArgs;

static void* worker_row_chunks(void *arg) {
    WorkerArgs *wa = (WorkerArgs*)arg;
    size_t total = wa->C->rows * wa->C->cols;
    size_t base = (total / wa->num_threads) * (size_t)wa->thread_id;
    size_t rem = total % (size_t)wa->num_threads;
    size_t extra_before = (size_t)wa->thread_id < rem ? (size_t)wa->thread_id : rem;
    size_t chunk = total / (size_t)wa->num_threads + ((size_t)wa->thread_id < rem ? 1 : 0);
    size_t start = base + extra_before;
    size_t end = start + chunk;

    for (size_t idx = start; idx < end; ++idx) {
        size_t r = idx / wa->C->cols;
        size_t c = idx % wa->C->cols;
        double val = compute_element(wa->A, wa->B, r, c);
        *elem_ptr(wa->C, r, c) = val;
        (void)wa;
    }
    return NULL;
}

static void* worker_col_chunks(void *arg) {
    WorkerArgs *wa = (WorkerArgs*)arg;
    size_t cols = wa->C->cols;
    size_t base = (cols / (size_t)wa->num_threads) * (size_t)wa->thread_id;
    size_t rem = cols % (size_t)wa->num_threads;
    size_t extra_before = (size_t)wa->thread_id < rem ? (size_t)wa->thread_id : rem;
    size_t chunk = cols / (size_t)wa->num_threads + ((size_t)wa->thread_id < rem ? 1 : 0);
    size_t c_start = base + extra_before;
    size_t c_end = c_start + chunk;

    for (size_t r = 0; r < wa->C->rows; ++r) {
        for (size_t c = c_start; c < c_end; ++c) {
            double val = compute_element(wa->A, wa->B, r, c);
            *elem_ptr(wa->C, r, c) = val;
            (void)wa;
        }
    }
    return NULL;
}

static void* worker_strided(void *arg) {
    WorkerArgs *wa = (WorkerArgs*)arg;
    size_t total = wa->C->rows * wa->C->cols;
    for (size_t idx = (size_t)wa->thread_id; idx < total; idx += (size_t)wa->num_threads) {
        size_t r = idx / wa->C->cols;
        size_t c = idx % wa->C->cols;
        double val = compute_element(wa->A, wa->B, r, c);
        *elem_ptr(wa->C, r, c) = val;
        (void)wa;
    }
    return NULL;
}

static double now_seconds(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER counter;
    if (freq.QuadPart == 0) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
#endif
}

static void multiply_reference(const Matrix *A, const Matrix *B, Matrix *C) {
    for (size_t r = 0; r < C->rows; ++r) {
        for (size_t c = 0; c < C->cols; ++c) {
            *elem_ptr(C, r, c) = compute_element(A, B, r, c);
        }
    }
}

static int compare_matrices(const Matrix *X, const Matrix *Y, double eps) {
    if (X->rows != Y->rows || X->cols != Y->cols) return 0;
    size_t n = X->rows * X->cols;
    for (size_t i = 0; i < n; ++i) {
        double a = X->data[i];
        double b = Y->data[i];
        double diff = a - b;
        if (diff < 0) diff = -diff;
        if (diff > eps) return 0;
    }
    return 1;
}
int main(void) {
    const size_t sizes[] = {100, 500, 1000, 2500};
    const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_threads = NUM_THREADS;

    printf("Number of Threads: %d\n\n", num_threads);
    printf("Size\tSeq(ms)\tRow(ms)\tCol(ms)\tStrided(ms)\tOK1\tOK2\tOK3\n");

    for (size_t si = 0; si < num_sizes; ++si) {
        size_t n = sizes[si];
        size_t m = n, p = n;

        Matrix A = allocate_matrix(n, m);
        Matrix B = allocate_matrix(m, p);
        Matrix Cseq = allocate_matrix(n, p);
        Matrix C1 = allocate_matrix(n, p);
        Matrix C2 = allocate_matrix(n, p);
        Matrix C3 = allocate_matrix(n, p);

        unsigned int seed = (RNG_SEED == 0) ? (unsigned int)time(NULL) : (unsigned int)(RNG_SEED + (unsigned int)n);
        fill_matrix_random(&A, seed);
        fill_matrix_random(&B, seed ^ 0x9e3779b9u);

        double t0 = now_seconds();
        multiply_reference(&A, &B, &Cseq);
        double t1 = now_seconds();
        long long sequential_ms = (long long)((t1 - t0) * 1000.0);

        pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * (size_t)num_threads);
        WorkerArgs *args = (WorkerArgs*)malloc(sizeof(WorkerArgs) * (size_t)num_threads);
        if (!threads || !args) { fprintf(stderr, "Thread alloc failed\n"); return 1; }

        for (int tid = 0; tid < num_threads; ++tid) {
            args[tid].A = &A; args[tid].B = &B; args[tid].C = &C1;
            args[tid].thread_id = tid; args[tid].num_threads = num_threads;
            args[tid].strategy = STRAT_ROW_CHUNKS;
        }
        double s1 = now_seconds();
        for (int tid = 0; tid < num_threads; ++tid) pthread_create(&threads[tid], NULL, worker_row_chunks, &args[tid]);
        for (int tid = 0; tid < num_threads; ++tid) pthread_join(threads[tid], NULL);
        double e1 = now_seconds();
        long long time1 = (long long)((e1 - s1) * 1000.0);

        for (int tid = 0; tid < num_threads; ++tid) { args[tid].C = &C2; args[tid].strategy = STRAT_COL_CHUNKS; }
        double s2 = now_seconds();
        for (int tid = 0; tid < num_threads; ++tid) pthread_create(&threads[tid], NULL, worker_col_chunks, &args[tid]);
        for (int tid = 0; tid < num_threads; ++tid) pthread_join(threads[tid], NULL);
        double e2 = now_seconds();
        long long time2 = (long long)((e2 - s2) * 1000.0);

        for (int tid = 0; tid < num_threads; ++tid) { args[tid].C = &C3; args[tid].strategy = STRAT_STRIDED; }
        double s3 = now_seconds();
        for (int tid = 0; tid < num_threads; ++tid) pthread_create(&threads[tid], NULL, worker_strided, &args[tid]);
        for (int tid = 0; tid < num_threads; ++tid) pthread_join(threads[tid], NULL);
        double e3 = now_seconds();
        long long time3 = (long long)((e3 - s3) * 1000.0);

        int correct1 = compare_matrices(&Cseq, &C1, 1e-9);
        int correct2 = compare_matrices(&Cseq, &C2, 1e-9);
        int correct3 = compare_matrices(&Cseq, &C3, 1e-9);

        printf("%zu\t%lld\t%lld\t%lld\t%lld\t%s\t%s\t%s\n",
               n, sequential_ms, time1, time2, time3,
               correct1 ? "OK" : "X", correct2 ? "OK" : "X", correct3 ? "OK" : "X");

        free(threads); free(args);
        free_matrix(&A); free_matrix(&B);
        free_matrix(&Cseq); free_matrix(&C1); free_matrix(&C2); free_matrix(&C3);
    }

    return 0;
}


