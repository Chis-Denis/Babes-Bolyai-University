#!/bin/bash
# Build script for n-body simulation

mkdir -p build
cd build
cmake ..
make -j$(nproc)

echo "Build complete! Executables are in the build/ directory:"
echo "  - simple (threaded example)"
echo "  - complex (threaded example)"
if [ -f distributed_example ]; then
    echo "  - distributed_example (MPI example)"
fi

