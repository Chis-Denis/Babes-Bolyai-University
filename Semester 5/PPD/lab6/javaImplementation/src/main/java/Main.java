import java.util.*;

public class Main {
    public static void main(String[] args) {
        // Example 1: Simple graph with a Hamiltonian cycle
        System.out.println("=== Example 1: Graph with Hamiltonian Cycle ===");
        Graph graph1 = createExampleGraph1();
        int numThreads = 8;
        HamiltonianCycleFinder finder = new HamiltonianCycleFinder(graph1, numThreads);
        
        int startVertex = 0;
        System.out.println("Searching for Hamiltonian cycle starting from vertex " + startVertex);
        System.out.println("Using " + numThreads + " threads");
        
        long startTime = System.currentTimeMillis();
        List<Integer> cycle = finder.findHamiltonianCycle(startVertex);
        long endTime = System.currentTimeMillis();
        
        if (cycle != null) {
            System.out.println("Hamiltonian cycle found: " + cycle);
            System.out.println("Time taken: " + (endTime - startTime) + " ms");
        } else {
            System.out.println("No Hamiltonian cycle found");
            System.out.println("Time taken: " + (endTime - startTime) + " ms");
        }
        
        finder.shutdown();
        
        System.out.println("\n=== Example 2: Larger Graph ===");
        Graph graph2 = createExampleGraph2();
        HamiltonianCycleFinder finder2 = new HamiltonianCycleFinder(graph2, numThreads);
        
        startTime = System.currentTimeMillis();
        cycle = finder2.findHamiltonianCycle(0);
        endTime = System.currentTimeMillis();
        
        if (cycle != null) {
            System.out.println("Hamiltonian cycle found: " + cycle);
            System.out.println("Time taken: " + (endTime - startTime) + " ms");
        } else {
            System.out.println("No Hamiltonian cycle found");
            System.out.println("Time taken: " + (endTime - startTime) + " ms");
        }
        
        finder2.shutdown();
    }
    
    private static Graph createExampleGraph1() {
        Graph graph = new Graph(4);
        // Cycle: 0 -> 1 -> 2 -> 3 -> 0
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 0);
        
        graph.addEdge(0, 2);
        graph.addEdge(1, 3);
        graph.addEdge(2, 0);
        graph.addEdge(3, 1);
        
        return graph;
    }
    
    private static Graph createExampleGraph2() {
        Graph graph = new Graph(6);
        // Create a cycle: 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 0
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 4);
        graph.addEdge(4, 5);
        graph.addEdge(5, 0);
        
        graph.addEdge(0, 2);
        graph.addEdge(1, 3);
        graph.addEdge(2, 4);
        graph.addEdge(3, 5);
        graph.addEdge(4, 0);
        graph.addEdge(5, 1);
        
        return graph;
    }
}

