# N-Body Simulation - C++ Implementation

A comprehensive C++ implementation of the n-body problem simulation with two parallel computing approaches:
1. **Threaded Implementation**: Uses `std::async` for multi-threaded computation
2. **Distributed Implementation**: Uses MPI (Message Passing Interface) for distributed computation across multiple nodes

## Project Structure

```
proiect/
├── Makefile                # Build configuration
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

- C++17 compatible compiler (g++ is included with MSYS2/MinGW)
- Make (mingw32-make on Windows)
- For distributed implementation: MPI (optional)

**Note**: OpenCL is NOT required. This project uses standard C++ threading (`std::async`) and MPI for distributed computing.

## Quick Start

### Build the Project

```powershell
# Build all threaded examples
mingw32-make all

# Or build specific targets
mingw32-make simple
mingw32-make complex
mingw32-make distributed  # (requires MPI)
```

### Run Examples

```powershell
# Simple threaded example
.\build\simple.exe

# Complex example (solar system)
.\build\complex.exe

# Distributed example (requires MPI)
mpiexec -n 4 .\build\distributed_example.exe
```

## Building

### With MPI Support

Install MPI to build the distributed implementation:

- **Windows**: Install MS-MPI from Microsoft
- **Linux**: `sudo apt-get install libopenmpi-dev openmpi-bin`
- **macOS**: `brew install openmpi`

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

## Testing

### Quick Test

```powershell
# Build the project
mingw32-make all

# Test threaded implementation
.\build\simple.exe
.\build\complex.exe

# Test distributed implementation (requires MPI)
mingw32-make distributed
mpiexec -n 4 .\build\distributed_example.exe
```

## License

This project is created for educational purposes as part of the Parallel and Distributed Programming course.
