import numpy as np
from .functions import rastrigin, himmelblau, get_domains
from .ga import GeneticAlgorithm

import itertools

# Define the benchmark functions to be tested
functions = {
    'rastrigin': rastrigin,
    'himmelblau': himmelblau
}

# Get the domain constraints for each function
domains = get_domains()

# Define the encoding types and their corresponding crossover methods
encodings = ['real', 'binary']
crossovers = {
    'real': ['arithmetic', 'blx'],  # Real-valued encodings use arithmetic or BLX-alpha crossover
    'binary': ['1point', '2point']  # Binary encodings use one-point or two-point crossover
}

# Number of independent runs for each configuration
N_RUNS = 30

# Dictionary to store results for each configuration
RESULTS = {}

# Main experiment loop
# For each function, encoding type, and crossover method combination:
for fname, func in functions.items():
    for encoding in encodings:
        for crossover in crossovers[encoding]:
            key = (fname, encoding, crossover)
            RESULTS[key] = []
            
            # Perform multiple runs for statistical significance
            for run in range(N_RUNS):
                # Initialize and run the genetic algorithm
                ga = GeneticAlgorithm(
                    func=func,
                    domain=domains[fname],
                    encoding=encoding,
                    crossover_type=crossover,
                    mutation_rate=0.01,      # 1% chance of mutation per gene
                    crossover_rate=0.7,      # 70% chance of crossover
                    population_size=50,      # Population size of 50 individuals
                    generations=100          # Run for 100 generations
                )
                
                # Run the algorithm and collect results
                best_fitness, best_individuals = ga.run()
                RESULTS[key].append({
                    'best_fitness': best_fitness[-1],        # Final best fitness
                    'best_individual': best_individuals[-1],  # Final best solution
                    'history': best_fitness                  # Fitness history
                })
                print(f"Done: {key} run {run+1}/{N_RUNS}")

# Save results to a pickle file for later analysis
import pickle
with open('benchmark_ga/results.pkl', 'wb') as f:
    pickle.dump(RESULTS, f)
print('All experiments completed and results saved.') 