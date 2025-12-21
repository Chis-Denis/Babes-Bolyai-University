# Hamiltonian Cycle Finder - Java Implementation

A Java implementation for finding Hamiltonian cycles in directed graphs using parallel search with ForkJoinPool and RecursiveTask.

## Problem Description

Given a directed graph, find a Hamiltonian cycle (if one exists) using parallelized search. The implementation:
- Uses a specified number of threads to parallelize the search
- Starts from a fixed vertex (no need to try each vertex as starting point)
- Splits work between threads at multiple levels based on vertex out-degrees
- Allocates threads proportionally among branches

### Example Thread Allocation

If you have 8 threads and the first vertex has an out-degree of 3:
- Two branches will have 3 threads allocated each
- The remaining branch will have 2 threads

Further down the search tree, if a vertex has 4 out-neighbors:
- Two of them will be explored by one thread
- The other thread will explore the remaining 2 neighbors
- Eventually, the search becomes non-parallel when threads are exhausted

## Project Structure

```
src/main/java/
├── Graph.java                 - Directed graph representation
├── HamiltonianCycleTask.java  - RecursiveTask for parallel search
├── HamiltonianCycleFinder.java - Main finder class using ForkJoinPool
└── Main.java                  - Example usage and test cases
```

## How to Compile

Compile all Java files:
```bash
javac src/main/java/*.java
```

Or compile from the source directory:
```bash
cd src/main/java
javac *.java
```

## How to Run

Run the main class:
```bash
java -cp src/main/java Main
```

Or from the source directory:
```bash
cd src/main/java
java Main
```

## Usage Example

```java
// Create a graph
Graph graph = new Graph(4);
graph.addEdge(0, 1);
graph.addEdge(1, 2);
graph.addEdge(2, 3);
graph.addEdge(3, 0);

// Create finder with 8 threads
HamiltonianCycleFinder finder = new HamiltonianCycleFinder(graph, 8);

// Find cycle starting from vertex 0
List<Integer> cycle = finder.findHamiltonianCycle(0);

if (cycle != null) {
    System.out.println("Cycle found: " + cycle);
} else {
    System.out.println("No cycle found");
}

// Don't forget to shutdown
finder.shutdown();
```

## Implementation Details

### Parallelization Strategy

1. **Multi-level splitting**: Work is split at multiple levels of the search tree
2. **Proportional allocation**: Threads are distributed proportionally based on the number of branches
3. **RecursiveTask**: Each branch of the search tree is handled by a separate RecursiveTask
4. **ForkJoinPool**: Manages thread pool and work-stealing for efficient parallel execution

### Algorithm

The implementation uses backtracking DFS with parallel exploration:
- At each vertex, unvisited neighbors are identified
- If multiple threads are available, work is split among neighbors
- Threads are allocated proportionally (e.g., 8 threads, 3 neighbors → 3, 3, 2)
- When threads are exhausted, sequential search continues
- First cycle found is returned immediately

## Requirements

- Java 8 or higher (for ForkJoinPool and RecursiveTask)
- No external dependencies required
