using DSMLib;
using System.Collections.Concurrent;

// Callback function called when any subscribed variable changes
void VariableChangeCallback(int varId, int value)
{
    Console.WriteLine($"[Callback] Variable {varId} changed to {value}");
}

// Main function for each process
async Task ProcessMain(int processId, int numProcesses,
    Dictionary<int, List<int>> subscriptions,
    List<(string opType, int[] args)> operations)
{
    const int basePort = 10000;
    int port = basePort + processId; // Each process gets unique port

    // Create DSM instance for this process
    using var dsm = new DSM(processId, numProcesses, port, subscriptions, VariableChangeCallback);
    
    dsm.Start(); // Start network communication
    
    await Task.Delay(1000); // Wait for all processes to connect
    
    Console.WriteLine($"[Process {processId}] Started and connected");

    // Execute all operations for this process
    foreach (var (opType, args) in operations)
    {
        switch (opType)
        {
            case "write":
                {
                    int varId = args[0];
                    int value = args[1];
                    Console.WriteLine($"[Process {processId}] Writing {value} to variable {varId}");
                    dsm.Write(varId, value); // Write to shared variable
                    await Task.Delay(500); // Small delay between operations
                }
                break;
                
            case "cas":
                {
                    int varId = args[0];
                    int oldValue = args[1];
                    int newValue = args[2];
                    Console.WriteLine($"[Process {processId}] CAS: var{varId} from {oldValue} to {newValue}");
                    bool result = dsm.CompareAndSwap(varId, oldValue, newValue);
                    Console.WriteLine($"[Process {processId}] CAS result: {result}");
                    await Task.Delay(500);
                }
                break;
                
            case "read":
                {
                    int varId = args[0];
                    int value = dsm.GetValue(varId);
                    Console.WriteLine($"[Process {processId}] Read variable {varId}: {value}");
                    await Task.Delay(200);
                }
                break;
                
            case "sleep":
                {
                    await Task.Delay(args[0]); // Sleep to synchronize timing
                }
                break;
        }
    }

    await Task.Delay(3000); // Wait for all operations to complete
    
    dsm.Stop(); // Stop network communication
    Console.WriteLine($"[Process {processId}] Stopped");
}

int numProcesses = 3;

// Define which processes are subscribed to which variables
// Key = variable ID, Value = list of process IDs subscribed to that variable
var subscriptions = new Dictionary<int, List<int>>
{
    { 0, new List<int> { 0, 1 } },      // Variable 0: subscribed by processes 0, 1
    { 1, new List<int> { 1, 2 } },
    { 2, new List<int> { 0, 1, 2 } }
};

var operations = new Dictionary<int, List<(string, int[])>>
{
    {
        0, new List<(string, int[])>
        {
            ("write", new[] { 0, 10 }),
            ("sleep", new[] { 500 }),
            ("write", new[] { 2, 20 }),
            ("sleep", new[] { 1000 }),
            ("cas", new[] { 2, 20, 25 })
        }
    },
    {
        1, new List<(string, int[])>
        {
            ("sleep", new[] { 300 }),
            ("write", new[] { 0, 15 }),
            ("sleep", new[] { 500 }),
            ("write", new[] { 1, 30 }),
            ("sleep", new[] { 500 }),
            ("write", new[] { 2, 35 })
        }
    },
    {
        2, new List<(string, int[])>
        {
            ("sleep", new[] { 600 }),
            ("write", new[] { 1, 40 }),
            ("sleep", new[] { 500 }),
            ("read", new[] { 2 }),
            ("cas", new[] { 1, 40, 45 })
        }
    }
};

// Create and start all processes concurrently
var tasks = new List<Task>();

for (int pid = 0; pid < numProcesses; pid++)
{
    int processId = pid; // Capture for closure
    tasks.Add(ProcessMain(processId, numProcesses, subscriptions, operations[processId]));
}

// Wait for all processes to complete
await Task.WhenAll(tasks);

Console.WriteLine("\nAll processes completed.");

