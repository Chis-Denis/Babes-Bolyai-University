#ifndef BODY_H
#define BODY_H

#include <array>
#include <string>

/**
 * Represents a celestial body with position, velocity, and mass.
 */
class Body {
public:
    std::array<double, 3> position;  // 3D position vector (x, y, z)
    std::array<double, 3> velocity;  // 3D velocity vector (vx, vy, vz)
    double mass;                      // Mass of the body
    int id;                          // Unique identifier

    /**
     * Constructor
     * @param pos Initial position (x, y, z)
     * @param vel Initial velocity (vx, vy, vz)
     * @param m Mass of the body
     * @param body_id Unique identifier
     */
    Body(const std::array<double, 3>& pos = {0, 0, 0},
         const std::array<double, 3>& vel = {0, 0, 0},
         double m = 0.0,
         int body_id = 0);

    /**
     * String representation of the body
     */
    std::string toString() const;
};

#endif // BODY_H

