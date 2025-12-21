import java.util.*;

public class Graph {
    private int numVertices;
    private List<List<Integer>> adjacencyList;
    
    public Graph(int numVertices) {
        this.numVertices = numVertices;
        this.adjacencyList = new ArrayList<>();
        for (int i = 0; i < numVertices; i++) {
            adjacencyList.add(new ArrayList<>());
        }
    }
    
    public void addEdge(int u, int v) {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
            adjacencyList.get(u).add(v);
        }
    }
    
    public List<Integer> getNeighbors(int vertex) {
        return adjacencyList.get(vertex);
    }
    
    public int getOutDegree(int vertex) {
        return adjacencyList.get(vertex).size();
    }
    
    public int getNumVertices() {
        return numVertices;
    }
    
    public boolean hasEdge(int u, int v) {
        return adjacencyList.get(u).contains(v);
    }
}

