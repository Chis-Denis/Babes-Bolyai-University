import numpy as np

class GeneticAlgorithm:
    """
    A Genetic Algorithm implementation for optimizing 2D functions.
    Supports both real-valued and binary encodings with various crossover methods.
    """
    def __init__(self, func, domain, encoding='real', crossover_type='arithmetic', mutation_rate=0.01, crossover_rate=0.7, population_size=50, generations=100, n_bits=16, blx_alpha=0.5):
        """
        Initialize the Genetic Algorithm with the given parameters.
        
        Args:
            func: The objective function to optimize
            domain: Dictionary with 'x' and 'y' bounds {'x': (min, max), 'y': (min, max)}
            encoding: 'real' or 'binary' encoding type
            crossover_type: Type of crossover ('arithmetic', 'blx', '1point', '2point')
            mutation_rate: Probability of mutation per gene
            crossover_rate: Probability of crossover
            population_size: Number of individuals in the population
            generations: Number of generations to evolve
            n_bits: Number of bits per variable (for binary encoding)
            blx_alpha: Alpha parameter for BLX-alpha crossover
        """
        self.func = func
        self.domain = domain
        self.encoding = encoding
        self.crossover_type = crossover_type
        self.mutation_rate = mutation_rate
        self.crossover_rate = crossover_rate
        self.population_size = population_size
        self.generations = generations
        self.n_bits = n_bits
        self.blx_alpha = blx_alpha
        self.population = None

    def initialize_population(self):
        """
        Initialize the population based on the chosen encoding type.
        For real encoding: Random points within the domain
        For binary encoding: Random binary strings
        """
        if self.encoding == 'real':
            x = np.random.uniform(self.domain['x'][0], self.domain['x'][1], self.population_size)
            y = np.random.uniform(self.domain['y'][0], self.domain['y'][1], self.population_size)
            self.population = np.stack([x, y], axis=1)
        elif self.encoding == 'binary':
            # Each individual: [x_bits, y_bits]
            self.population = np.random.randint(0, 2, (self.population_size, 2 * self.n_bits))

    def binary_to_real(self, bits):
        """
        Convert binary representation to real values.
        Splits the binary string into x and y components and decodes each.
        """
        x_bits = bits[:self.n_bits]
        y_bits = bits[self.n_bits:]
        x = self.decode_bits(x_bits, self.domain['x'])
        y = self.decode_bits(y_bits, self.domain['y'])
        return np.array([x, y])

    def decode_bits(self, bits, domain):
        """
        Convert binary string to float value within the given domain.
        Uses linear mapping from binary to real values.
        """
        value = int(''.join(str(int(b)) for b in bits), 2)
        max_val = 2 ** self.n_bits - 1
        return domain[0] + (domain[1] - domain[0]) * value / max_val

    def real_to_binary(self, real):
        """
        Convert real values to binary representation.
        Encodes x and y coordinates separately.
        """
        x, y = real
        x_bits = self.encode_float(x, self.domain['x'])
        y_bits = self.encode_float(y, self.domain['y'])
        return np.concatenate([x_bits, y_bits])

    def encode_float(self, value, domain):
        """
        Convert a float value to binary representation.
        Uses linear mapping from real to binary values.
        """
        max_val = 2 ** self.n_bits - 1
        norm = int((value - domain[0]) / (domain[1] - domain[0]) * max_val)
        return np.array(list(np.binary_repr(norm, width=self.n_bits))).astype(int)

    def evaluate_fitness(self):
        """
        Evaluate the fitness of all individuals in the population.
        For real encoding: Direct function evaluation
        For binary encoding: Convert to real values first
        """
        if self.encoding == 'real':
            return np.array([self.func(ind[0], ind[1]) for ind in self.population])
        elif self.encoding == 'binary':
            return np.array([self.func(*self.binary_to_real(ind)) for ind in self.population])

    def select_parents(self, fitness):
        """
        Tournament selection: Select two random individuals and return the better one.
        """
        idx = np.random.choice(np.arange(self.population_size), size=2, replace=False)
        return self.population[idx[0]] if fitness[idx[0]] < fitness[idx[1]] else self.population[idx[1]]

    def crossover(self, parent1, parent2):
        """
        Perform crossover between two parents to create two children.
        Different methods for real and binary encodings:
        - Real: arithmetic or BLX-alpha crossover
        - Binary: one-point or two-point crossover
        """
        if np.random.rand() > self.crossover_rate:
            return parent1.copy(), parent2.copy()
        if self.encoding == 'real':
            if self.crossover_type == 'arithmetic':
                alpha = np.random.rand()
                child1 = alpha * parent1 + (1 - alpha) * parent2
                child2 = alpha * parent2 + (1 - alpha) * parent1
                return child1, child2
            elif self.crossover_type == 'blx':
                cmin = np.minimum(parent1, parent2)
                cmax = np.maximum(parent1, parent2)
                I = cmax - cmin
                child1 = np.random.uniform(cmin - self.blx_alpha * I, cmax + self.blx_alpha * I)
                child2 = np.random.uniform(cmin - self.blx_alpha * I, cmax + self.blx_alpha * I)
                return child1, child2
        elif self.encoding == 'binary':
            if self.crossover_type == '1point':
                point = np.random.randint(1, len(parent1))
                child1 = np.concatenate([parent1[:point], parent2[point:]])
                child2 = np.concatenate([parent2[:point], parent1[point:]])
                return child1, child2
            elif self.crossover_type == '2point':
                pt1, pt2 = np.sort(np.random.choice(np.arange(1, len(parent1)), size=2, replace=False))
                child1 = parent1.copy()
                child2 = parent2.copy()
                child1[pt1:pt2] = parent2[pt1:pt2]
                child2[pt1:pt2] = parent1[pt1:pt2]
                return child1, child2
        return parent1.copy(), parent2.copy()

    def mutate(self, individual):
        """
        Apply mutation to an individual.
        For real encoding: Gaussian mutation
        For binary encoding: Bit flip mutation
        """
        if self.encoding == 'real':
            for i in range(2):
                if np.random.rand() < self.mutation_rate:
                    # Gaussian mutation
                    domain = self.domain['x'] if i == 0 else self.domain['y']
                    std = (domain[1] - domain[0]) * 0.1
                    individual[i] += np.random.normal(0, std)
                    individual[i] = np.clip(individual[i], domain[0], domain[1])
            return individual
        elif self.encoding == 'binary':
            for i in range(len(individual)):
                if np.random.rand() < self.mutation_rate:
                    individual[i] = 1 - individual[i]
            return individual
        return individual

    def run(self):
        """
        Run the genetic algorithm for the specified number of generations.
        Returns:
            best_fitness: List of best fitness values per generation
            best_individuals: List of best individuals per generation
        """
        self.initialize_population()
        best_fitness = []
        best_individuals = []
        
        for gen in range(self.generations):
            # Evaluate current population
            fitness = self.evaluate_fitness()
            
            # Create new population
            new_population = []
            for _ in range(self.population_size // 2):
                # Select parents and create offspring
                parent1 = self.select_parents(fitness)
                parent2 = self.select_parents(fitness)
                child1, child2 = self.crossover(parent1, parent2)
                child1 = self.mutate(child1)
                child2 = self.mutate(child2)
                new_population.extend([child1, child2])
            
            # Update population
            self.population = np.array(new_population)
            
            # Record best solution
            fitness = self.evaluate_fitness()
            best_idx = np.argmin(fitness)
            best_fitness.append(fitness[best_idx])
            best_individuals.append(self.population[best_idx])
        
        # Decode best individual if using binary encoding
        if self.encoding == 'binary':
            best_individuals = [self.binary_to_real(ind) for ind in best_individuals]
            
        return best_fitness, best_individuals 