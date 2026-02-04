# Distributed Shared Memory (DSM) - C# .NET Implementation

This project implements a simple distributed shared memory (DSM) mechanism in C# .NET.

## Features

- **Write Operation**: Write values to variables (local or remote)
- **Compare and Swap (CAS)**: Atomic compare-and-swap operation
- **Callback Notifications**: Receive notifications when variables change
- **Global Ordering**: All processes receive callbacks in the same order
- **Subscription-based Access**: Only subscribed nodes can access variables
- **Decentralized Architecture**: No central server, direct peer-to-peer communication

## Requirements

- .NET 8.0 SDK or later

## Building

To build the solution:

```bash
dotnet build DSM.sln
```

## Running

To run the program:

```bash
dotnet run --project DSMProgram/DSMProgram.csproj
```

The program will simulate 3 processes communicating via the DSM mechanism. Each process:
- Subscribes to specific variables
- Performs write and CAS operations
- Receives callbacks when variables change

## Architecture

- **DSM Library**: Core distributed shared memory implementation (`DSM/`)
- **DSMProgram**: Main program demonstrating the DSM usage (`DSMProgram/`)

### Key Components

1. **DSM.cs**: Main DSM class with write, CAS, and callback ordering logic
2. **DSMNetwork.cs**: Network communication layer using TCP/IP
3. **DSMMessages.cs**: Message types and serialization

## Design Notes

- Uses TCP/IP for inter-process communication
- Implements logical clocks for ordering
- Uses a two-phase commit protocol for writes and CAS operations
- Ensures global ordering of callbacks through deterministic timestamp ordering
- Subscription-based access control ensures only subscribed nodes can modify variables

