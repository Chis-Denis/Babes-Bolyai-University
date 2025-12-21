using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    // Simple HTTP Parser
    public class HttpParser
    {
        public Dictionary<string, string> Headers { get; private set; }
        public int ContentLength { get; private set; }
        public string StatusLine { get; private set; }
        public byte[] Body { get; private set; }

        public HttpParser()
        {
            Headers = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        }

        public bool ParseResponse(byte[] data)
        {
            string response = Encoding.UTF8.GetString(data);
            string[] lines = response.Split(new[] { "\r\n" }, StringSplitOptions.None);

            if (lines.Length == 0) return false;

            StatusLine = lines[0];
            int headerEndIndex = -1;

            // Parse headers
            for (int i = 1; i < lines.Length; i++)
            {
                if (string.IsNullOrEmpty(lines[i]))
                {
                    headerEndIndex = i;
                    break;
                }

                int colonIndex = lines[i].IndexOf(':');
                if (colonIndex > 0)
                {
                    string key = lines[i].Substring(0, colonIndex).Trim();
                    string value = lines[i].Substring(colonIndex + 1).Trim();
                    Headers[key] = value;

                    // Extract Content-Length
                    if (key.Equals("Content-Length", StringComparison.OrdinalIgnoreCase))
                    {
                        if (int.TryParse(value, out int length))
                        {
                            ContentLength = length;
                        }
                    }
                }
            }

            // Extract body
            if (headerEndIndex >= 0 && headerEndIndex < lines.Length - 1)
            {
                int bodyStart = response.IndexOf("\r\n\r\n", StringComparison.Ordinal);
                if (bodyStart >= 0)
                {
                    bodyStart += 4; // Skip "\r\n\r\n"
                    int bodyLength = data.Length - bodyStart;
                    if (bodyLength > 0)
                    {
                        Body = new byte[bodyLength];
                        Array.Copy(data, bodyStart, Body, 0, bodyLength);
                    }
                }
            }

            return true;
        }
    }

    // Implementation 1: Direct Callback (Event-driven)
    public class CallbackHttpDownloader
    {
        private class DownloadState
        {
            public Socket Socket { get; set; }
            public string Host { get; set; }
            public string Path { get; set; }
            public List<byte> ResponseData { get; set; }
            public HttpParser Parser { get; set; }
            public Action<string, byte[]> OnComplete { get; set; }
            public Action<string, Exception> OnError { get; set; }
        }

        public static void DownloadFile(string url, Action<string, byte[]> onComplete, Action<string, Exception> onError)
        {
            try
            {
                Uri uri = new Uri(url);
                string host = uri.Host;
                int port = uri.Port == -1 ? 80 : uri.Port;
                string path = uri.PathAndQuery;

                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                DownloadState state = new DownloadState
                {
                    Socket = socket,
                    Host = host,
                    Path = path,
                    ResponseData = new List<byte>(),
                    Parser = new HttpParser(),
                    OnComplete = onComplete,
                    OnError = onError
                };

                IPAddress[] addresses = Dns.GetHostAddresses(host);
                IPEndPoint remoteEP = new IPEndPoint(addresses[0], port);

                socket.BeginConnect(remoteEP, ConnectCallback, state);
            }
            catch (Exception ex)
            {
                onError?.Invoke(url, ex);
            }
        }

        private static void ConnectCallback(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState;
            try
            {
                state.Socket.EndConnect(ar);

                string request = $"GET {state.Path} HTTP/1.1\r\n" +
                                $"Host: {state.Host}\r\n" +
                                "Connection: close\r\n\r\n";

                byte[] requestBytes = Encoding.UTF8.GetBytes(request);
                state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, SendCallback, state);
            }
            catch (Exception ex)
            {
                state.OnError?.Invoke($"{state.Host}{state.Path}", ex);
                state.Socket?.Close();
            }
        }

        private static void SendCallback(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState;
            try
            {
                state.Socket.EndSend(ar);
                byte[] buffer = new byte[4096];
                state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ReceiveCallback, new { State = state, Buffer = buffer });
            }
            catch (Exception ex)
            {
                state.OnError?.Invoke($"{state.Host}{state.Path}", ex);
                state.Socket?.Close();
            }
        }

        private static void ReceiveCallback(IAsyncResult ar)
        {
            dynamic context = ar.AsyncState;
            DownloadState state = context.State;
            byte[] buffer = context.Buffer;

            try
            {
                int bytesReceived = state.Socket.EndReceive(ar);
                if (bytesReceived > 0)
                {
                    state.ResponseData.AddRange(buffer.Take(bytesReceived));

                    // Check if we have received all data based on Content-Length
                    if (!state.Parser.ParseResponse(state.ResponseData.ToArray()))
                    {
                        // Continue receiving
                        state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ReceiveCallback, context);
                    }
                    else
                    {
                        int totalReceived = state.ResponseData.Count;
                        int headerEnd = Encoding.UTF8.GetString(state.ResponseData.ToArray()).IndexOf("\r\n\r\n");
                        int headerLength = headerEnd >= 0 ? Encoding.UTF8.GetBytes(Encoding.UTF8.GetString(state.ResponseData.Take(headerEnd + 4).ToArray())).Length : 0;

                        if (state.Parser.ContentLength > 0)
                        {
                            int bodyReceived = totalReceived - headerLength;
                            if (bodyReceived < state.Parser.ContentLength)
                            {
                                // Continue receiving
                                state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ReceiveCallback, context);
                            }
                            else
                            {
                                // Complete
                                byte[] body = state.Parser.Body ?? state.ResponseData.Skip(headerLength).Take(state.Parser.ContentLength).ToArray();
                                state.OnComplete?.Invoke($"{state.Host}{state.Path}", body);
                                state.Socket.Close();
                            }
                        }
                        else
                        {
                            // No Content-Length, assume connection closed when no more data
                            state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ReceiveCallback, context);
                        }
                    }
                }
                else
                {
                    // Connection closed
                    byte[] fullData = state.ResponseData.ToArray();
                    state.Parser.ParseResponse(fullData);
                    byte[] body = state.Parser.Body ?? new byte[0];
                    state.OnComplete?.Invoke($"{state.Host}{state.Path}", body);
                    state.Socket.Close();
                }
            }
            catch (Exception ex)
            {
                state.OnError?.Invoke($"{state.Host}{state.Path}", ex);
                state.Socket?.Close();
            }
        }
    }

    // Implementation 2: Task-wrapped with ContinueWith()
    public class TaskContinueWithHttpDownloader
    {
        private class DownloadState
        {
            public Socket Socket { get; set; }
            public string Host { get; set; }
            public string Path { get; set; }
            public List<byte> ResponseData { get; set; }
            public HttpParser Parser { get; set; }
        }

        public static Task<byte[]> DownloadFileAsync(string url)
        {
            TaskCompletionSource<byte[]> tcs = new TaskCompletionSource<byte[]>();

            try
            {
                Uri uri = new Uri(url);
                string host = uri.Host;
                int port = uri.Port == -1 ? 80 : uri.Port;
                string path = uri.PathAndQuery;

                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                DownloadState state = new DownloadState
                {
                    Socket = socket,
                    Host = host,
                    Path = path,
                    ResponseData = new List<byte>(),
                    Parser = new HttpParser()
                };

                IPAddress[] addresses = Dns.GetHostAddresses(host);
                IPEndPoint remoteEP = new IPEndPoint(addresses[0], port);

                Task connectTask = ConnectTask(socket, remoteEP);
                connectTask.ContinueWith(connectResult =>
                {
                    if (connectResult.IsFaulted)
                    {
                        tcs.SetException(connectResult.Exception);
                        socket.Close();
                        return;
                    }

                    string request = $"GET {state.Path} HTTP/1.1\r\n" +
                                    $"Host: {state.Host}\r\n" +
                                    "Connection: close\r\n\r\n";

                    byte[] requestBytes = Encoding.UTF8.GetBytes(request);
                    Task sendTask = SendTask(socket, requestBytes);
                    sendTask.ContinueWith(sendResult =>
                    {
                        if (sendResult.IsFaulted)
                        {
                            tcs.SetException(sendResult.Exception);
                            socket.Close();
                            return;
                        }

                        Task<byte[]> receiveTask = ReceiveAllTask(socket, state);
                        receiveTask.ContinueWith(receiveResult =>
                        {
                            if (receiveResult.IsFaulted)
                            {
                                tcs.SetException(receiveResult.Exception);
                            }
                            else
                            {
                                byte[] fullData = receiveResult.Result;
                                state.Parser.ParseResponse(fullData);
                                byte[] body = state.Parser.Body ?? new byte[0];
                                tcs.SetResult(body);
                            }
                            socket.Close();
                        });
                    });
                });

                return tcs.Task;
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
                return tcs.Task;
            }
        }

        private static Task ConnectTask(Socket socket, IPEndPoint remoteEP)
        {
            TaskCompletionSource<object> tcs = new TaskCompletionSource<object>();
            socket.BeginConnect(remoteEP, ar =>
            {
                try
                {
                    socket.EndConnect(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        private static Task SendTask(Socket socket, byte[] data)
        {
            TaskCompletionSource<object> tcs = new TaskCompletionSource<object>();
            socket.BeginSend(data, 0, data.Length, SocketFlags.None, ar =>
            {
                try
                {
                    socket.EndSend(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        private static Task<byte[]> ReceiveAllTask(Socket socket, DownloadState state)
        {
            TaskCompletionSource<byte[]> tcs = new TaskCompletionSource<byte[]>();
            byte[] buffer = new byte[4096];

            Action receiveAction = null;
            receiveAction = () =>
            {
                socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
                {
                    try
                    {
                        int bytesReceived = socket.EndReceive(ar);
                        if (bytesReceived > 0)
                        {
                            state.ResponseData.AddRange(buffer.Take(bytesReceived));

                            // Check if we have all data
                            byte[] currentData = state.ResponseData.ToArray();
                            state.Parser.ParseResponse(currentData);

                            if (state.Parser.ContentLength > 0)
                            {
                                string responseStr = Encoding.UTF8.GetString(currentData);
                                int headerEnd = responseStr.IndexOf("\r\n\r\n");
                                if (headerEnd >= 0)
                                {
                                    int headerLength = Encoding.UTF8.GetBytes(responseStr.Substring(0, headerEnd + 4)).Length;
                                    int bodyReceived = currentData.Length - headerLength;
                                    if (bodyReceived >= state.Parser.ContentLength)
                                    {
                                        tcs.SetResult(currentData);
                                        return;
                                    }
                                }
                            }

                            // Continue receiving
                            receiveAction();
                        }
                        else
                        {
                            // Connection closed
                            tcs.SetResult(state.ResponseData.ToArray());
                        }
                    }
                    catch (Exception ex)
                    {
                        tcs.SetException(ex);
                    }
                }, null);
            };

            receiveAction();
            return tcs.Task;
        }
    }

    // Implementation 3: async/await
    public class AsyncAwaitHttpDownloader
    {
        private class DownloadState
        {
            public Socket Socket { get; set; }
            public string Host { get; set; }
            public string Path { get; set; }
            public List<byte> ResponseData { get; set; }
            public HttpParser Parser { get; set; }
        }

        public static async Task<byte[]> DownloadFileAsync(string url)
        {
            try
            {
                Uri uri = new Uri(url);
                string host = uri.Host;
                int port = uri.Port == -1 ? 80 : uri.Port;
                string path = uri.PathAndQuery;

                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                DownloadState state = new DownloadState
                {
                    Socket = socket,
                    Host = host,
                    Path = path,
                    ResponseData = new List<byte>(),
                    Parser = new HttpParser()
                };

                IPAddress[] addresses = Dns.GetHostAddresses(host);
                IPEndPoint remoteEP = new IPEndPoint(addresses[0], port);

                await ConnectAsync(socket, remoteEP);
                string request = $"GET {state.Path} HTTP/1.1\r\n" +
                                $"Host: {state.Host}\r\n" +
                                "Connection: close\r\n\r\n";

                byte[] requestBytes = Encoding.UTF8.GetBytes(request);
                await SendAsync(socket, requestBytes);
                byte[] fullData = await ReceiveAllAsync(socket, state);
                state.Parser.ParseResponse(fullData);
                byte[] body = state.Parser.Body ?? new byte[0];
                socket.Close();
                return body;
            }
            catch (Exception)
            {
                throw;
            }
        }

        private static Task ConnectAsync(Socket socket, IPEndPoint remoteEP)
        {
            TaskCompletionSource<object> tcs = new TaskCompletionSource<object>();
            socket.BeginConnect(remoteEP, ar =>
            {
                try
                {
                    socket.EndConnect(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        private static Task SendAsync(Socket socket, byte[] data)
        {
            TaskCompletionSource<object> tcs = new TaskCompletionSource<object>();
            socket.BeginSend(data, 0, data.Length, SocketFlags.None, ar =>
            {
                try
                {
                    socket.EndSend(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        private static async Task<byte[]> ReceiveAllAsync(Socket socket, DownloadState state)
        {
            byte[] buffer = new byte[4096];

            while (true)
            {
                int bytesReceived = await ReceiveAsync(socket, buffer);
                if (bytesReceived > 0)
                {
                    state.ResponseData.AddRange(buffer.Take(bytesReceived));

                    // Check if we have all data
                    byte[] currentData = state.ResponseData.ToArray();
                    state.Parser.ParseResponse(currentData);

                    if (state.Parser.ContentLength > 0)
                    {
                        string responseStr = Encoding.UTF8.GetString(currentData);
                        int headerEnd = responseStr.IndexOf("\r\n\r\n");
                        if (headerEnd >= 0)
                        {
                            int headerLength = Encoding.UTF8.GetBytes(responseStr.Substring(0, headerEnd + 4)).Length;
                            int bodyReceived = currentData.Length - headerLength;
                            if (bodyReceived >= state.Parser.ContentLength)
                            {
                                return currentData;
                            }
                        }
                    }
                }
                else
                {
                    // Connection closed
                    return state.ResponseData.ToArray();
                }
            }
        }

        private static Task<int> ReceiveAsync(Socket socket, byte[] buffer)
        {
            TaskCompletionSource<int> tcs = new TaskCompletionSource<int>();
            socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
            {
                try
                {
                    int bytesReceived = socket.EndReceive(ar);
                    tcs.SetResult(bytesReceived);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);
            return tcs.Task;
        }
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            string[] urls = {
                "http://example.com",
                "http://httpbin.org/get",
                "http://www.google.com"
            };

            Console.WriteLine("=== Implementation 1: Direct Callback ===");
            int callbackCompleted = 0;
            foreach (string url in urls)
            {
                CallbackHttpDownloader.DownloadFile(url,
                    (urlResult, data) =>
                    {
                        Console.WriteLine($"✓ Downloaded {urlResult}: {data.Length} bytes");
                        Interlocked.Increment(ref callbackCompleted);
                    },
                    (urlResult, ex) =>
                    {
                        Console.WriteLine($"✗ Error downloading {urlResult}: {ex.Message}");
                        Interlocked.Increment(ref callbackCompleted);
                    });
            }

            // Wait for all callbacks to complete
            while (callbackCompleted < urls.Length)
            {
                Thread.Sleep(100);
            }

            Console.WriteLine("\n=== Implementation 2: Task with ContinueWith() ===");
            List<Task<byte[]>> tasks = new List<Task<byte[]>>();
            foreach (string url in urls)
            {
                tasks.Add(TaskContinueWithHttpDownloader.DownloadFileAsync(url));
            }

            Task.WaitAll(tasks.ToArray());
            for (int i = 0; i < tasks.Count; i++)
            {
                if (tasks[i].Status == TaskStatus.RanToCompletion)
                {
                    Console.WriteLine($"✓ Downloaded {urls[i]}: {tasks[i].Result.Length} bytes");
                }
                else
                {
                    Console.WriteLine($"✗ Error downloading {urls[i]}: {tasks[i].Exception?.InnerException?.Message}");
                }
            }

            Console.WriteLine("\n=== Implementation 3: async/await ===");
            List<Task<byte[]>> asyncTasks = new List<Task<byte[]>>();
            foreach (string url in urls)
            {
                asyncTasks.Add(AsyncAwaitHttpDownloader.DownloadFileAsync(url));
            }

            Task.WaitAll(asyncTasks.ToArray());
            for (int i = 0; i < asyncTasks.Count; i++)
            {
                if (asyncTasks[i].Status == TaskStatus.RanToCompletion)
                {
                    Console.WriteLine($"✓ Downloaded {urls[i]}: {asyncTasks[i].Result.Length} bytes");
                }
                else
                {
                    Console.WriteLine($"✗ Error downloading {urls[i]}: {asyncTasks[i].Exception?.InnerException?.Message}");
                }
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
