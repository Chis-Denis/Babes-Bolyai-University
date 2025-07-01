# Benchmark Optimization Functions Using Genetic Algorithms

## Objective
Explore benchmark optimization functions and apply Genetic Algorithms (GAs) to find their minima. Two multimodal functions (Rastrigin and Himmelblau) were selected, implemented, and optimized using a configurable GA. Statistical performance analysis was conducted for different GA configurations.

## Selected Functions
- **Rastrigin function**: 2D, domain [-5.12, 5.12]^2
- **Himmelblau's function**: 2D, domain [-5, 5]^2

## Function Visualizations
- 2D contour and 3D surface plots were generated for both functions (see figures).

## Genetic Algorithm Configurations
- **Encodings**: Binary, Real-valued
- **Crossover types**:
  - Binary: 1-point, 2-point
  - Real: Arithmetic, BLX-α
- **Adjustable parameters**: mutation rate, crossover rate, population size, generations

## Experimental Setup
- All combinations of encoding and crossover were tested for both functions.
- Each configuration was run 30 times independently.
- Fixed number of fitness evaluations per run.

## Results Summary
- For each configuration, best fitness, mean, and standard deviation were recorded.
- Statistical tests (t-test, ANOVA) were used to compare configurations.
- Boxplots of best fitness distributions are provided for each function.

## Statistical Analysis
- Example t-test: real-arithmetic vs binary-1point (Rastrigin)
- Example ANOVA: all Rastrigin configurations

## Conclusions
- The results indicate which GA configuration performs best for each function.
- Statistical significance is assessed using p-values from t-tests and ANOVA.

## Figures
- See `rastrigin_fitness_boxplot.png` and `himmelblau_fitness_boxplot.png` for visual summaries.

---

*This report was generated as part of the Benchmark Optimization Assignment.* 