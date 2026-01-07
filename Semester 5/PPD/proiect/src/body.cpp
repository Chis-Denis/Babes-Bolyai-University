#include "body.h"
#include <sstream>
#include <iomanip>

Body::Body(const std::array<double, 3>& pos,
           const std::array<double, 3>& vel,
           double m,
           int body_id)
    : position(pos), velocity(vel), mass(m), id(body_id) {
}

std::string Body::toString() const {
    std::ostringstream oss;
    oss << "Body(id=" << id 
        << ", mass=" << std::scientific << std::setprecision(2) << mass
        << ", pos=(" << std::fixed << std::setprecision(2) 
        << position[0] << ", " << position[1] << ", " << position[2] << "))";
    return oss.str();
}

