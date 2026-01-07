"""
Body class representing a celestial body in the n-body simulation.
"""

import numpy as np
from typing import Tuple


class Body:
    """
    Represents a celestial body with position, velocity, and mass.
    
    Attributes:
        position: 3D position vector (x, y, z)
        velocity: 3D velocity vector (vx, vy, vz)
        mass: Mass of the body
        id: Unique identifier for the body
    """
    
    def __init__(self, position: Tuple[float, float, float], 
                 velocity: Tuple[float, float, float], 
                 mass: float, 
                 body_id: int = 0):
        """
        Initialize a body.
        
        Args:
            position: Initial position (x, y, z)
            velocity: Initial velocity (vx, vy, vz)
            mass: Mass of the body
            body_id: Unique identifier
        """
        self.position = np.array(position, dtype=np.float64)
        self.velocity = np.array(velocity, dtype=np.float64)
        self.mass = float(mass)
        self.id = body_id
    
    def __repr__(self) -> str:
        """String representation of the body."""
        return (f"Body(id={self.id}, mass={self.mass:.2e}, "
                f"pos=({self.position[0]:.2f}, {self.position[1]:.2f}, {self.position[2]:.2f}))")
    
    def copy(self) -> 'Body':
        """Create a deep copy of this body."""
        return Body(
            position=self.position.copy(),
            velocity=self.velocity.copy(),
            mass=self.mass,
            body_id=self.id
        )

