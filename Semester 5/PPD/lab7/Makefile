CC = mpicc
CFLAGS = -Wall -O2 -std=c99
LDFLAGS = 

# Targets
all: polynomial_mpi compare bigint_mpi

polynomial_mpi: polynomial_mpi.c
	$(CC) $(CFLAGS) -o polynomial_mpi polynomial_mpi.c $(LDFLAGS)

compare: compare.c
	$(CC) $(CFLAGS) -o compare compare.c $(LDFLAGS)

bigint_mpi: bigint_mpi.c
	$(CC) $(CFLAGS) -o bigint_mpi bigint_mpi.c $(LDFLAGS)

clean:
	rm -f polynomial_mpi compare bigint_mpi *.o

.PHONY: all clean

