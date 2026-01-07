"""
Simple example demonstrating the threaded n-body simulation.
"""

from shared.utils import generate_random_bodies, generate_solar_system
from threaded.simulation import ThreadedNBodySimulation


def main():
    """Run a simple n-body simulation."""
    print("=" * 60)
    print("Simple N-Body Simulation (Threaded)")
    print("=" * 60)
    
    # Generate bodies
    print("\nGenerating bodies...")
    bodies = generate_random_bodies(n=10, 
                                   mass_range=(1e20, 1e25),
                                   position_range=(-1e6, 1e6),
                                   velocity_range=(-1e3, 1e3))
    
    print(f"Created {len(bodies)} bodies")
    
    # Create simulation
    simulation = ThreadedNBodySimulation(bodies, num_threads=4)
    
    # Run simulation
    print("\nRunning simulation...")
    simulation.run(num_steps=100, dt=1.0, verbose=True)
    
    # Print statistics
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

