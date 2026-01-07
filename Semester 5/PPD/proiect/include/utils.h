#ifndef UTILS_H
#define UTILS_H

#include "body.h"
#include <vector>
#include <string>

/**
 * Generate n random bodies for simulation.
 * @param n Number of bodies to generate
 * @param mass_min Minimum mass
 * @param mass_max Maximum mass
 * @param pos_min Minimum position coordinate
 * @param pos_max Maximum position coordinate
 * @param vel_min Minimum velocity coordinate
 * @param vel_max Maximum velocity coordinate
 * @return List of randomly generated bodies
 */
std::vector<Body> generateRandomBodies(
    int n,
    double mass_min = 1e20,
    double mass_max = 1e25,
    double pos_min = -1e6,
    double pos_max = 1e6,
    double vel_min = -1e3,
    double vel_max = 1e3
);

/**
 * Generate a simplified solar system with Sun and planets.
 * @return List of bodies representing the solar system
 */
std::vector<Body> generateSolarSystem();

/**
 * Save current state of bodies to a file.
 * @param bodies List of bodies to save
 * @param filename Output filename
 */
void saveSnapshot(const std::vector<Body>& bodies, const std::string& filename);

/**
 * Load bodies from a saved snapshot file.
 * @param filename Input filename
 * @return List of loaded bodies
 */
std::vector<Body> loadSnapshot(const std::string& filename);

#endif // UTILS_H

