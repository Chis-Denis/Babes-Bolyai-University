# Distributed Shared Memory (DSM) Implementation

This project implements a distributed shared memory mechanism for distributed programming, addressing consistency issues in distributed systems.

## Overview

The DSM system provides:
- **Write operations** to local and remote variables
- **Total ordering of callbacks** - all processes receive the same sequence of variable change notifications
- **Compare-and-exchange (CAS)** operations with proper synchronization
- **Subscription-based access** - only subscribed processes can read/write and receive notifications

## Architecture

### Components

The codebase is organized into modular components for better understanding:

1. **`dsm_messages.py`**: Message type definitions and message structure
   - `MessageType` enum: All message types in the system
   - `Message` class: Message structure for communication

2. **`dsm_network.py`**: Network communication layer
   - `NetworkManager` class: Handles all TCP socket communication
   - Connection management (accepting, connecting, maintaining connections)
   - Message sending and receiving

3. **`dsm.py`**: Core DSM implementation
   - `DSM` class: Main distributed shared memory implementation
   - Variable storage and management
   - Write and CAS operation handlers
   - Total ordering mechanism
   - Callback system

4. **`main.py`**: Demonstration program showing DSM usage

### Key Features

- **Distributed Architecture**: No central server - communication only between subscribers
- **Total Ordering**: Uses Lamport logical clocks and two-phase commit to ensure all processes see callbacks in the same order
- **Static Subscriptions**: Each process knows which variables it subscribes to and who else subscribes
- **Fault-Free Assumption**: Assumes processes are not faulty (no fault tolerance)

## Requirements

- Python 3.6+
- No external dependencies (uses only standard library)

## Usage

### Running the Main Program

```bash
python main.py
```

The main program creates 3 processes that:
- Subscribe to different sets of variables
- Perform write and compare-and-exchange operations
- Receive callbacks when variables change

### Using the DSM Library

```python
from dsm import DSM

# Define subscriptions (var_id -> list of subscriber process IDs)
subscriptions = {
    0: [0, 1],      # Variable 0: processes 0 and 1 subscribe
    1: [1, 2],      # Variable 1: processes 1 and 2 subscribe
    2: [0, 1, 2],   # Variable 2: all processes subscribe
}

# Callback function for variable changes
def on_variable_change(var_id: int, value: int):
    print(f"Variable {var_id} changed to {value}")

# Create DSM instance
dsm = DSM(
    process_id=0,           # Unique process ID
    num_processes=3,        # Total number of processes
    port=10000,             # Base port for this process
    subscriptions=subscriptions,
    callback=on_variable_change
)

# Start DSM
dsm.start()

# Write to a variable
dsm.write(var_id=0, value=10)

# Compare and swap
success = dsm.compare_and_swap(var_id=0, old_value=10, new_value=20)

# Read a variable
value = dsm.get_value(var_id=0)

# Stop DSM
dsm.stop()
```

## Implementation Details

### Total Ordering Mechanism

To ensure all processes receive callbacks in the same order:

1. **Write Operation**:
   - Initiator sends write request to all subscribers
   - Each subscriber acknowledges with their logical clock
   - Initiator collects all acks and determines final timestamp (max + 1)
   - Initiator broadcasts commit message with final timestamp
   - All processes apply callbacks in timestamp order

2. **Compare-and-Exchange**:
   - Initiator sends CAS request to all subscribers
   - Each subscriber checks if current value matches expected value
   - All subscribers respond with success/failure
   - If all succeed, initiator broadcasts commit; otherwise abort
   - Callbacks are triggered only on successful commit

### Message Types

- `WRITE`: Initial write request
- `WRITE_ACK`: Acknowledgment of write request
- `WRITE_COMMIT`: Commit message with agreed timestamp
- `CAS`: Compare-and-swap request
- `CAS_ACK`: Acknowledgment of CAS request
- `CAS_COMMIT`: Commit message for successful CAS
- `CAS_ABORT`: Abort message for failed CAS

### Network Communication

- Uses TCP sockets for reliable communication
- Each process listens on a unique port (base_port + process_id)
- Processes connect to each other as needed
- Messages are serialized using pickle

## Notes

- **Subscriptions are static**: Defined at initialization, cannot change during runtime
- **Only subscribers can access**: Non-subscribers cannot read/write or receive notifications
- **Local-first design**: Variables are stored locally on subscribing processes
- **No central bottleneck**: All communication is peer-to-peer between subscribers

## Example Output

When running `main.py`, you should see output like:

```
[Process 0] Started and connected
[Process 1] Started and connected
[Process 2] Started and connected
[Process 0] Writing 10 to variable 0
[Process 0] Callback: Variable 0 changed to 10
[Process 1] Callback: Variable 0 changed to 10
[Process 1] Writing 15 to variable 0
...
```

All processes will see callbacks in the same order, ensuring consistency.

