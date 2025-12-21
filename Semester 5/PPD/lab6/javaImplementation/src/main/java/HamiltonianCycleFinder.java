import java.util.*;
import java.util.concurrent.ForkJoinPool;

public class HamiltonianCycleFinder {
    private Graph graph;
    private int numThreads;
    private ForkJoinPool pool;
    
    public HamiltonianCycleFinder(Graph graph, int numThreads) {
        this.graph = graph;
        this.numThreads = numThreads;
        this.pool = new ForkJoinPool(numThreads);
    }
    
    /**
     * Find a Hamiltonian cycle starting from the specified vertex.
     * startVertex The vertex to start the search from
     * return A list representing the Hamiltonian cycle, or null if none exists
     */
    public List<Integer> findHamiltonianCycle(int startVertex) {
        if (startVertex < 0 || startVertex >= graph.getNumVertices()) {
            throw new IllegalArgumentException("Invalid start vertex");
        }
        
        List<Integer> initialPath = new ArrayList<>();
        initialPath.add(startVertex);
        Set<Integer> visited = new HashSet<>();
        visited.add(startVertex);
        
        HamiltonianCycleTask task = new HamiltonianCycleTask(
            graph, startVertex, initialPath, visited, numThreads
        );
        
        return pool.invoke(task);
    }
    
    public void shutdown() {
        pool.shutdown();
    }
}

