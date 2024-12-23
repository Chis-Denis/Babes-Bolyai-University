from Graph import Graph
from Exceptions import *


def lowest_cost_walk(graph, start_vertex, end_vertex):
    # Initialize a matrix to store the lowest cost walks
    INF = float('inf')
    n = graph.count_vertices()
    d = [[INF] * (n + 1) for _ in range(n)]

    d[start_vertex][0] = 0

    for k in range(1, n + 1):
        for v in graph.vertices_iterator():
            d[v][k] = d[v][k - 1]

        for u in graph.vertices_iterator():
            for v in graph.neighbours_iterator(u):
                if d[u][k - 1] != INF and d[u][k - 1] + graph.get_edge_cost(u, v) < d[v][k]:
                    d[v][k] = d[u][k - 1] + graph.get_edge_cost(u, v)

    for v in graph.vertices_iterator():
        if d[v][n] < 0:
            print("Negative cost cycle reachable from the starting vertex.")
            return

    min_cost = INF
    for k in range(n + 1):
        min_cost = min(min_cost, d[end_vertex][k])

    if min_cost == INF:
        print("No path found from start_vertex to end_vertex.")
    else:
        print("Lowest cost walk from", start_vertex, "to", end_vertex, ":", min_cost)
