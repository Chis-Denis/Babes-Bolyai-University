# N-Body Simulation - C++ Implementation

A comprehensive C++ implementation of the n-body problem simulation with two parallel computing approaches:
1. **Threaded Implementation**: Uses `std::async` for multi-threaded computation
2. **Distributed Implementation**: Uses MPI (Message Passing Interface) for distributed computation across multiple nodes

## Project Structure

```
cpp/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
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

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- For distributed implementation: MPI (OpenMPI or MPICH)

## Building

### Basic Build

```bash
cd cpp
mkdir build
cd build
cmake ..
make
```

### Windows (Visual Studio)

```bash
cd cpp
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

### With MPI Support

Ensure MPI is installed and CMake can find it:

```bash
# Linux
sudo apt-get install libopenmpi-dev openmpi-bin

# macOS
brew install openmpi

# Windows
# Install MS-MPI from Microsoft
```

CMake will automatically detect MPI if installed.

## Running

### Threaded Examples

```bash
# Simple example
./build/simple

# Complex example
./build/complex
```

### Distributed Example

```bash
# Run with 4 MPI processes
mpirun -np 4 ./build/distributed_example
```

## Features

- **Two Implementations**: Threaded and distributed versions
- **Modern C++**: Uses C++17 features
- **Modular Design**: Clean separation with static libraries
- **Physics Accuracy**: Proper gravitational force calculations
- **Energy Conservation**: Tracks kinetic and potential energy
- **Scalable**: Supports any number of bodies and processes

## Code Examples

### Basic Threaded Simulation

```cpp
#include "threaded_simulation.h"
#include "utils.h"

int main() {
    auto bodies = generateRandomBodies(10);
    ThreadedNBodySimulation simulation(bodies, 4);
    simulation.run(100, 1.0, true);
    
    auto stats = simulation.getStatistics();
    std::cout << "Total energy: " << stats.total_energy << std::endl;
    return 0;
}
```

### Distributed Simulation

```cpp
#include "distributed_simulation.h"
#include "utils.h"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    std::vector<Body> bodies;
    if (rank == 0) {
        bodies = generateRandomBodies(20);
    }
    
    DistributedNBodySimulation simulation(bodies);
    simulation.run(100, 1.0, true);
    
    MPI_Finalize();
    return 0;
}
```

## Performance

The C++ implementation provides significant performance improvements over the Python version:
- **Compiled Performance**: Native code execution
- **Memory Efficiency**: Direct memory management
- **Optimization**: Compiler optimizations available

## License

This project is created for educational purposes as part of the Parallel and Distributed Programming course.

