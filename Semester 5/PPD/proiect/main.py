"""
Main entry point for n-body simulation.
Allows easy selection between threaded and distributed implementations.
"""

import argparse
import sys
from shared.utils import generate_random_bodies, generate_solar_system


def run_threaded(num_bodies: int, num_steps: int, dt: float, num_threads: int, solar_system: bool):
    """Run threaded implementation."""
    from threaded.simulation import ThreadedNBodySimulation
    
    print("=" * 60)
    print("N-Body Simulation - Threaded Implementation")
    print("=" * 60)
    
    if solar_system:
        print("Using solar system configuration")
        bodies = generate_solar_system()
    else:
        print(f"Generating {num_bodies} random bodies...")
        bodies = generate_random_bodies(n=num_bodies)
    
    simulation = ThreadedNBodySimulation(bodies, num_threads=num_threads)
    print(f"Using {num_threads} threads")
    
    simulation.run(num_steps=num_steps, dt=dt, verbose=True)
    
    print("\n" + "=" * 60)
    print("Final Statistics")
    print("=" * 60)
    stats = simulation.get_statistics()
    for key, value in stats.items():
        if key != 'center_of_mass':
            print(f"{key}: {value}")
    print(f"center_of_mass: ({stats['center_of_mass'][0]:.2e}, "
          f"{stats['center_of_mass'][1]:.2e}, {stats['center_of_mass'][2]:.2e})")


def run_distributed(num_bodies: int, num_steps: int, dt: float, solar_system: bool):
    """Run distributed implementation."""
    try:
        from mpi4py import MPI
        from distributed.simulation import DistributedNBodySimulation
    except ImportError:
        print("Error: mpi4py is required for distributed simulation")
        print("Install with: pip install mpi4py")
        print("And ensure MPI is installed on your system")
        sys.exit(1)
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    
    if rank == 0:
        print("=" * 60)
        print("N-Body Simulation - Distributed Implementation (MPI)")
        print("=" * 60)
        print(f"Running on {comm.Get_size()} processes")
        
        if solar_system:
            print("Using solar system configuration")
            bodies = generate_solar_system()
        else:
            print(f"Generating {num_bodies} random bodies...")
            bodies = generate_random_bodies(n=num_bodies)
    else:
        bodies = None
    
    simulation = DistributedNBodySimulation(bodies)
    
    if rank == 0:
        simulation.run(num_steps=num_steps, dt=dt, verbose=True)
        
        print("\n" + "=" * 60)
        print("Final Statistics")
        print("=" * 60)
        stats = simulation.get_statistics()
        for key, value in stats.items():
            if key != 'center_of_mass':
                print(f"{key}: {value}")
        print(f"center_of_mass: ({stats['center_of_mass'][0]:.2e}, "
              f"{stats['center_of_mass'][1]:.2e}, {stats['center_of_mass'][2]:.2e})")
    else:
        simulation.run(num_steps=num_steps, dt=dt, verbose=False)


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description='N-Body Simulation - Threaded or Distributed Implementation'
    )
    parser.add_argument(
        '--mode',
        choices=['threaded', 'distributed'],
        default='threaded',
        help='Simulation mode (default: threaded)'
    )
    parser.add_argument(
        '--bodies',
        type=int,
        default=10,
        help='Number of bodies (default: 10)'
    )
    parser.add_argument(
        '--steps',
        type=int,
        default=100,
        help='Number of simulation steps (default: 100)'
    )
    parser.add_argument(
        '--dt',
        type=float,
        default=1.0,
        help='Time step (default: 1.0)'
    )
    parser.add_argument(
        '--threads',
        type=int,
        default=None,
        help='Number of threads (threaded mode only, default: CPU count)'
    )
    parser.add_argument(
        '--solar-system',
        action='store_true',
        help='Use solar system configuration instead of random bodies'
    )
    
    args = parser.parse_args()
    
    if args.mode == 'threaded':
        if args.threads is None:
            import os
            args.threads = os.cpu_count() or 4
        run_threaded(args.bodies, args.steps, args.dt, args.threads, args.solar_system)
    else:
        run_distributed(args.bodies, args.steps, args.dt, args.solar_system)


if __name__ == "__main__":
    main()

