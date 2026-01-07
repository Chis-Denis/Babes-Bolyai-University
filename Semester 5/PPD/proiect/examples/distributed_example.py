"""
Example demonstrating the distributed MPI n-body simulation.

To run this example:
    mpirun -np 4 python examples/distributed_example.py
"""

from shared.utils import generate_random_bodies
from distributed.simulation import DistributedNBodySimulation


def main():
    """Run a distributed n-body simulation."""
    from mpi4py import MPI
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    
    if rank == 0:
        print("=" * 60)
        print("Distributed N-Body Simulation (MPI)")
        print("=" * 60)
        print(f"Running on {comm.Get_size()} processes")
        
        # Generate bodies (only on rank 0)
        print("\nGenerating bodies...")
        bodies = generate_random_bodies(n=20, 
                                       mass_range=(1e20, 1e25),
                                       position_range=(-1e6, 1e6),
                                       velocity_range=(-1e3, 1e3))
        print(f"Created {len(bodies)} bodies")
    else:
        bodies = None
    
    # Create distributed simulation
    simulation = DistributedNBodySimulation(bodies)
    
    if rank == 0:
        print(f"\nProcess {rank}: Managing bodies {simulation.start_idx} to {simulation.end_idx}")
        print("Running simulation...")
    
    # Run simulation
    simulation.run(num_steps=100, dt=1.0, verbose=(rank == 0))
    
    # Print statistics (only on rank 0)
    if rank == 0:
        print("\n" + "=" * 60)
        print("Simulation Statistics")
        print("=" * 60)
        stats = simulation.get_statistics()
        for key, value in stats.items():
            if key != 'center_of_mass':
                print(f"{key}: {value}")
        print(f"center_of_mass: ({stats['center_of_mass'][0]:.2e}, "
              f"{stats['center_of_mass'][1]:.2e}, {stats['center_of_mass'][2]:.2e})")


if __name__ == "__main__":
    main()

