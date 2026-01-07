#include "threaded_simulation.h"
#include "utils.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Testing n-body simulation..." << std::endl;
    
    // Test 1: Generate bodies
    std::cout << "Test 1: Generating bodies..." << std::endl;
    auto bodies = generateRandomBodies(5);
    assert(bodies.size() == 5);
    std::cout << "  ✓ Generated " << bodies.size() << " bodies" << std::endl;
    
    // Test 2: Create simulation
    std::cout << "Test 2: Creating simulation..." << std::endl;
    ThreadedNBodySimulation simulation(bodies, 2);
    std::cout << "  ✓ Simulation created" << std::endl;
    
    // Test 3: Run simulation
    std::cout << "Test 3: Running simulation..." << std::endl;
    simulation.run(10, 1.0, false);
    auto stats = simulation.getStatistics();
    assert(stats.num_bodies == 5);
    assert(stats.step_count == 10);
    std::cout << "  ✓ Simulation completed" << std::endl;
    
    // Test 4: Check energy
    std::cout << "Test 4: Checking energy..." << std::endl;
    auto [kinetic, potential, total] = simulation.getEnergy();
    std::cout << "  ✓ Energy calculated: " << total << " J" << std::endl;
    
    std::cout << "\nAll tests passed! ✓" << std::endl;
    return 0;
}

