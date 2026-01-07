"""
Threaded n-body simulation using concurrent.futures for parallel computation.
"""

import numpy as np
from concurrent.futures import ThreadPoolExecutor, as_completed
from typing import List, Tuple
import time

from shared.body import Body
from shared.physics import calculate_force, update_position, calculate_energy


class ThreadedNBodySimulation:
    """
    N-body simulation using threads for parallel force calculations.
    
    This implementation uses ThreadPoolExecutor to parallelize the computation
    of gravitational forces between bodies.
    """
    
    def __init__(self, bodies: List[Body], num_threads: int = None):
        """
        Initialize the simulation.
        
        Args:
            bodies: List of bodies to simulate
            num_threads: Number of threads to use (default: CPU count)
        """
        self.bodies = bodies
        self.num_bodies = len(bodies)
        
        if num_threads is None:
            import os
            num_threads = os.cpu_count() or 4
        
        self.num_threads = num_threads
        self.time_elapsed = 0.0
        self.step_count = 0
    
    def _calculate_forces_chunk(self, indices: List[int]) -> List[Tuple[int, np.ndarray]]:
        """
        Calculate forces for a chunk of bodies.
        
        Args:
            indices: List of body indices to process
            
        Returns:
            List of (index, force_vector) tuples
        """
        forces = []
        for i in indices:
            force = np.zeros(3, dtype=np.float64)
            for j in range(self.num_bodies):
                if i != j:
                    force += calculate_force(self.bodies[i], self.bodies[j])
            forces.append((i, force))
        return forces
    
    def _calculate_forces_parallel(self) -> List[np.ndarray]:
        """
        Calculate all forces in parallel using threads.
        
        Returns:
            List of force vectors, one for each body
        """
        # Divide work among threads
        chunk_size = max(1, self.num_bodies // self.num_threads)
        chunks = []
        
        for i in range(0, self.num_bodies, chunk_size):
            chunk_indices = list(range(i, min(i + chunk_size, self.num_bodies)))
            chunks.append(chunk_indices)
        
        # Calculate forces in parallel
        forces = [np.zeros(3, dtype=np.float64) for _ in range(self.num_bodies)]
        
        with ThreadPoolExecutor(max_workers=self.num_threads) as executor:
            futures = [executor.submit(self._calculate_forces_chunk, chunk) 
                      for chunk in chunks]
            
            for future in as_completed(futures):
                chunk_results = future.result()
                for idx, force in chunk_results:
                    forces[idx] = force
        
        return forces
    
    def step(self, dt: float) -> None:
        """
        Perform one simulation step.
        
        Args:
            dt: Time step
        """
        # Calculate forces in parallel
        forces = self._calculate_forces_parallel()
        
        # Update positions and velocities
        for i, body in enumerate(self.bodies):
            update_position(body, forces[i], dt)
        
        self.time_elapsed += dt
        self.step_count += 1
    
    def run(self, num_steps: int, dt: float, verbose: bool = True) -> None:
        """
        Run the simulation for a specified number of steps.
        
        Args:
            num_steps: Number of simulation steps
            dt: Time step per iteration
            verbose: Whether to print progress
        """
        start_time = time.time()
        
        for step in range(num_steps):
            self.step(dt)
            
            if verbose and (step + 1) % max(1, num_steps // 10) == 0:
                kinetic, potential = calculate_energy(self.bodies)
                total_energy = kinetic + potential
                print(f"Step {step + 1}/{num_steps} | "
                      f"Time: {self.time_elapsed:.2e}s | "
                      f"Energy: {total_energy:.2e} J")
        
        elapsed_time = time.time() - start_time
        
        if verbose:
            print(f"\nSimulation completed in {elapsed_time:.2f} seconds")
            print(f"Average time per step: {elapsed_time / num_steps * 1000:.2f} ms")
            print(f"Using {self.num_threads} threads")
    
    def get_energy(self) -> Tuple[float, float, float]:
        """
        Get current system energy.
        
        Returns:
            Tuple of (kinetic_energy, potential_energy, total_energy)
        """
        kinetic, potential = calculate_energy(self.bodies)
        return kinetic, potential, kinetic + potential
    
    def get_statistics(self) -> dict:
        """
        Get simulation statistics.
        
        Returns:
            Dictionary with simulation statistics
        """
        kinetic, potential, total = self.get_energy()
        
        # Calculate center of mass
        total_mass = sum(body.mass for body in self.bodies)
        com = np.zeros(3)
        for body in self.bodies:
            com += body.position * body.mass
        com /= total_mass
        
        return {
            'num_bodies': self.num_bodies,
            'num_threads': self.num_threads,
            'time_elapsed': self.time_elapsed,
            'step_count': self.step_count,
            'kinetic_energy': kinetic,
            'potential_energy': potential,
            'total_energy': total,
            'center_of_mass': com.tolist()
        }

