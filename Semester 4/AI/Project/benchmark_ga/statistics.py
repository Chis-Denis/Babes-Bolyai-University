"""
Statistical analysis and visualization of Genetic Algorithm benchmark results.
This script analyzes the performance of different GA configurations and generates
comparative visualizations.
"""

import pickle
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import ttest_ind, f_oneway

# Load the experimental results from the pickle file
with open('benchmark_ga/results.pkl', 'rb') as f:
    RESULTS = pickle.load(f)

# Calculate summary statistics for each configuration
# For each combination of function, encoding, and crossover method:
summary = {}
for key, runs in RESULTS.items():
    # Extract the best fitness values from all runs
    fitnesses = [r['best_fitness'] for r in runs]
    summary[key] = {
        'mean': np.mean(fitnesses),    # Average performance
        'std': np.std(fitnesses),      # Standard deviation (measure of consistency)
        'all': fitnesses               # All fitness values for statistical tests
    }
    print(f"{key}: mean={summary[key]['mean']:.4f}, std={summary[key]['std']:.4f}")

# Perform statistical tests to compare different configurations

# Example 1: T-test between real-arithmetic and binary-1point for rastrigin
# This tests if there's a significant difference between these two configurations
fa = summary[('rastrigin', 'real', 'arithmetic')]['all']
fb = summary[('rastrigin', 'binary', '1point')]['all']
t_stat, p_val = ttest_ind(fa, fb)
print(f"T-test rastrigin real-arithmetic vs binary-1point: t={t_stat:.4f}, p={p_val:.4f}")

# Example 2: ANOVA test for all rastrigin configurations
# This tests if there are significant differences between any of the rastrigin configurations
rastrigin_groups = [summary[k]['all'] for k in summary if k[0] == 'rastrigin']
f_stat, p_val = f_oneway(*rastrigin_groups)
print(f"ANOVA rastrigin configs: F={f_stat:.4f}, p={p_val:.4f}")

# Generate boxplots to visualize the distribution of results
# Create separate plots for each benchmark function
for fname in ['rastrigin', 'himmelblau']:
    plt.figure(figsize=(8, 5))
    labels = []
    data = []
    
    # Collect data for all configurations of the current function
    for encoding in ['real', 'binary']:
        # Different crossover methods for each encoding type
        for crossover in (['arithmetic', 'blx'] if encoding == 'real' else ['1point', '2point']):
            key = (fname, encoding, crossover)
            labels.append(f"{encoding}-{crossover}")
            data.append(summary[key]['all'])
    
    # Create boxplot
    plt.boxplot(data, labels=labels)
    plt.title(f"Best Fitness Distribution: {fname}")
    plt.ylabel('Best Fitness')
    plt.xticks(rotation=30)  # Rotate labels for better readability
    plt.tight_layout()
    
    # Save the plot to a file
    plt.savefig(f'benchmark_ga/{fname}_fitness_boxplot.png')
    plt.show() 