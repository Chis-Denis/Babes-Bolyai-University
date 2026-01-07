#include "distributed_simulation.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        std::cout << "============================================================" << std::endl;
        std::cout << "Distributed N-Body Simulation (MPI)" << std::endl;
        std::cout << "============================================================" << std::endl;
        std::cout << "Running on " << size << " processes" << std::endl;
        
        // Generate bodies (only on rank 0)
        std::cout << "\nGenerating bodies..." << std::endl;
        auto bodies = generateRandomBodies(20, 1e20, 1e25, -1e6, 1e6, -1e3, 1e3);
        std::cout << "Created " << bodies.size() << " bodies" << std::endl;
        
        // Create distributed simulation
        DistributedNBodySimulation simulation(bodies);
        
        std::cout << "\nRunning simulation..." << std::endl;
        simulation.run(100, 1.0, true);
        
        // Print statistics
        std::cout << "\n============================================================" << std::endl;
        std::cout << "Simulation Statistics" << std::endl;
        std::cout << "============================================================" << std::endl;
        auto stats = simulation.getStatistics();
        std::cout << "num_bodies: " << stats.num_bodies << std::endl;
        std::cout << "num_processes: " << stats.num_processes << std::endl;
        std::cout << "time_elapsed: " << std::scientific << std::setprecision(2) 
                  << stats.time_elapsed << std::endl;
        std::cout << "step_count: " << stats.step_count << std::endl;
        std::cout << "total_energy: " << stats.total_energy << std::endl;
        std::cout << "center_of_mass: (" << stats.center_of_mass[0] << ", "
                  << stats.center_of_mass[1] << ", " << stats.center_of_mass[2] << ")" << std::endl;
    } else {
        // Other ranks: create simulation (bodies will be received in constructor)
        std::vector<Body> empty_bodies;  // Will be populated by constructor
        DistributedNBodySimulation simulation(empty_bodies);
        simulation.run(100, 1.0, false);
    }
    
    MPI_Finalize();
    return 0;
}

