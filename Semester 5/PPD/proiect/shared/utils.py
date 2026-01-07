"""
Utility functions for n-body simulation.
"""

import numpy as np
from typing import List, Tuple
from .body import Body


def generate_random_bodies(n: int, 
                          mass_range: Tuple[float, float] = (1e20, 1e25),
                          position_range: Tuple[float, float] = (-1e6, 1e6),
                          velocity_range: Tuple[float, float] = (-1e3, 1e3)) -> List[Body]:
    """
    Generate n random bodies for simulation.
    
    Args:
        n: Number of bodies to generate
        mass_range: (min, max) mass range
        position_range: (min, max) position range for each coordinate
        velocity_range: (min, max) velocity range for each coordinate
        
    Returns:
        List of randomly generated bodies
    """
    bodies = []
    np.random.seed(42)  # For reproducibility
    
    for i in range(n):
        mass = np.random.uniform(mass_range[0], mass_range[1])
        position = np.random.uniform(position_range[0], position_range[1], 3)
        velocity = np.random.uniform(velocity_range[0], velocity_range[1], 3)
        bodies.append(Body(position, velocity, mass, i))
    
    return bodies


def generate_solar_system() -> List[Body]:
    """
    Generate a simplified solar system with Sun and planets.
    
    Returns:
        List of bodies representing the solar system
    """
    bodies = []
    
    # Sun (at origin, stationary)
    bodies.append(Body(
        position=(0.0, 0.0, 0.0),
        velocity=(0.0, 0.0, 0.0),
        mass=1.989e30,  # Sun's mass in kg
        body_id=0
    ))
    
    # Earth
    bodies.append(Body(
        position=(1.496e11, 0.0, 0.0),  # 1 AU
        velocity=(0.0, 2.978e4, 0.0),  # Orbital velocity
        mass=5.972e24,
        body_id=1
    ))
    
    # Mars
    bodies.append(Body(
        position=(2.279e11, 0.0, 0.0),  # ~1.5 AU
        velocity=(0.0, 2.407e4, 0.0),
        mass=6.39e23,
        body_id=2
    ))
    
    # Jupiter
    bodies.append(Body(
        position=(7.785e11, 0.0, 0.0),  # ~5.2 AU
        velocity=(0.0, 1.307e4, 0.0),
        mass=1.898e27,
        body_id=3
    ))
    
    return bodies


def save_snapshot(bodies: List[Body], filename: str) -> None:
    """
    Save current state of bodies to a file.
    
    Args:
        bodies: List of bodies to save
        filename: Output filename
    """
    with open(filename, 'w') as f:
        f.write(f"{len(bodies)}\n")
        for body in bodies:
            f.write(f"{body.id} {body.mass} "
                   f"{body.position[0]} {body.position[1]} {body.position[2]} "
                   f"{body.velocity[0]} {body.velocity[1]} {body.velocity[2]}\n")


def load_snapshot(filename: str) -> List[Body]:
    """
    Load bodies from a saved snapshot file.
    
    Args:
        filename: Input filename
        
    Returns:
        List of loaded bodies
    """
    bodies = []
    with open(filename, 'r') as f:
        n = int(f.readline().strip())
        for i in range(n):
            line = f.readline().split()
            body_id = int(line[0])
            mass = float(line[1])
            position = (float(line[2]), float(line[3]), float(line[4]))
            velocity = (float(line[5]), float(line[6]), float(line[7]))
            bodies.append(Body(position, velocity, mass, body_id))
    
    return bodies

