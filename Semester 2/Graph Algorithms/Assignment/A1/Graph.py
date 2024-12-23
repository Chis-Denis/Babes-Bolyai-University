from Exceptions import *
from random import randrange
from copy import deepcopy
from queue import PriorityQueue


def read_file(file_path):
    file = open(file_path, "r")

    if file == None:
        print("Nothing to see here!")
        return

    n, m = map(int, file.readline().split())
    g = Graph(n)
    for _ in range(m):
        vertex1, vertex2, edge_cost = map(int, file.readline().split())
        g.add_edge(vertex1, vertex2, edge_cost)
    file.close()
    return g


def write_file(file_path, graph):
    file = open(file_path, "w")

    file.write("{0} {1}\n".format(graph.count_vertices(), graph.count_edges()))

    for node in graph.vertices_iterator():
        for neighbour in graph.neighbours_iterator(node):
            file.write("{0} {1} {2}\n".format(node,
                                              neighbour,
                                              graph.get_edge_cost(node, neighbour)))
    file.close()


def random_graph(vertices_number, edges_number):
    """
    Function that generates a random graph with a given number of vertices and edges.
    """
    g = Graph()

    for i in range(vertices_number):
        g.add_vertex(i)
    for j in range(edges_number):
        vertex1 = randrange(vertices_number)
        vertex2 = randrange(vertices_number)
        while g.is_edge(vertex1, vertex2):
            vertex1 = randrange(vertices_number)
            vertex2 = randrange(vertices_number)
        g.add_edge(vertex1, vertex2, randrange(100))


class Graph:
    def __init__(self, n=0, m=0):
        self._vertices = set()
        self._neighbours = dict()
        self._transposed = dict()
        self._cost = dict()

        for i in range(n):
            self.add_vertex(i)
        for j in range(m):
            vertex1 = randrange(n)
            vertex2 = randrange(n)
            while self.is_edge(vertex1, vertex2):
                vertex1 = randrange(n)
                vertex2 = randrange(n)
            self.add_edge(vertex1, vertex2, randrange(100))

    def vertices_iterator(self):
        """
        Input: -
        Returns an iterator to the set of vertices.
        """
        for vertex in self._vertices:
            yield vertex

    def neighbours_iterator(self, vertex):
        """
        Input: vertex - a vertex from the graph.
        Returns an iterator to the set of (outbound) neighbours of a vertex.
        """
        if not self.is_vertex(vertex):
            raise VertexError("Invalid vertex.")

        for neighbour in self._neighbours[vertex]:
            yield neighbour

    def transposed_iterator(self, vertex):
        """
        Input: vertex - a vertex from the graph.
        Returns an iterator to the set of (inbound) neighbours of a vertex.
        """
        if not self.is_vertex(vertex):
            raise VertexError("Invalid vertex.")
        for neighbour in self._transposed[vertex]:
            yield neighbour

    def edges_iterator(self):
        """
        Input: -
        Returns an iterator to the set of edges.
        """
        for key, value in self._cost.items():
            yield key[0], key[1], value

    def is_vertex(self, vertex):
        """
        Input: vertex - a vertex from the graph
        Returns True if vertex belongs to the graph.
        """
        return vertex in self._vertices

    def is_edge(self, vertex1, vertex2):
        """
        Input: vertex1, vertex2 - vertices of the supposed edge.
        Returns True if the edge from vertex1 to vertex2 belongs to the graph.
        """
        return vertex1 in self._neighbours and vertex2 in self._neighbours[vertex1]

    def count_vertices(self):
        """
        Input: -
        Returns the number of vertices in the graph.
        """
        return len(self._vertices)

    def count_edges(self):
        """
        Input: -
        Returns the number of edges in the graph.
        """
        return len(self._cost)

    def in_degree(self, vertex):
        """
        Input: vertex - a vertex from the graph
        Returns the number of edges with the endpoint vertex.
        """
        if vertex not in self._transposed:
            raise VertexError("The specified vertex does not exist.")
        return len(self._transposed[vertex])

    def out_degree(self, vertex):
        """
        Returns the number of edges with the start point vertex.
        """
        if vertex not in self._neighbours:
            raise VertexError("The specified vertex does not exist.")
        return len(self._neighbours[vertex])

    def get_edge_cost(self, vertex1, vertex2):
        """
        Returns the cost of an edge if it exists.
        """
        if (vertex1, vertex2) not in self._cost:
            raise EdgeError("The specified edge does not exist.")
        return self._cost[(vertex1, vertex2)]

    def set_edge_cost(self, vertex1, vertex2, new_cost):
        """
        Sets the cost of an edge in the graph if it exists.
        """
        if (vertex1, vertex2) not in self._cost:
            raise EdgeError("The specified edge does not exist.")
        self._cost[(vertex1, vertex2)] = new_cost

    def add_vertex(self, vertex):
        """
        Adds a vertex to the graph.
        """
        if self.is_vertex(vertex):
            raise VertexError("Cannot add a vertex which already exists.")
        self._vertices.add(vertex)
        self._neighbours[vertex] = set()
        self._transposed[vertex] = set()

    def add_edge(self, vertex1, vertex2, edge_cost=0):
        """
        Adds an edge to the graph, with a given cost; if no cost is given,
        the default is 0. The method raises an exception if the edge already exists,
        and it works by adding the edge to the set of neighbours of the first vertex.
        """
        if self.is_edge(vertex1, vertex2):
            raise EdgeError("The specified edge already exists")
        if not self.is_vertex(vertex1) or not self.is_vertex(vertex2):
            raise EdgeError("The vertices on the edge do not exist.")
        self._neighbours[vertex1].add(vertex2)
        self._transposed[vertex2].add(vertex1)
        self._cost[(vertex1, vertex2)] = edge_cost

    def remove_edge(self, vertex1, vertex2):
        """
        Removes an edge from the graph, if it exists.
        The method raises an exception if the edge does not exist.
        It works by removing the cost of the edge, and removing the second vertex
        from the set of neighbours of the first vertex. Then, it removes the first
        vertex from the set of transposed neighbours of the second vertex.

        """
        if not self.is_edge(vertex1, vertex2):
            raise EdgeError("The specified edge does not exist.")

        del self._cost[(vertex1, vertex2)]
        self._neighbours[vertex1].remove(vertex2)
        self._transposed[vertex2].remove(vertex1)

    def remove_vertex(self, vertex):
        """
        Removes a vertex from the graph, if it exists. It works by using
        an auxiliary list to store the neighbours of the vertex, and then
        to remove the edges between the vertex and its neighbours. It then
        goes through the list of transposed neighbours, and removes the edges
        from the transposed neighbours to the vertex. Finally, it removes the
        entry from the dictionary of neighbours and transposed neighbours, and
        finally removes the vertex from the set of vertices.

        """
        if not self.is_vertex(vertex):
            raise VertexError("Cannot remove a vertex which doesn't exist.")

        to_remove = []

        for node in self._neighbours[vertex]:
            to_remove.append(node)

        for node in to_remove:
            self.remove_edge(vertex, node)
        to_remove = []

        for node in self._transposed[vertex]:
            to_remove.append(node)

        for node in to_remove:
            self.remove_edge(node, vertex)

        del self._neighbours[vertex]
        del self._transposed[vertex]
        self._vertices.remove(vertex)

    def copy(self):
        """
        Returns a deepcopy of the graph.
        """
        return deepcopy(self)

    def find_minimum_spanning_tree_using_prim(self, start_node):

        dad = [-1] * self.count_vertices()
        dad[start_node] = start_node

        q = PriorityQueue()
        q.put((0, start_node, start_node))

        minimum_cost = 0

        for i in range(self.count_vertices()):
            current_pair = ()
            while not q.empty():
                current_pair = q.get()
                if dad[current_pair[1]] == -1:
                    break
            current_node = current_pair[1]
            dad[current_node] = current_pair[2]
            minimum_cost += current_pair[0]
            for neighbour in self._neighbours[current_node]:
                if dad[neighbour] == -1:
                    q.put((self._cost[(current_node, neighbour)], neighbour, current_node))

        return minimum_cost, dad

    def is_valid_vertex(self, v, pos, path):
        if v not in self._neighbours[path[pos - 1]]:
            return False
        if v in path:
            return False
        return True

    def ham_cycle_util(self, path, pos):
        if pos == len(self._vertices):
            return path[0] in self._neighbours[path[pos - 1]]

        for v in self._vertices:
            if self.is_valid_vertex(v, pos, path):
                path[pos] = v

                if self.ham_cycle_util(path, pos + 1):
                    return True

                path[pos] = -1

        return False

    def find_hamiltonian_cycle(self):
        path = [-1] * len(self._vertices)
        start_vertex = next(iter(self._vertices))
        path[0] = start_vertex

        if not self.ham_cycle_util(path, 1):
            return None
        else:
            return path + [path[0]]

    def create_random_hamiltonian_graph(self):
        for i in range(len(self._vertices)):
            for j in range(i + 1, len(self._vertices)):
                self.add_edge(i, j, randrange(100))

