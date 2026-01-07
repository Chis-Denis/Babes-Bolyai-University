#include "utils.h"
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Body> generateRandomBodies(
    int n,
    double mass_min,
    double mass_max,
    double pos_min,
    double pos_max,
    double vel_min,
    double vel_max) {
    
    std::vector<Body> bodies;
    bodies.reserve(n);
    
    // Use fixed seed for reproducibility
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> mass_dist(mass_min, mass_max);
    std::uniform_real_distribution<double> pos_dist(pos_min, pos_max);
    std::uniform_real_distribution<double> vel_dist(vel_min, vel_max);
    
    for (int i = 0; i < n; ++i) {
        std::array<double, 3> position = {
            pos_dist(gen),
            pos_dist(gen),
            pos_dist(gen)
        };
        std::array<double, 3> velocity = {
            vel_dist(gen),
            vel_dist(gen),
            vel_dist(gen)
        };
        double mass = mass_dist(gen);
        
        bodies.emplace_back(position, velocity, mass, i);
    }
    
    return bodies;
}

std::vector<Body> generateSolarSystem() {
    std::vector<Body> bodies;
    
    // Sun (at origin, stationary)
    bodies.emplace_back(
        std::array<double, 3>{0.0, 0.0, 0.0},
        std::array<double, 3>{0.0, 0.0, 0.0},
        1.989e30,  // Sun's mass in kg
        0
    );
    
    // Earth
    bodies.emplace_back(
        std::array<double, 3>{1.496e11, 0.0, 0.0},  // 1 AU
        std::array<double, 3>{0.0, 2.978e4, 0.0},  // Orbital velocity
        5.972e24,
        1
    );
    
    // Mars
    bodies.emplace_back(
        std::array<double, 3>{2.279e11, 0.0, 0.0},  // ~1.5 AU
        std::array<double, 3>{0.0, 2.407e4, 0.0},
        6.39e23,
        2
    );
    
    // Jupiter
    bodies.emplace_back(
        std::array<double, 3>{7.785e11, 0.0, 0.0},  // ~5.2 AU
        std::array<double, 3>{0.0, 1.307e4, 0.0},
        1.898e27,
        3
    );
    
    return bodies;
}

void saveSnapshot(const std::vector<Body>& bodies, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    file << bodies.size() << "\n";
    for (const auto& body : bodies) {
        file << body.id << " " << body.mass << " "
             << body.position[0] << " " << body.position[1] << " " << body.position[2] << " "
             << body.velocity[0] << " " << body.velocity[1] << " " << body.velocity[2] << "\n";
    }
    file.close();
}

std::vector<Body> loadSnapshot(const std::string& filename) {
    std::vector<Body> bodies;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading" << std::endl;
        return bodies;
    }
    
    int n;
    file >> n;
    bodies.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        int body_id;
        double mass;
        std::array<double, 3> position;
        std::array<double, 3> velocity;
        
        file >> body_id >> mass
             >> position[0] >> position[1] >> position[2]
             >> velocity[0] >> velocity[1] >> velocity[2];
        
        bodies.emplace_back(position, velocity, mass, body_id);
    }
    
    file.close();
    return bodies;
}

