CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
TARGET = hamiltonian
SOURCE = hamiltonian.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

# Run with graph2.txt (25 vertices, big graph)
run-graph2:
	$(TARGET).exe 25 4 graph2.txt

# Run with graph3.txt (60 vertices, extremely big graph)
run-graph3:
	$(TARGET).exe 60 8 graph3.txt

# Run with original graph.txt (4 vertices)
run-graph:
	$(TARGET).exe 4 2 graph.txt

.PHONY: clean run-graph2 run-graph3 run-graph



