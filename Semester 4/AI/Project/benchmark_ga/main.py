# Main entry point for the Genetic Algorithm benchmark program
# This script is responsible for visualizing the results of the benchmark functions

from .visualization import visualize_all

if __name__ == "__main__":
    print("Visualizing benchmark functions...")
    visualize_all()  # Generate and display visualizations for all benchmark results 