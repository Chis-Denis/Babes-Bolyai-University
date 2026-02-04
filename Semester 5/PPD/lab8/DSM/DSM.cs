using System.Collections.Concurrent;

namespace DSMLib;

public class DSM : IDisposable
{
    public delegate void VariableChangeCallback(int varId, int value);

    // Process identification and configuration
    private readonly int _processId;
    private readonly int _numProcesses;
    private readonly int _port;
    private readonly Dictionary<int, List<int>> _subscriptions; // Maps varId -> list of subscribed process IDs
    private readonly VariableChangeCallback _callback;
    
    // Local variable storage and network communication
    private readonly Dictionary<int, int> _variables = new(); // Local copy of shared variables
    private readonly NetworkManager _network;
    
    // Logical clock for event ordering (Lamport timestamps)
    private int _logicalClock = 0;
    // Pending callbacks queued for global ordering (sorted by timestamp, senderId, msgId)
    private readonly SortedDictionary<(int timestamp, int senderId, int msgId), PendingMessage> _pendingMessages = new();
    private readonly object _pendingLock = new();
    
    // Message ID generation (thread-safe counter)
    private int _msgCounter = 0;
    private readonly object _msgCounterLock = new();
    
    // CAS operation tracking (waiting for acks from all subscribers)
    private readonly Dictionary<int, CASOperation> _casOperations = new();
    private readonly object _casLock = new();
    
    // Write operation tracking (waiting for acks from all subscribers)
    private readonly Dictionary<int, WriteAckInfo> _writeAcks = new();
    private readonly object _writeLock = new();
    
    private bool _disposed = false;

    private class PendingMessage
    {
        public Message Message { get; set; } = new();
        public int Timestamp { get; set; }
        public int MsgId { get; set; }
        public int SenderId { get; set; }
    }

    private class CASOperation
    {
        public int VarId { get; set; }
        public int OldValue { get; set; }
        public int NewValue { get; set; }
        public int ProcessId { get; set; }
        public HashSet<int> Acks { get; set; } = new(); // Process IDs that sent acks
        public Dictionary<int, bool> Results { get; set; } = new(); // Process ID = whether value matched
        public List<int> Timestamps { get; set; } = new(); // Timestamps from all processes for global ordering
    }

    private class WriteAckInfo
    {
        public int VarId { get; set; }
        public int Value { get; set; }
        public HashSet<int> Subscribers { get; set; } = new(); // All processes subscribed to this variable
        public List<int> Timestamps { get; set; } = new(); // Timestamps from all processes for global ordering
    }

    public DSM(int processId, int numProcesses, int port,
        Dictionary<int, List<int>> subscriptions,
        VariableChangeCallback callback)
    {
        _processId = processId;
        _numProcesses = numProcesses;
        _port = port;
        _subscriptions = new Dictionary<int, List<int>>(subscriptions);
        _callback = callback;
        
        _network = new NetworkManager(_processId, _numProcesses, _port, HandleMessage);
        
        // Initialize variables to 0
        foreach (var varId in _subscriptions.Keys)
        {
            _variables[varId] = 0;
        }
    }

    public void Start()
    {
        _network.Start();
    }

    public void Stop()
    {
        _network.Stop();
    }

    public bool Write(int varId, int value)
    {
        // Check if this process is subscribed to the variable
        if (!_subscriptions.ContainsKey(varId) || !_subscriptions[varId].Contains(_processId))
        {
            Console.WriteLine($"[Process {_processId}] Not subscribed to variable {varId}");
            return false;
        }

        var subscribers = _subscriptions[varId];
        var msgId = GetNextMsgId(); // Unique message ID for this operation
        var timestamp = IncrementClock(); // Increment logical clock for this event

        var writeMsg = new Message
        {
            Type = MessageType.Write,
            VarId = varId,
            Value = value,
            ProcessId = _processId,
            Timestamp = timestamp,
            SenderId = _processId,
            MsgId = msgId
        };

        // Track this write operation to collect acks
        lock (_writeLock)
        {
            _writeAcks[msgId] = new WriteAckInfo
            {
                VarId = varId,
                Value = value,
                Subscribers = new HashSet<int>(subscribers),
                Timestamps = new List<int>() // Will collect timestamps from all processes
            };
        }

        // Phase 1: Send write request to all subscribers
        foreach (var subscriber in subscribers)
        {
            if (subscriber != _processId)
            {
                _network.SendMessage(subscriber, writeMsg);
            }
        }

        // Wait for acks from all subscribers
        int globalTimestamp;
        lock (_writeLock)
        {
            var ackInfo = _writeAcks[msgId];
            var startTime = DateTime.Now;
            // Wait until all subscribers respond or timeout (5 seconds)
            while (ackInfo.Timestamps.Count < subscribers.Count - 1 && 
                   (DateTime.Now - startTime).TotalSeconds < 5)
            {
                Monitor.Wait(_writeLock, 100); // Wait for HandleWriteAck to add timestamp
            }

            // Timeout: not all subscribers responded
            if (ackInfo.Timestamps.Count < subscribers.Count - 1)
            {
                _writeAcks.Remove(msgId);
                return false;
            }

            // All acks received: calculate global timestamp for deterministic ordering
            ackInfo.Timestamps.Add(timestamp); // Add our own timestamp
            ackInfo.Timestamps.Sort(); // Sort all timestamps
            globalTimestamp = ackInfo.Timestamps[0]; // Use minimum for global ordering
            _writeAcks.Remove(msgId);
        }

        // Phase 2: Send commit with global timestamp to all subscribers
        var commitMsg = new Message
        {
            Type = MessageType.WriteCommit,
            VarId = varId,
            Value = value,
            ProcessId = _processId,
            Timestamp = globalTimestamp, // All processes use same timestamp for ordering
            SenderId = _processId,
            MsgId = msgId
        };

        foreach (var subscriber in subscribers)
        {
            if (subscriber != _processId)
            {
                _network.SendMessage(subscriber, commitMsg);
            }
        }

        // Queue callback for global ordering (ensures all processes see callbacks in same order)
        TriggerCallbackOrdered(varId, value, globalTimestamp, msgId, _processId);

        return true;
    }

    public bool CompareAndSwap(int varId, int oldValue, int newValue)
    {
        // Check subscription before attempting CAS
        if (!_subscriptions.ContainsKey(varId) || !_subscriptions[varId].Contains(_processId))
        {
            Console.WriteLine($"[Process {_processId}] Not subscribed to variable {varId}");
            return false;
        }

        var subscribers = _subscriptions[varId];
        var msgId = GetNextMsgId(); // Unique message ID for this CAS operation
        var timestamp = IncrementClock(); // Logical clock for this event

        var casMsg = new Message
        {
            Type = MessageType.Cas,
            VarId = varId,
            OldValue = oldValue,
            NewValue = newValue,
            ProcessId = _processId,
            Timestamp = timestamp,
            SenderId = _processId,
            MsgId = msgId
        };

        // Track this CAS operation to collect acks and results
        lock (_casLock)
        {
            _casOperations[msgId] = new CASOperation
            {
                VarId = varId,
                OldValue = oldValue,
                NewValue = newValue,
                ProcessId = _processId,
                Timestamps = new List<int>() // Will collect timestamps from all processes
            };
        }

        // Phase 1: Send CAS request to all subscribers
        foreach (var subscriber in subscribers)
        {
            if (subscriber != _processId)
            {
                _network.SendMessage(subscriber, casMsg);
            }
        }

        // Wait for acks from all subscribers (with timeout)
        bool success = false;
        int globalTimestamp;
        lock (_casLock)
        {
            var casOp = _casOperations[msgId];
            var startTime = DateTime.Now;
            // Wait until all subscribers respond or timeout (5 seconds)
            while (casOp.Acks.Count < subscribers.Count - 1 && 
                   (DateTime.Now - startTime).TotalSeconds < 5)
            {
                Monitor.Wait(_casLock, 100); // Wait for HandleCasAck to add result
            }

            // Timeout: not all subscribers responded
            if (casOp.Acks.Count < subscribers.Count - 1)
            {
                _casOperations.Remove(msgId);
                return false;
            }

            // All acks received: calculate global timestamp and check consensus
            casOp.Timestamps.Add(timestamp); // Add our own timestamp
            casOp.Timestamps.Sort(); // Sort all timestamps
            globalTimestamp = casOp.Timestamps[0]; // Use minimum for global ordering
            
            // CAS succeeds only if ALL processes agree the value matches
            success = casOp.Results.Values.All(r => r);
            
            _casOperations.Remove(msgId);
        }

        // Phase 2: Send commit (if all agreed) or abort (if any disagreed) with global timestamp
        var responseMsg = new Message
        {
            Type = success ? MessageType.CasCommit : MessageType.CasAbort,
            VarId = varId,
            OldValue = oldValue,
            NewValue = newValue,
            ProcessId = _processId,
            Timestamp = globalTimestamp, // All processes use same timestamp for ordering
            SenderId = _processId,
            MsgId = msgId
        };

        foreach (var subscriber in subscribers)
        {
            if (subscriber != _processId)
            {
                _network.SendMessage(subscriber, responseMsg);
            }
        }

        // Only update variable and trigger callback if CAS succeeded
        if (success)
        {
            _variables[varId] = newValue;
            TriggerCallbackOrdered(varId, newValue, globalTimestamp, msgId, _processId);
        }

        return success;
    }

    public int GetValue(int varId)
    {
        return _variables.TryGetValue(varId, out var value) ? value : 0;
    }

    private int GetNextMsgId()
    {
        // Thread-safe message ID generation
        lock (_msgCounterLock)
        {
            return ++_msgCounter;
        }
    }

    private int IncrementClock()
    {
        // Increment logical clock for local event
        return ++_logicalClock;
    }

    private void UpdateClock(int receivedTimestamp)
    {
        // Update logical clock when receiving message
        _logicalClock = Math.Max(_logicalClock, receivedTimestamp) + 1;
    }

    private void HandleMessage(Message msg)
    {
        // Update logical clock before processing message
        UpdateClock(msg.Timestamp);

        // Route message to appropriate handler based on type
        switch (msg.Type)
        {
            case MessageType.Write:
                HandleWrite(msg);
                break;
            case MessageType.WriteAck:
                HandleWriteAck(msg);
                break;
            case MessageType.WriteCommit:
                HandleWriteCommit(msg);
                break;
            case MessageType.Cas:
                HandleCas(msg);
                break;
            case MessageType.CasAck:
                HandleCasAck(msg);
                break;
            case MessageType.CasCommit:
                HandleCasCommit(msg);
                break;
            case MessageType.CasAbort:
                HandleCasAbort(msg);
                break;
        }
    }

    private void HandleWrite(Message msg)
    {
        // Respond to write request with acknowledgment (two-phase commit phase 1)
        var ackMsg = new Message
        {
            Type = MessageType.WriteAck,
            VarId = msg.VarId,
            Value = msg.Value,
            ProcessId = _processId,
            Timestamp = IncrementClock(), // Include our timestamp for global ordering
            SenderId = _processId,
            MsgId = msg.MsgId
        };

        _network.SendMessage(msg.SenderId, ackMsg);
    }

    private void HandleWriteAck(Message msg)
    {
        // Collect acknowledgment from subscriber (wakes up waiting Write operation)
        lock (_writeLock)
        {
            if (_writeAcks.TryGetValue(msg.MsgId, out var ackInfo))
            {
                ackInfo.Timestamps.Add(msg.Timestamp); // Collect timestamp for global ordering
            }
            Monitor.PulseAll(_writeLock); // Wake up Write() waiting for acks
        }
    }

    private void HandleWriteCommit(Message msg)
    {
        // Apply write commit (two-phase commit phase 2): update local variable
        _variables[msg.VarId] = msg.Value;
        // Queue callback for global ordering
        TriggerCallbackOrdered(msg.VarId, msg.Value, msg.Timestamp, msg.MsgId, msg.SenderId);
    }

    private void HandleCas(Message msg)
    {
        // Check if current value matches expected value for CAS
        var currentValue = GetValue(msg.VarId);
        var matches = currentValue == msg.OldValue;

        // Respond with CAS acknowledgment (two-phase commit phase 1)
        var ackMsg = new Message
        {
            Type = MessageType.CasAck,
            VarId = msg.VarId,
            OldValue = msg.OldValue,
            NewValue = msg.NewValue,
            Value = matches ? 1 : 0, // 1 = match (success), 0 = no match (failure)
            ProcessId = _processId,
            Timestamp = IncrementClock(), // Include our timestamp for global ordering
            SenderId = _processId,
            MsgId = msg.MsgId
        };

        _network.SendMessage(msg.SenderId, ackMsg);
    }

    private void HandleCasAck(Message msg)
    {
        // Collect CAS acknowledgment from subscriber (wakes up waiting CompareAndSwap operation)
        lock (_casLock)
        {
            if (_casOperations.TryGetValue(msg.MsgId, out var casOp))
            {
                casOp.Acks.Add(msg.SenderId); // Track which process sent ack
                casOp.Results[msg.SenderId] = msg.Value == 1; // Store comparison result (match or not)
                casOp.Timestamps.Add(msg.Timestamp); // Collect timestamp for global ordering
            }
            Monitor.PulseAll(_casLock); // Wake up CompareAndSwap() waiting for acks
        }
    }

    private void HandleCasCommit(Message msg)
    {
        // Apply CAS commit (two-phase commit phase 2): update local variable
        _variables[msg.VarId] = msg.NewValue;
        // Queue callback for global ordering
        TriggerCallbackOrdered(msg.VarId, msg.NewValue, msg.Timestamp, msg.MsgId, msg.SenderId);
    }

    private void HandleCasAbort(Message msg)
    {
        // CAS failed: no change to variable (value didn't match expected)
    }

    private void TriggerCallbackOrdered(int varId, int value, int timestamp, int msgId, int senderId)
    {
        // Queue callback for global ordering (ensures all processes see callbacks in same order)
        var pending = new PendingMessage
        {
            Message = new Message { VarId = varId, Value = value },
            Timestamp = timestamp,
            MsgId = msgId,
            SenderId = senderId
        };

        lock (_pendingLock)
        {
            // Use (timestamp, senderId, msgId) as key for deterministic sorting
            var key = (timestamp, senderId, msgId);
            _pendingMessages[key] = pending;
        }
        
        // Process any ready callbacks in order
        ProcessPendingMessages();
    }

    private void ProcessPendingMessages()
    {
        lock (_pendingLock)
        {
            // Process callbacks in deterministic order: timestamp (primary), senderId (secondary), msgId (tertiary)
            // This ensures ALL processes see callbacks in the SAME order (global ordering requirement)
            while (_pendingMessages.Count > 0)
            {
                var firstKey = _pendingMessages.Keys.First(); // SortedDictionary ensures deterministic order
                var pending = _pendingMessages[firstKey];
                _pendingMessages.Remove(firstKey);
                
                // Release lock before calling callback to avoid deadlocks (callback might take time)
                Monitor.Exit(_pendingLock);
                try
                {
                    _callback(pending.Message.VarId, pending.Message.Value);
                }
                finally
                {
                    Monitor.Enter(_pendingLock); // Re-acquire lock for next iteration
                }
            }
        }
    }

    public void Dispose()
    {
        if (!_disposed)
        {
            Stop();
            _network.Dispose();
            _disposed = true;
        }
    }
}

