# N-Body Simulation Project

A comprehensive implementation of the n-body problem simulation with two parallel computing approaches:
1. **Threaded Implementation**: Uses Python's `concurrent.futures` (or C++ `std::async`) for multi-threaded computation
2. **Distributed Implementation**: Uses MPI (Message Passing Interface) for distributed computation across multiple nodes

## Implementations

This project includes **two language implementations**:
- **Python** (`/` root directory): Python implementation using concurrent.futures and mpi4py
- **C++** (`/cpp` directory): C++ implementation using std::async and MPI C++ bindings

## Project Structure

### Python Implementation

```
proiect/
├── README.md                 # This file
├── requirements.txt          # Python dependencies
├── shared/                   # Shared utilities
│   ├── __init__.py
│   ├── body.py              # Body class representing celestial bodies
│   ├── physics.py           # Physics calculations (forces, energy)
│   └── utils.py             # Utility functions (body generation, I/O)
├── threaded/                 # Threaded implementation
│   ├── __init__.py
│   ├── simulation.py        # Main threaded simulation class
│   └── visualizer.py        # Optional 3D visualization
├── distributed/              # Distributed MPI implementation
│   ├── __init__.py
│   ├── simulation.py        # Main distributed simulation class
│   └── config.py            # MPI configuration
└── examples/                 # Example scripts
    ├── simple.py            # Simple threaded example
    ├── complex.py           # Complex example with visualization
    ├── distributed_example.py  # MPI distributed example
    └── benchmark.py         # Performance comparison
```

### C++ Implementation

```
cpp/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # C++ specific documentation
├── include/                # Header files
│   ├── body.h             # Body class
│   ├── physics.h          # Physics calculations
│   ├── utils.h            # Utility functions
│   ├── threaded_simulation.h    # Threaded simulation
│   └── distributed_simulation.h # Distributed simulation
├── src/                    # Source files
│   ├── body.cpp
│   ├── physics.cpp
│   ├── utils.cpp
│   ├── threaded_simulation.cpp
│   └── distributed_simulation.cpp
└── examples/               # Example programs
    ├── simple.cpp         # Simple threaded example
    ├── complex.cpp        # Complex example
    └── distributed_example.cpp # MPI distributed example
```

## Features

- **Two Implementations**: Threaded and distributed versions as required
- **Modular Design**: Clean separation of concerns with shared utilities
- **Physics Accuracy**: Proper gravitational force calculations with softening parameter
- **Energy Conservation**: Tracks kinetic and potential energy
- **Visualization**: Optional 3D trajectory plotting (threaded version)
- **Scalable**: Supports any number of bodies and processes
- **Well Documented**: Comprehensive code documentation

## Installation

### Python Implementation

#### Prerequisites

- Python 3.7 or higher
- For distributed implementation: MPI implementation (OpenMPI or MPICH)

#### Install Dependencies

```bash
pip install -r requirements.txt
```

#### Install MPI (for distributed implementation)

**Windows:**
- Install MS-MPI from Microsoft
- Or use WSL with OpenMPI

**Linux:**
```bash
sudo apt-get install openmpi-bin openmpi-common libopenmpi-dev
```

**macOS:**
```bash
brew install openmpi
```

### C++ Implementation

#### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- For distributed implementation: MPI (OpenMPI or MPICH)

#### Build

```bash
cd cpp
mkdir build
cd build
cmake ..
make
```

See `cpp/README.md` for detailed C++ build instructions.

## Usage

### Threaded Implementation

Run a simple threaded simulation:

```bash
python examples/simple.py
```

Run a complex example with visualization:

```bash
python examples/complex.py
```

### Distributed Implementation

Run the distributed MPI simulation:

```bash
mpirun -np 4 python examples/distributed_example.py
```

Where `-np 4` specifies the number of MPI processes to use.

### Benchmarking

Compare performance between implementations:

```bash
# Threaded benchmark
python examples/benchmark.py

# Distributed benchmark
mpirun -np 4 python examples/benchmark.py
```

## Code Examples

### Basic Threaded Simulation

```python
from shared.utils import generate_random_bodies
from threaded.simulation import ThreadedNBodySimulation

# Generate bodies
bodies = generate_random_bodies(n=10)

# Create simulation
simulation = ThreadedNBodySimulation(bodies, num_threads=4)

# Run simulation
simulation.run(num_steps=100, dt=1.0)

# Get statistics
stats = simulation.get_statistics()
print(f"Total energy: {stats['total_energy']:.2e} J")
```

### Distributed Simulation

```python
from shared.utils import generate_random_bodies
from distributed.simulation import DistributedNBodySimulation

# Generate bodies (only on rank 0)
from mpi4py import MPI
if MPI.COMM_WORLD.Get_rank() == 0:
    bodies = generate_random_bodies(n=20)
else:
    bodies = None

# Create distributed simulation
simulation = DistributedNBodySimulation(bodies)

# Run simulation
simulation.run(num_steps=100, dt=1.0)
```

## Physics Details

The simulation implements:

- **Gravitational Force**: F = G × m₁ × m₂ / r²
- **Integration**: Verlet integration method for position and velocity updates
- **Softening Parameter**: Prevents division by zero for close encounters
- **Energy Conservation**: Tracks kinetic and potential energy

## Performance Considerations

- **Threaded Version**: Best for single-node multi-core systems
- **Distributed Version**: Best for multi-node clusters with many bodies
- **Scaling**: Performance improves with more threads/processes up to a point
- **Communication Overhead**: Distributed version has MPI communication overhead

## Project Requirements Met

✅ **Two Implementations**:
- Threaded implementation using `concurrent.futures`
- Distributed implementation using MPI (mpi4py)

✅ **Clean Code Structure**:
- Modular design with separated concerns
- Shared utilities for common functionality
- Well-documented code

✅ **Working Implementation**:
- Both implementations are functional and tested
- Example scripts demonstrate usage
- Proper error handling

## License

This project is created for educational purposes as part of the Parallel and Distributed Programming course.

## Author

Created for PPD (Parallel and Distributed Programming) course project.

