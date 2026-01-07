"""
Complex example with solar system simulation and visualization.
"""

from shared.utils import generate_solar_system
from threaded.simulation import ThreadedNBodySimulation

try:
    from threaded.visualizer import NBodyVisualizer
    VISUALIZATION_AVAILABLE = True
except ImportError:
    VISUALIZATION_AVAILABLE = False
    print("Note: matplotlib not available for visualization")


def main():
    """Run a complex n-body simulation with visualization."""
    print("=" * 60)
    print("Solar System N-Body Simulation (Threaded)")
    print("=" * 60)
    
    # Generate solar system
    print("\nGenerating solar system...")
    bodies = generate_solar_system()
    
    print(f"Created {len(bodies)} celestial bodies:")
    for body in bodies:
        print(f"  - {body}")
    
    # Create simulation
    simulation = ThreadedNBodySimulation(bodies, num_threads=4)
    
    # Create visualizer if available
    visualizer = None
    if VISUALIZATION_AVAILABLE:
        visualizer = NBodyVisualizer()
        print("\nVisualization enabled")
    
    # Run simulation with periodic visualization updates
    print("\nRunning simulation...")
    num_steps = 1000
    dt = 3600.0  # 1 hour per step
    
    for step in range(num_steps):
        simulation.step(dt)
        
        if visualizer:
            visualizer.update(simulation.bodies)
        
        if (step + 1) % 100 == 0:
            kinetic, potential, total = simulation.get_energy()
            print(f"Step {step + 1}/{num_steps} | "
                  f"Time: {simulation.time_elapsed / 86400:.2f} days | "
                  f"Energy: {total:.2e} J")
    
    # Print final statistics
    print("\n" + "=" * 60)
    print("Final Statistics")
    print("=" * 60)
    stats = simulation.get_statistics()
    for key, value in stats.items():
        if key != 'center_of_mass':
            print(f"{key}: {value}")
    
    # Show visualization
    if visualizer:
        print("\nGenerating visualization...")
        visualizer.plot()
        visualizer.save("solar_system_trajectories.png")
        print("Visualization saved to solar_system_trajectories.png")
        # Uncomment to display interactively:
        # visualizer.show()


if __name__ == "__main__":
    main()

