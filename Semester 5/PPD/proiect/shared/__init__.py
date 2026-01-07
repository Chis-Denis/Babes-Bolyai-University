"""
Shared utilities for n-body simulation implementations.
"""

from .body import Body
from .physics import calculate_force, update_position, calculate_energy

__all__ = ['Body', 'calculate_force', 'update_position', 'calculate_energy']

