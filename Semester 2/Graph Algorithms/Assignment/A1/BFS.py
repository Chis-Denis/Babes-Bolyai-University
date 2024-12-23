#1. Write a program that, given a directed graph and two vertices, finds a lowest length path between them, by using a forward breadth-first search from the starting vertex.



from collections import deque
def bfs_shortest_path(graph, start_vertex, end_vertex):

    q = deque()
    q.append(start_vertex)
    visited = set()
    visited.add(start_vertex)
    parent = dict()
    parent[start_vertex] = None

    while len(q) > 0:
        current_vertex = q.popleft()
        for neighbour in graph.neighbours_iterator(current_vertex):
            if neighbour not in visited:
                visited.add(neighbour)
                parent[neighbour] = current_vertex
                q.append(neighbour)
                if neighbour == end_vertex:
                    path = []
                    while neighbour is not None:
                        path.append(neighbour)
                        neighbour = parent[neighbour]
                    return path[::-1]
    return None