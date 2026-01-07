#ifndef DISTRIBUTED_SIMULATION_H
#define DISTRIBUTED_SIMULATION_H

#include "body.h"
#include "physics.h"
#include <vector>
#include <mpi.h>

/**
 * N-body simulation using MPI for distributed computation across nodes.
 * This implementation distributes bodies across MPI processes and uses
 * message passing to exchange position and velocity data for force calculations.
 */
class DistributedNBodySimulation {
public:
    /**
     * Initialize the distributed simulation.
     * @param bodies List of bodies to simulate (only used on rank 0)
     */
    DistributedNBodySimulation(std::vector<Body> bodies);
    
    /**
     * Destructor - finalize MPI if needed
     */
    ~DistributedNBodySimulation();
    
    /**
     * Perform one simulation step.
     * @param dt Time step
     */
    void step(double dt);
    
    /**
     * Run the simulation for a specified number of steps.
     * @param num_steps Number of simulation steps
     * @param dt Time step per iteration
     * @param verbose Whether to print progress (only on rank 0)
     */
    void run(int num_steps, double dt, bool verbose = true);
    
    /**
     * Get current system energy (only valid on rank 0).
     * @return Tuple of (kinetic_energy, potential_energy, total_energy)
     */
    std::tuple<double, double, double> getEnergy();
    
    /**
     * Get simulation statistics (only valid on rank 0).
     */
    struct Statistics {
        int num_bodies;
        int num_processes;
        double time_elapsed;
        int step_count;
        double kinetic_energy;
        double potential_energy;
        double total_energy;
        std::array<double, 3> center_of_mass;
    };
    
    Statistics getStatistics();
    
    /**
     * Get all bodies (only valid on rank 0).
     */
    std::vector<Body> getAllBodies();

private:
    MPI_Comm comm_;
    int rank_;
    int size_;
    std::vector<Body> local_bodies_;
    int num_bodies_;
    size_t start_idx_;
    size_t end_idx_;
    size_t local_num_;
    double time_elapsed_;
    int step_count_;
    
    /**
     * Gather all body positions and masses from all processes.
     * @return Pair of (positions array, masses array)
     */
    std::pair<std::vector<std::array<double, 3>>, std::vector<double>> 
    gatherAllPositionsMasses();
    
    /**
     * Calculate forces for local bodies.
     * @param all_positions All body positions
     * @param all_masses All body masses
     * @return List of force vectors for local bodies
     */
    std::vector<std::array<double, 3>> 
    calculateLocalForces(const std::vector<std::array<double, 3>>& all_positions,
                        const std::vector<double>& all_masses);
    
    /**
     * Gather all bodies from all processes (only on rank 0).
     */
    std::vector<Body> gatherAllBodies();
};

#endif // DISTRIBUTED_SIMULATION_H

