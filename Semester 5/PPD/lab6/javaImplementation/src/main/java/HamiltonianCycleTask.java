import java.util.*;
import java.util.concurrent.RecursiveTask;

public class HamiltonianCycleTask extends RecursiveTask<List<Integer>> {
    private Graph graph;
    private int startVertex;
    private List<Integer> currentPath;
    private Set<Integer> visited;
    private int availableThreads;
    
    public HamiltonianCycleTask(Graph graph, int startVertex, 
                                List<Integer> currentPath, 
                                Set<Integer> visited, 
                                int availableThreads) {
        this.graph = graph;
        this.startVertex = startVertex;
        this.currentPath = new ArrayList<>(currentPath);
        this.visited = new HashSet<>(visited);
        this.availableThreads = availableThreads;
    }
    
    @Override
    protected List<Integer> compute() {
        int currentVertex = currentPath.isEmpty() ? startVertex : currentPath.get(currentPath.size() - 1);
        
        // Base case: if we've visited all vertices, check if we can return to start
        if (currentPath.size() == graph.getNumVertices()) {
            if (graph.hasEdge(currentVertex, startVertex)) {
                List<Integer> cycle = new ArrayList<>(currentPath);
                cycle.add(startVertex);
                return cycle;
            }
            return null; // No cycle found
        }
        
        // Get neighbors that havent been visited yet
        List<Integer> neighbors = new ArrayList<>();
        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (!visited.contains(neighbor)) {
                neighbors.add(neighbor);
            }
        }
        
        // If no unvisited neighbors, backtrack
        if (neighbors.isEmpty()) {
            return null;
        }
        
        // If we have only one thread or one neighbor, do sequential search
        if (availableThreads <= 1 || neighbors.size() == 1) {
            return sequentialSearch(currentVertex, neighbors);
        }
        
        // Parallel search: split work among neighbors
        List<HamiltonianCycleTask> tasks = new ArrayList<>();
        int totalNeighbors = neighbors.size();
        
        // Distribute threads among neighbors
        int threadsPerNeighbor = availableThreads / totalNeighbors;
        int extraThreads = availableThreads % totalNeighbors;
        
        for (int i = 0; i < neighbors.size(); i++) {
            int neighbor = neighbors.get(i);
            int threadsForThisNeighbor = threadsPerNeighbor;
            if (i < extraThreads) {
                threadsForThisNeighbor++;
            }
            
            List<Integer> newPath = new ArrayList<>(currentPath);
            newPath.add(neighbor);
            Set<Integer> newVisited = new HashSet<>(visited);
            newVisited.add(neighbor);
            
            HamiltonianCycleTask task = new HamiltonianCycleTask(
                graph, startVertex, newPath, newVisited, threadsForThisNeighbor
            );
            tasks.add(task);
        }
        
        // Fork all tasks
        for (HamiltonianCycleTask task : tasks) {
            task.fork();
        }
        
        // Join results - return first non-null result (first cycle found)
        for (HamiltonianCycleTask task : tasks) {
            List<Integer> result = task.join();
            if (result != null) {
                // Cancel other tasks if possible (they'll complete but we ignore results)
                return result;
            }
        }
        
        return null; // No cycle found in any branch
    }

    // Sequential search when we can't parallelize further.
    private List<Integer> sequentialSearch(int currentVertex, List<Integer> neighbors) {
        for (int neighbor : neighbors) {
            List<Integer> newPath = new ArrayList<>(currentPath);
            newPath.add(neighbor);
            Set<Integer> newVisited = new HashSet<>(visited);
            newVisited.add(neighbor);
            
            List<Integer> result = searchRecursive(newPath, newVisited);
            if (result != null) {
                return result;
            }
        }
        return null;
    }
    
    // Recursive backtracking search.
    private List<Integer> searchRecursive(List<Integer> path, Set<Integer> visited) {
        if (path.size() == graph.getNumVertices()) {
            int lastVertex = path.get(path.size() - 1);
            if (graph.hasEdge(lastVertex, startVertex)) {
                List<Integer> cycle = new ArrayList<>(path);
                cycle.add(startVertex);
                return cycle;
            }
            return null;
        }
        
        int currentVertex = path.get(path.size() - 1);
        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (!visited.contains(neighbor)) {
                path.add(neighbor);
                visited.add(neighbor);
                
                List<Integer> result = searchRecursive(path, visited);
                if (result != null) {
                    return result;
                }
                
                // Backtrack
                path.remove(path.size() - 1);
                visited.remove(neighbor);
            }
        }
        
        return null;
    }
}

