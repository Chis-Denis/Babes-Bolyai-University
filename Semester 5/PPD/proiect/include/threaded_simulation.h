#ifndef THREADED_SIMULATION_H
#define THREADED_SIMULATION_H

#include "body.h"
#include "physics.h"
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <atomic>

/**
 * N-body simulation using threads for parallel force calculations.
 * This implementation uses std::async to parallelize the computation
 * of gravitational forces between bodies.
 */
class ThreadedNBodySimulation {
public:
    /**
     * Initialize the simulation.
     * @param bodies List of bodies to simulate
     * @param num_threads Number of threads to use (default: hardware concurrency)
     */
    ThreadedNBodySimulation(std::vector<Body> bodies, int num_threads = 0);
    
    /**
     * Perform one simulation step.
     * @param dt Time step
     */
    void step(double dt);
    
    /**
     * Run the simulation for a specified number of steps.
     * @param num_steps Number of simulation steps
     * @param dt Time step per iteration
     * @param verbose Whether to print progress
     */
    void run(int num_steps, double dt, bool verbose = true);
    
    /**
     * Get current system energy.
     * @return Tuple of (kinetic_energy, potential_energy, total_energy)
     */
    std::tuple<double, double, double> getEnergy() const;
    
    /**
     * Get simulation statistics.
     */
    struct Statistics {
        int num_bodies;
        int num_threads;
        double time_elapsed;
        int step_count;
        double kinetic_energy;
        double potential_energy;
        double total_energy;
        std::array<double, 3> center_of_mass;
    };
    
    Statistics getStatistics() const;
    
    /**
     * Get the bodies (const reference).
     */
    const std::vector<Body>& getBodies() const { return bodies_; }

private:
    std::vector<Body> bodies_;
    int num_bodies_;
    int num_threads_;
    double time_elapsed_;
    int step_count_;
    
    /**
     * Calculate forces for a chunk of bodies.
     * @param start_idx Starting index
     * @param end_idx Ending index (exclusive)
     * @return List of (index, force_vector) pairs
     */
    std::vector<std::pair<size_t, std::array<double, 3>>> 
    calculateForcesChunk(size_t start_idx, size_t end_idx) const;
    
    /**
     * Calculate all forces in parallel using threads.
     * @return List of force vectors, one for each body
     */
    std::vector<std::array<double, 3>> calculateForcesParallel() const;
};

#endif // THREADED_SIMULATION_H

