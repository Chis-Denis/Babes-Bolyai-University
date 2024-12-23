from Graph import *
from Exceptions import *
from Assig3 import lowest_cost_walk
from BFS import bfs_shortest_path


class UI:
    def __init__(self, type_of_graph):
        self.type_of_graph = type_of_graph
        self.graph = None
        graph = Graph()

    def empty_graph(self):
        self.graph = self.type_of_graph()
        print("Now you have an empty graph!")

    def n_vertices_graph(self):
        vertices_number = input("How many vertices do you need: ")

        try:
            self.graph = self.type_of_graph(int(vertices_number))
            print("You got yourself a graph with {0} vertices!".format(vertices_number))
        except Exception as e:
            print(e)

    def random_graph(self):
        vertices_number = int(input("How many vertices would you like?\n> "))
        edges_number = int(input("How many edges would you like?\n> "))

        if edges_number > vertices_number * vertices_number:
            print("It is not possible to construct a graph with that many edges!")
            self.empty_graph()
        else:
            try:
                self.graph = self.type_of_graph(int(vertices_number),
                                                int(edges_number))
                print("Random Graph: Generated!")
            except Exception as e:
                print(e)


    def add_vertex(self):

        new_vertex = input("Type the vertex you wish to add: ")
        try:
            self.graph.add_vertex(int(new_vertex))
            print("The vertex {0} has been added.".format(new_vertex))
        except Exception as e:
            print(e)

    def add_edge(self):
        vertex1 = (input("Type the first vertex of the edge: "))
        vertex2 = input("Type the second vertex of the edge: ")
        cost = input("Type the cost of the edge: ")

        try:
            self.graph.add_edge(int(vertex1),
                                int(vertex2),
                                int(cost))
            print("The edge {0} {1} has been added.".format(vertex1, vertex2))

        except Exception as e:
            print(e)

    def remove_vertex(self):
        vertex_to_remove = input("Type the vertex you would like to remove: ")

        try:
            self.graph.remove_vertex(int(vertex_to_remove))
            print("The given vertex has been removed.")
        except Exception as e:
            print(e)

    def remove_edge(self):
        vertex1 = input("Type the first vertex of the edge: ")
        vertex2 = input("Type the second vertex of the edge: ")

        try:
            self.graph.remove_edge(int(vertex1), int(vertex2))
            print("The edge has been removed.")
        except Exception as e:
            print(e)

    def change_edge(self):
        vertex1 = input("Type the first vertex of the edge: ")
        vertex2 = input("Type the second vertex of the edge: ")
        cost = input("Type the cost of the edge: ")
        try:
            self.graph.set_edge_cost(int(vertex1), int(vertex2), int(cost))
        except Exception as e:
            print(e)

    def print_edge(self):
        vertex1 = input("Type the first vertex of the edge: ")
        vertex2 = input("Type the second vertex of the edge: ")
        try:
            print("The cost of the given edge is {0}.".format(self.graph.get_edge_cost(int(vertex1), int(vertex2))))
        except Exception as e:
            print(e)

    def in_degree(self):
        n = input("Type the vertex for which you wish to find the in degree: ")
        try:
            print(self.graph.in_degree(int(n)))
        except Exception as e:
            print(e)

    def out_degree(self):
        n = input("Type the vertex for which you wish to find the out degree: ")
        try:
            print(self.graph.out_degree(int(n)))
        except Exception as e:
            print(e)

    def count_vertices(self):
        print("There is a grand total of {0} vertices.".format(self.graph.count_vertices()))

    def count_edges(self):
        print("There is a grand total of {0} edges.".format(self.graph.count_edges()))

    def is_vertex(self):
        n = input("Type the vertex you wish to check: ")
        try:
            if self.graph.is_vertex(int(n)):
                print("The given vertex belongs to the graph!")
            else:
                print("The given vertex does not belong to the graph :(")
        except Exception as e:
            print(e)

    def is_edge(self):
        vertex1 = input("Type the first vertex of the edge: ")
        vertex2 = input("Type the second vertex of the edge: ")
        try:
            if self.graph.is_edge(int(vertex1), int(vertex2)):
                print("The given edge does exist in the graph!")
            else:
                print("The edge does not exist in the graph :(")
        except Exception as e:
            print(e)

    def print_vertex_list(self):
        for node in self.graph.vertices_iterator():
            print(node, end=" ")
        print()

    def print_neighbour_list(self):
        n = input("Type the vertex you wish to find neighbours for: ")
        try:
            anyone = False
            for node in self.graph.neighbours_iterator(int(n)):
                print(node, end=" ")
                anyone = True
            if not anyone:
                print("Vertex {0} has no neighbours.".format(n))
            else:
                print()
        except Exception as e:
            print(e)

    def print_transpose_list(self):
        n = input("Type the vertex you wish to find inbound neighbours for: ")
        try:
            anyone = False
            for node in self.graph.transposed_iterator(int(n)):
                print(node, end=" ")
                anyone = True
            if not anyone:
                print("Vertex {0} has no inbound neighbours.".format(n))
            else:
                print()
        except Exception as e:
            print(e)

    def print_edges(self):
        anyone = False
        for triple in self.graph.edges_iterator():
            print("Vertices {0}, {1} and cost {2}.".format(triple[0], triple[1], triple[2]))
            anyone = True
        if not anyone:
            print("There are no edges in the graph...")

    def read_file(self):
        path = input("Type the file from which you wish to read: ")
        try:
            self.graph = read_file(path)
            print("The graph has been read from the file!")
        except Exception as e:
            print(e)

    def write_file(self):
        path = input("Type the file you wish to write to: ")
        try:
            write_file(path, self.graph)
            print("The graph has been saved to the file!")
        except Exception as e:
            print(e)

    def generate_graph(self):
        print("1. Generate an empty graph")
        print("2. Generate a graph with n vertices")
        print("3. Generate a graph with n vertices and m random edges")
        cmd = input("> ")

        commands = {
            "1": self.empty_graph,
            "2": self.n_vertices_graph,
            "3": self.random_graph
        }

        if cmd in commands:
            commands[cmd]()
        elif cmd == "0":
            print("Alright!")
        else:
            print("Invalid choice.")

    def vertex_operations(self):
        print("1. Add vertex")
        print("2. Remove Vertex")
        print("3. Count Vertices")
        print("4. Check If The Vertex Belongs To The Graph")
        print("5. Print Vertex List")
        print("6. Print in-degree")
        print("7. Print out-degree")

        cmd = input("> ")

        commands = {
            "1": self.add_vertex,
            "2": self.remove_vertex,
            "3": self.count_vertices,
            "4": self.is_vertex,
            "5": self.print_vertex_list,
            "6": self.in_degree,
            "7": self.out_degree,
        }

        if cmd in commands:
            commands[cmd]()
        elif cmd == "0":
            print("Alright!")
        else:
            print("Invalid choice.")

    def edge_operations(self):
        print("1. Add Edge")
        print("2. Remove Edge")
        print("3. Change Edge")
        print("4. Print Edge")
        print("5. Count Edges")
        print("6. Check If The Edge Belongs To The Graph")
        print("7. Print The Neighbour List")
        print("8. Print The Transposed List")
        print("9. Print The List Of Edges")

        cmd = input("> ")

        commands_chain = {
            "1": self.add_edge,
            "2": self.remove_edge,
            "3": self.change_edge,
            "4": self.print_edge,
            "5": self.count_edges,
            "6": self.is_edge,
            "7": self.print_neighbour_list,
            "8": self.print_transpose_list,
            "9": self.print_edges
        }

        if cmd in commands_chain:
            commands_chain[cmd]()
        elif cmd == "0":
            print("Alright!")
        else:
            print("Invalid choice.")


    def find_shortest_path(self):

        start_vertex = input("Type the start vertex: ")
        end_vertex = input("Type the end vertex: ")
        try:
            path = bfs_shortest_path(self.graph, int(start_vertex), int(end_vertex))
            if path is None:
                print("There is no path between the two vertices.")
            else:
                print("The shortest path between the two vertices is: ", end="")
                for vertex in path:
                    print(vertex, end=" ")
                print()
        except Exception as e:
            print(e)

    def find_lowest_cost_walk(self):
        start_vertex = input("Enter the start vertex: ")
        end_vertex = input("Enter the end vertex: ")
        try:
            start_vertex = int(start_vertex)
            end_vertex = int(end_vertex)
            # Call the function from another file
            lowest_cost_walk(self.graph, start_vertex, end_vertex)
        except ValueError:
            print("Please enter valid integer vertex IDs.")
        except VertexError as ve:
            print(ve)
        except EdgeError as ee:
            print(ee)

    def find_hamiltonian_cycle(self):
        try:
            cycle = self.graph.find_hamiltonian_cycle()
            if cycle:
                print("Hamiltonian Cycle found: ", " -> ".join(map(str, cycle)))
            else:
                print("No Hamiltonian Cycle exists in the graph.")
        except Exception as e:
            print(e)

    def create_random_hamiltonian_graph(self):
        try:
            self.graph = self.graph.create_random_hamiltonian_graph()
            print("Random Hamiltonian Graph created!")
        except Exception as e:
            print(e)

    def minimum_cost_spanning_tree(self):



        """"start_node = int(input("Please enter the starting node: "))
        minimum_cost_spanning_tree = Graph.find_minimum_spanning_tree_using_prim(graph, start_node)
        print("The cost of minimum spanning tree is " +
              str(minimum_cost_spanning_tree[0]))
        print("The edges of the minimum spanning tree are: ")
        edges = minimum_cost_spanning_tree[1]
        for i in range(len(edges)):
            if i != edges[i]:
                print(str(i) + " " + str(edges[i]))"""""

    def start(self):
        self.empty_graph()
        commands = {"1": self.generate_graph,
                    "2": self.vertex_operations,
                    "3": self.edge_operations,
                    "5": self.read_file,
                    "6": self.write_file,
                    "7": self.find_shortest_path,
                    "8": self.find_lowest_cost_walk,
                    "9": self.minimum_cost_spanning_tree,
                    "10": self.find_hamiltonian_cycle,
                    "11": self.create_random_hamiltonian_graph}
        while True:
            print("___________________________________________________________")
            print("Choose one of the following commands: ")
            print("1. Generate a graph")
            print("2. Vertex operations")
            print("3. Edge operations")
            print("5. Reads the graph from a file")
            print("6. Writes the graph to a file")
            print("7. Find the shortest path between two vertices")
            print("8. Find the lowest cost walk between two vertices")
            print("9. Find the minimum cost spanning tree using Prim's algorithm")
            print("10. Find a Hamiltonian cycle")
            print("11. Create a random Hamiltonian graph")
            print("0. Exit")
            cmd = input("> ")
            if cmd in commands:
                commands[cmd]()
            elif cmd == "0":
                print("Sad to see you go, goodbye!")
                break
            else:
                print("Invalid choice.")

UI(Graph).start()