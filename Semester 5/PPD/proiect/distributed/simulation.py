"""
Distributed n-body simulation using MPI for parallel computation across nodes.
"""

import numpy as np
from typing import List, Tuple
import time

try:
    from mpi4py import MPI
    MPI_AVAILABLE = True
except ImportError:
    MPI_AVAILABLE = False
    print("Warning: mpi4py not available. Install with: pip install mpi4py")

from shared.body import Body
from shared.physics import calculate_force, update_position, calculate_energy


class DistributedNBodySimulation:
    """
    N-body simulation using MPI for distributed computation.
    
    This implementation distributes bodies across MPI processes and uses
    message passing to exchange position and velocity data for force calculations.
    """
    
    def __init__(self, bodies: List[Body]):
        """
        Initialize the distributed simulation.
        
        Args:
            bodies: List of bodies to simulate (only used on rank 0)
        """
        if not MPI_AVAILABLE:
            raise RuntimeError("mpi4py is required for distributed simulation")
        
        self.comm = MPI.COMM_WORLD
        self.rank = self.comm.Get_rank()
        self.size = self.comm.Get_size()
        
        # Distribute bodies across processes
        if self.rank == 0:
            self.all_bodies = bodies
            self.num_bodies = len(bodies)
        else:
            self.all_bodies = None
            self.num_bodies = None
        
        # Broadcast number of bodies
        self.num_bodies = self.comm.bcast(self.num_bodies, root=0)
        
        # Calculate which bodies this process is responsible for
        bodies_per_process = self.num_bodies // self.size
        remainder = self.num_bodies % self.size
        
        self.start_idx = self.rank * bodies_per_process + min(self.rank, remainder)
        self.end_idx = self.start_idx + bodies_per_process + (1 if self.rank < remainder else 0)
        self.local_num = self.end_idx - self.start_idx
        
        # Initialize local bodies
        if self.rank == 0:
            self.local_bodies = self.all_bodies[self.start_idx:self.end_idx]
            # Send bodies to other processes
            for proc in range(1, self.size):
                proc_start = proc * bodies_per_process + min(proc, remainder)
                proc_end = proc_start + bodies_per_process + (1 if proc < remainder else 0)
                proc_bodies = self.all_bodies[proc_start:proc_end]
                self.comm.send(proc_bodies, dest=proc)
        else:
            self.local_bodies = self.comm.recv(source=0)
        
        self.time_elapsed = 0.0
        self.step_count = 0
    
    def _gather_all_positions_masses(self) -> Tuple[np.ndarray, np.ndarray]:
        """
        Gather all body positions and masses from all processes.
        
        Returns:
            Tuple of (positions array, masses array)
        """
        # Prepare local data
        local_positions = np.array([body.position for body in self.local_bodies])
        local_masses = np.array([body.mass for body in self.local_bodies])
        
        # Flatten positions for communication
        local_positions_flat = local_positions.flatten()
        
        # Calculate sizes for each process
        local_pos_size = self.local_num * 3
        local_mass_size = self.local_num
        
        # Gather sizes from all processes
        pos_sizes = self.comm.allgather(local_pos_size)
        mass_sizes = self.comm.allgather(local_mass_size)
        
        # Calculate displacements
        pos_displacements = [0]
        mass_displacements = [0]
        for i in range(self.size - 1):
            pos_displacements.append(pos_displacements[-1] + pos_sizes[i])
            mass_displacements.append(mass_displacements[-1] + mass_sizes[i])
        
        # Prepare receive buffers
        all_positions_flat = np.zeros(self.num_bodies * 3, dtype=np.float64)
        all_masses_flat = np.zeros(self.num_bodies, dtype=np.float64)
        
        # Gather all positions
        self.comm.Allgatherv(
            [local_positions_flat, MPI.DOUBLE],
            [all_positions_flat, pos_sizes, pos_displacements, MPI.DOUBLE]
        )
        
        # Gather all masses
        self.comm.Allgatherv(
            [local_masses, MPI.DOUBLE],
            [all_masses_flat, mass_sizes, mass_displacements, MPI.DOUBLE]
        )
        
        # Reshape positions
        all_positions = all_positions_flat.reshape(self.num_bodies, 3)
        all_masses = all_masses_flat
        
        return all_positions, all_masses
    
    def _calculate_local_forces(self, all_positions: np.ndarray, 
                                all_masses: np.ndarray) -> List[np.ndarray]:
        """
        Calculate forces for local bodies.
        
        Args:
            all_positions: All body positions
            all_masses: All body masses
            
        Returns:
            List of force vectors for local bodies
        """
        local_forces = []
        
        for local_body in self.local_bodies:
            force = np.zeros(3, dtype=np.float64)
            
            for j in range(self.num_bodies):
                if local_body.id != j:
                    # Create temporary body for force calculation
                    temp_body = Body(
                        position=all_positions[j],
                        velocity=(0, 0, 0),
                        mass=all_masses[j],
                        body_id=j
                    )
                    force += calculate_force(local_body, temp_body)
            
            local_forces.append(force)
        
        return local_forces
    
    def step(self, dt: float) -> None:
        """
        Perform one simulation step.
        
        Args:
            dt: Time step
        """
        # Gather all positions and masses
        all_positions, all_masses = self._gather_all_positions_masses()
        
        # Calculate forces for local bodies
        local_forces = self._calculate_local_forces(all_positions, all_masses)
        
        # Update local bodies
        for i, body in enumerate(self.local_bodies):
            update_position(body, local_forces[i], dt)
        
        # Positions are already gathered in _gather_all_positions_masses for next iteration
        # No need to gather again here since we'll gather at the start of next step
        
        self.time_elapsed += dt
        self.step_count += 1
    
    def run(self, num_steps: int, dt: float, verbose: bool = True) -> None:
        """
        Run the simulation for a specified number of steps.
        
        Args:
            num_steps: Number of simulation steps
            dt: Time step per iteration
            verbose: Whether to print progress (only on rank 0)
        """
        start_time = time.time()
        
        for step in range(num_steps):
            self.step(dt)
            
            if verbose and self.rank == 0 and (step + 1) % max(1, num_steps // 10) == 0:
                # Gather all bodies for energy calculation
                all_bodies = self._gather_all_bodies()
                kinetic, potential = calculate_energy(all_bodies)
                total_energy = kinetic + potential
                print(f"Step {step + 1}/{num_steps} | "
                      f"Time: {self.time_elapsed:.2e}s | "
                      f"Energy: {total_energy:.2e} J | "
                      f"Processes: {self.size}")
        
        elapsed_time = time.time() - start_time
        
        if verbose and self.rank == 0:
            print(f"\nSimulation completed in {elapsed_time:.2f} seconds")
            print(f"Average time per step: {elapsed_time / num_steps * 1000:.2f} ms")
            print(f"Using {self.size} MPI processes")
    
    def _gather_all_bodies(self) -> List[Body]:
        """
        Gather all bodies from all processes (only on rank 0).
        
        Returns:
            Complete list of all bodies
        """
        if self.rank == 0:
            all_bodies = self.local_bodies.copy()
            
            # Receive bodies from other processes
            for proc in range(1, self.size):
                proc_bodies = self.comm.recv(source=proc)
                all_bodies.extend(proc_bodies)
            
            # Sort by ID to maintain order
            all_bodies.sort(key=lambda b: b.id)
            return all_bodies
        else:
            self.comm.send(self.local_bodies, dest=0)
            return []
    
    def get_energy(self) -> Tuple[float, float, float]:
        """
        Get current system energy (only valid on rank 0).
        
        Returns:
            Tuple of (kinetic_energy, potential_energy, total_energy)
        """
        if self.rank == 0:
            all_bodies = self._gather_all_bodies()
            kinetic, potential = calculate_energy(all_bodies)
            return kinetic, potential, kinetic + potential
        else:
            self._gather_all_bodies()  # Participate in gather
            return 0.0, 0.0, 0.0
    
    def get_statistics(self) -> dict:
        """
        Get simulation statistics (only valid on rank 0).
        
        Returns:
            Dictionary with simulation statistics
        """
        if self.rank != 0:
            return {}
        
        all_bodies = self._gather_all_bodies()
        kinetic, potential, total = self.get_energy()
        
        # Calculate center of mass
        total_mass = sum(body.mass for body in all_bodies)
        com = np.zeros(3)
        for body in all_bodies:
            com += body.position * body.mass
        com /= total_mass
        
        return {
            'num_bodies': self.num_bodies,
            'num_processes': self.size,
            'time_elapsed': self.time_elapsed,
            'step_count': self.step_count,
            'kinetic_energy': kinetic,
            'potential_energy': potential,
            'total_energy': total,
            'center_of_mass': com.tolist()
        }
    
    def get_all_bodies(self) -> List[Body]:
        """
        Get all bodies (only valid on rank 0).
        
        Returns:
            Complete list of all bodies
        """
        if self.rank == 0:
            return self._gather_all_bodies()
        else:
            self._gather_all_bodies()  # Participate in gather
            return []

