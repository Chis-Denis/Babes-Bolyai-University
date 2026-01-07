cmake_minimum_required(VERSION 3.10)
project(NBodySimulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files for shared library
set(SHARED_SOURCES
    src/body.cpp
    src/physics.cpp
    src/utils.cpp
)

# Create shared library
add_library(nbody_shared STATIC ${SHARED_SOURCES})

# Threaded simulation library
set(THREADED_SOURCES
    src/threaded_simulation.cpp
)

add_library(nbody_threaded STATIC ${THREADED_SOURCES})
target_link_libraries(nbody_threaded nbody_shared)

# Example executables
# Simple threaded example
add_executable(simple examples/simple.cpp)
target_link_libraries(simple nbody_threaded nbody_shared)

# Complex threaded example
add_executable(complex examples/complex.cpp)
target_link_libraries(complex nbody_threaded nbody_shared)

# Distributed simulation library (MPI) - optional
find_package(MPI COMPONENTS CXX)

if(MPI_CXX_FOUND)
    set(DISTRIBUTED_SOURCES
        src/distributed_simulation.cpp
    )

    add_library(nbody_distributed STATIC ${DISTRIBUTED_SOURCES})
    target_link_libraries(nbody_distributed nbody_shared MPI::MPI_CXX)

    # Distributed MPI example
    add_executable(distributed_example examples/distributed_example.cpp)
    target_link_libraries(distributed_example nbody_distributed nbody_shared MPI::MPI_CXX)
    
    message(STATUS "MPI found - distributed examples will be built")
else()
    message(WARNING "MPI not found - distributed examples will not be built")
    message(STATUS "Install MPI to build distributed examples")
endif()

# Installation
install(TARGETS simple complex distributed_example
    RUNTIME DESTINATION bin
)

install(TARGETS nbody_shared nbody_threaded nbody_distributed
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

