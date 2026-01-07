#include "physics.h"
#include <cmath>
#include <algorithm>

std::array<double, 3> calculateForce(const Body& body1, const Body& body2) {
    // Calculate distance vector
    std::array<double, 3> r;
    for (int i = 0; i < 3; ++i) {
        r[i] = body2.position[i] - body1.position[i];
    }
    
    // Calculate distance magnitude with softening
    double distance_sq = 0.0;
    for (int i = 0; i < 3; ++i) {
        distance_sq += r[i] * r[i];
    }
    distance_sq += SOFTENING * SOFTENING;
    double distance = std::sqrt(distance_sq);
    
    // Calculate force magnitude (F = G * m1 * m2 / r^2)
    double force_magnitude = G * body1.mass * body2.mass / distance_sq;
    
    // Calculate force vector (direction is normalized r)
    std::array<double, 3> force;
    for (int i = 0; i < 3; ++i) {
        force[i] = force_magnitude * r[i] / distance;
    }
    
    return force;
}

std::vector<std::array<double, 3>> calculateAllForces(const std::vector<Body>& bodies) {
    size_t n = bodies.size();
    std::vector<std::array<double, 3>> forces(n, {0.0, 0.0, 0.0});
    
    // Calculate pairwise forces
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            std::array<double, 3> force = calculateForce(bodies[i], bodies[j]);
            // Add force to body i
            for (int k = 0; k < 3; ++k) {
                forces[i][k] += force[k];
            }
            // Subtract force from body j (Newton's third law)
            for (int k = 0; k < 3; ++k) {
                forces[j][k] -= force[k];
            }
        }
    }
    
    return forces;
}

void updatePosition(Body& body, const std::array<double, 3>& force, double dt) {
    // Calculate acceleration (a = F / m)
    std::array<double, 3> acceleration;
    for (int i = 0; i < 3; ++i) {
        acceleration[i] = force[i] / body.mass;
    }
    
    // Update velocity (v = v + a * dt)
    for (int i = 0; i < 3; ++i) {
        body.velocity[i] += acceleration[i] * dt;
    }
    
    // Update position (x = x + v * dt)
    for (int i = 0; i < 3; ++i) {
        body.position[i] += body.velocity[i] * dt;
    }
}

std::pair<double, double> calculateEnergy(const std::vector<Body>& bodies) {
    size_t n = bodies.size();
    double kinetic = 0.0;
    double potential = 0.0;
    
    for (size_t i = 0; i < n; ++i) {
        // Kinetic energy: KE = 0.5 * m * v^2
        double v_sq = 0.0;
        for (int j = 0; j < 3; ++j) {
            v_sq += bodies[i].velocity[j] * bodies[i].velocity[j];
        }
        kinetic += 0.5 * bodies[i].mass * v_sq;
        
        // Potential energy: PE = -G * m1 * m2 / r (sum over all pairs)
        for (size_t j = i + 1; j < n; ++j) {
            std::array<double, 3> r;
            for (int k = 0; k < 3; ++k) {
                r[k] = bodies[j].position[k] - bodies[i].position[k];
            }
            double distance_sq = 0.0;
            for (int k = 0; k < 3; ++k) {
                distance_sq += r[k] * r[k];
            }
            double distance = std::sqrt(distance_sq);
            if (distance > 0) {
                potential -= G * bodies[i].mass * bodies[j].mass / distance;
            }
        }
    }
    
    return {kinetic, potential};
}

