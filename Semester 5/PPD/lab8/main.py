import multiprocessing
import time
import sys
from dsm import DSM


def variable_change_callback(var_id: int, value: int):
    process_id = multiprocessing.current_process().name
    print(f"[Process {process_id}] Callback: Variable {var_id} changed to {value}")


def process_main(process_id: int, num_processes: int, subscriptions: dict, operations: list):
    # Set process name
    multiprocessing.current_process().name = str(process_id)
    
    # Base port for communication
    base_port = 10000
    port = base_port + process_id
    
    # Create DSM instance
    dsm = DSM(process_id, num_processes, port, subscriptions, variable_change_callback)
    
    # Start DSM
    dsm.start()
    
    # Give time for all processes to connect
    time.sleep(1.0)
    
    print(f"[Process {process_id}] Started and connected")
    
    # Perform operations
    for op_type, args in operations:
        if op_type == "write":
            var_id, value = args
            print(f"[Process {process_id}] Writing {value} to variable {var_id}")
            dsm.write(var_id, value)
            time.sleep(0.5)  # Give time for propagation
        
        elif op_type == "cas":
            var_id, old_value, new_value = args
            print(f"[Process {process_id}] CAS: var{var_id} from {old_value} to {new_value}")
            result = dsm.compare_and_swap(var_id, old_value, new_value)
            print(f"[Process {process_id}] CAS result: {result}")
            time.sleep(0.5)
        
        elif op_type == "read":
            var_id = args[0]
            value = dsm.get_value(var_id)
            print(f"[Process {process_id}] Read variable {var_id}: {value}")
            time.sleep(0.2)
        
        elif op_type == "sleep":
            time.sleep(args[0])
    
    # Keep running for a bit to receive all callbacks
    time.sleep(2.0)
    
    # Stop DSM
    dsm.stop()
    print(f"[Process {process_id}] Stopped")


def main():
    # Configuration
    num_processes = 3
    num_variables = 3
    
    # Define subscriptions
    # Each variable has a list of process IDs that subscribe to it
    subscriptions = {
        0: [0, 1],  # Variable 0: subscribed by processes 0 and 1
        1: [1, 2],  # Variable 1: subscribed by processes 1 and 2
        2: [0, 1, 2],  # Variable 2: subscribed by all processes
    }
    
    # Define operations for each process
    operations = {
        0: [
            ("write", (0, 10)),  # Process 0 writes 10 to variable 0
            ("sleep", (0.5,)),
            ("write", (2, 20)),  # Process 0 writes 20 to variable 2
            ("sleep", (0.5,)),
            ("cas", (2, 20, 25)),  # Process 0 CAS: var2 from 20 to 25
        ],
        1: [
            ("sleep", (0.3,)),
            ("write", (0, 15)),  # Process 1 writes 15 to variable 0
            ("sleep", (0.5,)),
            ("write", (1, 30)),  # Process 1 writes 30 to variable 1
            ("sleep", (0.5,)),
            ("write", (2, 35)),  # Process 1 writes 35 to variable 2
        ],
        2: [
            ("sleep", (0.6,)),
            ("write", (1, 40)),  # Process 2 writes 40 to variable 1
            ("sleep", (0.5,)),
            ("read", (2,)),  # Process 2 reads variable 2
            ("cas", (1, 40, 45)),  # Process 2 CAS: var1 from 40 to 45
        ],
    }
    
    # Create and start processes
    processes = []
    for pid in range(num_processes):
        p = multiprocessing.Process(
            target=process_main,
            args=(pid, num_processes, subscriptions, operations.get(pid, []))
        )
        p.start()
        processes.append(p)
    
    # Wait for all processes to complete
    for p in processes:
        p.join()
    
    print("\nAll processes completed.")


if __name__ == "__main__":
    main()

