#include "threaded_simulation.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "============================================================" << std::endl;
    std::cout << "Solar System N-Body Simulation (Threaded)" << std::endl;
    std::cout << "============================================================" << std::endl;
    
    // Generate solar system
    std::cout << "\nGenerating solar system..." << std::endl;
    auto bodies = generateSolarSystem();
    
    std::cout << "Created " << bodies.size() << " celestial bodies:" << std::endl;
    for (const auto& body : bodies) {
        std::cout << "  - " << body.toString() << std::endl;
    }
    
    // Create simulation
    ThreadedNBodySimulation simulation(bodies, 4);
    
    // Run simulation
    std::cout << "\nRunning simulation..." << std::endl;
    int num_steps = 1000;
    double dt = 3600.0;  // 1 hour per step
    
    for (int step = 0; step < num_steps; ++step) {
        simulation.step(dt);
        
        if ((step + 1) % 100 == 0) {
            auto [kinetic, potential, total] = simulation.getEnergy();
            std::cout << "Step " << (step + 1) << "/" << num_steps 
                      << " | Time: " << std::fixed << std::setprecision(2) 
                      << simulation.getStatistics().time_elapsed / 86400.0 << " days"
                      << " | Energy: " << std::scientific << total << " J" << std::endl;
        }
    }
    
    // Print final statistics
    std::cout << "\n============================================================" << std::endl;
    std::cout << "Final Statistics" << std::endl;
    std::cout << "============================================================" << std::endl;
    auto stats = simulation.getStatistics();
    std::cout << "num_bodies: " << stats.num_bodies << std::endl;
    std::cout << "num_threads: " << stats.num_threads << std::endl;
    std::cout << "time_elapsed: " << std::scientific << std::setprecision(2) 
              << stats.time_elapsed << std::endl;
    std::cout << "step_count: " << stats.step_count << std::endl;
    std::cout << "total_energy: " << stats.total_energy << std::endl;
    
    return 0;
}

