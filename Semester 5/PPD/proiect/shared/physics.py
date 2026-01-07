"""
Physics calculations for n-body simulation.
"""

import numpy as np
from typing import List, Tuple
from .body import Body


# Physical constants
G = 6.67430e-11  # Gravitational constant (m^3 kg^-1 s^-2)
SOFTENING = 1e-3  # Softening parameter to prevent division by zero


def calculate_force(body1: Body, body2: Body) -> np.ndarray:
    """
    Calculate gravitational force exerted by body2 on body1.
    
    Args:
        body1: The body experiencing the force
        body2: The body exerting the force
        
    Returns:
        3D force vector
    """
    # Calculate distance vector
    r = body2.position - body1.position
    
    # Calculate distance magnitude with softening
    distance_sq = np.dot(r, r) + SOFTENING ** 2
    distance = np.sqrt(distance_sq)
    
    # Calculate force magnitude (F = G * m1 * m2 / r^2)
    force_magnitude = G * body1.mass * body2.mass / distance_sq
    
    # Calculate force vector (direction is normalized r)
    force = force_magnitude * r / distance
    
    return force


def calculate_all_forces(bodies: List[Body]) -> List[np.ndarray]:
    """
    Calculate net gravitational force on each body from all other bodies.
    
    Args:
        bodies: List of all bodies in the simulation
        
    Returns:
        List of force vectors, one for each body
    """
    n = len(bodies)
    forces = [np.zeros(3, dtype=np.float64) for _ in range(n)]
    
    # Calculate pairwise forces
    for i in range(n):
        for j in range(i + 1, n):
            force = calculate_force(bodies[i], bodies[j])
            forces[i] += force
            forces[j] -= force  # Newton's third law
    
    return forces


def update_position(body: Body, force: np.ndarray, dt: float) -> None:
    """
    Update body position and velocity using Verlet integration.
    
    Args:
        body: The body to update
        force: Net force acting on the body
        dt: Time step
    """
    # Calculate acceleration (a = F / m)
    acceleration = force / body.mass
    
    # Update velocity (v = v + a * dt)
    body.velocity += acceleration * dt
    
    # Update position (x = x + v * dt)
    body.position += body.velocity * dt


def calculate_energy(bodies: List[Body]) -> Tuple[float, float]:
    """
    Calculate total kinetic and potential energy of the system.
    
    Args:
        bodies: List of all bodies in the simulation
        
    Returns:
        Tuple of (kinetic_energy, potential_energy)
    """
    n = len(bodies)
    kinetic = 0.0
    potential = 0.0
    
    for i in range(n):
        # Kinetic energy: KE = 0.5 * m * v^2
        v_sq = np.dot(bodies[i].velocity, bodies[i].velocity)
        kinetic += 0.5 * bodies[i].mass * v_sq
        
        # Potential energy: PE = -G * m1 * m2 / r (sum over all pairs)
        for j in range(i + 1, n):
            r = bodies[j].position - bodies[i].position
            distance = np.linalg.norm(r)
            if distance > 0:
                potential -= G * bodies[i].mass * bodies[j].mass / distance
    
    return kinetic, potential

