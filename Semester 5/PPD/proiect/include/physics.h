#ifndef PHYSICS_H
#define PHYSICS_H

#include "body.h"
#include <vector>

// Physical constants
constexpr double G = 6.67430e-11;      // Gravitational constant (m^3 kg^-1 s^-2)
constexpr double SOFTENING = 1e-3;      // Softening parameter to prevent division by zero

/**
 * Calculate gravitational force exerted by body2 on body1.
 * @param body1 The body experiencing the force
 * @param body2 The body exerting the force
 * @return 3D force vector
 */
std::array<double, 3> calculateForce(const Body& body1, const Body& body2);

/**
 * Calculate net gravitational force on each body from all other bodies.
 * @param bodies List of all bodies in the simulation
 * @return List of force vectors, one for each body
 */
std::vector<std::array<double, 3>> calculateAllForces(const std::vector<Body>& bodies);

/**
 * Update body position and velocity using Verlet integration.
 * @param body The body to update (modified in place)
 * @param force Net force acting on the body
 * @param dt Time step
 */
void updatePosition(Body& body, const std::array<double, 3>& force, double dt);

/**
 * Calculate total kinetic and potential energy of the system.
 * @param bodies List of all bodies in the simulation
 * @return Pair of (kinetic_energy, potential_energy)
 */
std::pair<double, double> calculateEnergy(const std::vector<Body>& bodies);

#endif // PHYSICS_H

