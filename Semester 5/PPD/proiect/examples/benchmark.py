"""
Benchmark script to compare threaded vs distributed implementations.
"""

import time
from shared.utils import generate_random_bodies
from threaded.simulation import ThreadedNBodySimulation

try:
    from distributed.simulation import DistributedNBodySimulation
    DISTRIBUTED_AVAILABLE = True
except RuntimeError:
    DISTRIBUTED_AVAILABLE = False
    print("Note: Distributed simulation not available (MPI required)")


def benchmark_threaded(num_bodies: int, num_steps: int, num_threads: int):
    """Benchmark threaded implementation."""
    print(f"\n{'='*60}")
    print(f"Threaded Implementation ({num_threads} threads)")
    print(f"{'='*60}")
    
    bodies = generate_random_bodies(n=num_bodies)
    simulation = ThreadedNBodySimulation(bodies, num_threads=num_threads)
    
    start_time = time.time()
    simulation.run(num_steps=num_steps, dt=1.0, verbose=False)
    elapsed = time.time() - start_time
    
    stats = simulation.get_statistics()
    print(f"Bodies: {num_bodies}")
    print(f"Steps: {num_steps}")
    print(f"Time: {elapsed:.2f} seconds")
    print(f"Time per step: {elapsed / num_steps * 1000:.2f} ms")
    print(f"Total energy: {stats['total_energy']:.2e} J")
    
    return elapsed


def benchmark_distributed(num_bodies: int, num_steps: int):
    """Benchmark distributed implementation."""
    from mpi4py import MPI
    
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    
    if rank == 0:
        print(f"\n{'='*60}")
        print(f"Distributed Implementation ({comm.Get_size()} processes)")
        print(f"{'='*60}")
        bodies = generate_random_bodies(n=num_bodies)
    else:
        bodies = None
    
    simulation = DistributedNBodySimulation(bodies)
    
    if rank == 0:
        start_time = time.time()
    
    simulation.run(num_steps=num_steps, dt=1.0, verbose=False)
    
    if rank == 0:
        elapsed = time.time() - start_time
        stats = simulation.get_statistics()
        print(f"Bodies: {num_bodies}")
        print(f"Steps: {num_steps}")
        print(f"Time: {elapsed:.2f} seconds")
        print(f"Time per step: {elapsed / num_steps * 1000:.2f} ms")
        print(f"Total energy: {stats['total_energy']:.2e} J")
        return elapsed
    else:
        return 0.0


def main():
    """Run benchmarks."""
    print("=" * 60)
    print("N-Body Simulation Benchmark")
    print("=" * 60)
    
    # Test parameters
    num_bodies = 20
    num_steps = 100
    
    # Benchmark threaded
    threaded_time = benchmark_threaded(num_bodies, num_steps, num_threads=4)
    
    # Benchmark distributed (if available)
    if DISTRIBUTED_AVAILABLE:
        print("\nNote: Run distributed benchmark with:")
        print(f"  mpirun -np 4 python examples/benchmark.py")
    else:
        print("\nDistributed benchmark skipped (MPI not available)")


if __name__ == "__main__":
    main()

