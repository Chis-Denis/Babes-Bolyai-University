CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c11
LDFLAGS=-lpthread

TARGET=matrix_threads
SRC=matrix_threads.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

