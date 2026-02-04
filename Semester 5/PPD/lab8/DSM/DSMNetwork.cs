using System.Net;
using System.Net.Sockets;

namespace DSMLib;

public class NetworkManager : IDisposable
{
    private readonly int _processId;
    private readonly int _numProcesses;
    private readonly int _port;
    private readonly Action<Message> _messageHandler;
    
    private TcpListener? _listener;
    private readonly Dictionary<int, TcpClient> _connections = new();
    private readonly object _connectionLock = new();
    private readonly CancellationTokenSource _cancellationTokenSource = new();
    private bool _disposed = false;

    public NetworkManager(int processId, int numProcesses, int port, Action<Message> messageHandler)
    {
        _processId = processId;
        _numProcesses = numProcesses;
        _port = port;
        _messageHandler = messageHandler;
    }

    public void Start()
    {
        // Start TCP listener on this process's port
        _listener = new TcpListener(IPAddress.Any, _port);
        _listener.Start();
        
        // Start background tasks: accept incoming connections and connect to other processes
        Task.Run(AcceptConnections);
        Task.Run(ConnectToOthers);
    }

    public void Stop()
    {
        _cancellationTokenSource.Cancel();
        _listener?.Stop();
        
        lock (_connectionLock)
        {
            foreach (var client in _connections.Values)
            {
                try
                {
                    client.Close();
                }
                catch { }
            }
            _connections.Clear();
        }
    }

    public void SendMessage(int targetId, Message msg)
    {
        // Handle local messages directly (no network overhead)
        if (targetId == _processId)
        {
            _messageHandler(msg);
            return;
        }

        // Send message over TCP to target process
        lock (_connectionLock)
        {
            if (_connections.TryGetValue(targetId, out var client) && client.Connected)
            {
                try
                {
                    var data = msg.Serialize(); // Convert message to byte array
                    var lengthBytes = BitConverter.GetBytes(data.Length); // Prepend message length
                    var stream = client.GetStream();
                    
                    // Send: [4 bytes length][message data]
                    stream.Write(lengthBytes, 0, lengthBytes.Length);
                    stream.Write(data, 0, data.Length);
                    stream.Flush();
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[Process {_processId}] Error sending to {targetId}: {ex.Message}");
                }
            }
        }
    }

    private async Task AcceptConnections()
    {
        // Continuously accept incoming TCP connections from other processes
        while (!_cancellationTokenSource.Token.IsCancellationRequested)
        {
            try
            {
                var client = await _listener!.AcceptTcpClientAsync();
                _ = Task.Run(() => HandleClient(client)); // Handle each connection in separate task
            }
            catch (ObjectDisposedException)
            {
                break; // Listener was stopped
            }
            catch (Exception ex)
            {
                if (!_cancellationTokenSource.Token.IsCancellationRequested)
                {
                    Console.WriteLine($"[Process {_processId}] Accept error: {ex.Message}");
                }
            }
        }
    }

    private async Task HandleClient(TcpClient client)
    {
        // Handle messages from a connected client (receive loop)
        try
        {
            var stream = client.GetStream();
            var buffer = new byte[4];
            
            while (!_cancellationTokenSource.Token.IsCancellationRequested && client.Connected)
            {
                // Read message length (first 4 bytes)
                int bytesRead = 0;
                while (bytesRead < 4)
                {
                    var read = await stream.ReadAsync(buffer, bytesRead, 4 - bytesRead, _cancellationTokenSource.Token);
                    if (read == 0) return; // Connection closed
                    bytesRead += read;
                }
                
                int messageLength = BitConverter.ToInt32(buffer, 0);
                
                // Read message data (based on length)
                var messageBuffer = new byte[messageLength];
                bytesRead = 0;
                while (bytesRead < messageLength)
                {
                    var read = await stream.ReadAsync(messageBuffer, bytesRead, messageLength - bytesRead, _cancellationTokenSource.Token);
                    if (read == 0) return; // Connection closed
                    bytesRead += read;
                }
                
                // Deserialize and handle message
                var message = Message.Deserialize(messageBuffer);
                _messageHandler(message);
            }
        }
        catch (Exception ex)
        {
            if (!_cancellationTokenSource.Token.IsCancellationRequested)
            {
                Console.WriteLine($"[Process {_processId}] Client handler error: {ex.Message}");
            }
        }
        finally
        {
            client.Close();
        }
    }

    private async Task ConnectToOthers()
    {
        // Connect to all other processes (with retries for startup synchronization)
        const int basePort = 10000;
        const int maxRetries = 10;
        const int retryDelay = 500;

        for (int pid = 0; pid < _numProcesses; pid++)
        {
            if (pid == _processId) continue; // Skip self

            // Retry connection until successful or max retries reached
            for (int retry = 0; retry < maxRetries; retry++)
            {
                try
                {
                    var client = new TcpClient();
                    await client.ConnectAsync("127.0.0.1", basePort + pid);
                    
                    lock (_connectionLock)
                    {
                        _connections[pid] = client; // Store connection for sending messages
                    }
                    
                    _ = Task.Run(() => HandleClient(client)); // Start receiving messages from this connection
                    Console.WriteLine($"[Process {_processId}] Connected to process {pid}");
                    break; // Success, move to next process
                }
                catch
                {
                    // Retry after delay (process might not be ready yet)
                    if (retry < maxRetries - 1)
                    {
                        await Task.Delay(retryDelay);
                    }
                }
            }
        }
    }

    public void Dispose()
    {
        if (!_disposed)
        {
            Stop();
            _cancellationTokenSource.Dispose();
            _disposed = true;
        }
    }
}

