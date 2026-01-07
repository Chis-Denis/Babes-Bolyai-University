#include "threaded_simulation.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "============================================================" << std::endl;
    std::cout << "Simple N-Body Simulation (Threaded)" << std::endl;
    std::cout << "============================================================" << std::endl;
    
    // Generate bodies
    std::cout << "\nGenerating bodies..." << std::endl;
    auto bodies = generateRandomBodies(10, 1e20, 1e25, -1e6, 1e6, -1e3, 1e3);
    
    std::cout << "Created " << bodies.size() << " bodies" << std::endl;
    
    // Create simulation
    ThreadedNBodySimulation simulation(bodies, 4);
    
    // Run simulation
    std::cout << "\nRunning simulation..." << std::endl;
    simulation.run(100, 1.0, true);
    
    // Print statistics
    std::cout << "\n============================================================" << std::endl;
    std::cout << "Simulation Statistics" << std::endl;
    std::cout << "============================================================" << std::endl;
    auto stats = simulation.getStatistics();
    std::cout << "num_bodies: " << stats.num_bodies << std::endl;
    std::cout << "num_threads: " << stats.num_threads << std::endl;
    std::cout << "time_elapsed: " << std::scientific << std::setprecision(2) 
              << stats.time_elapsed << std::endl;
    std::cout << "step_count: " << stats.step_count << std::endl;
    std::cout << "kinetic_energy: " << stats.kinetic_energy << std::endl;
    std::cout << "potential_energy: " << stats.potential_energy << std::endl;
    std::cout << "total_energy: " << stats.total_energy << std::endl;
    std::cout << "center_of_mass: (" << stats.center_of_mass[0] << ", "
              << stats.center_of_mass[1] << ", " << stats.center_of_mass[2] << ")" << std::endl;
    
    return 0;
}

