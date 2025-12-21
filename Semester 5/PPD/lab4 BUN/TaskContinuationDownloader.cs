using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Lab4
{
    /// <summary>
    /// Task-wrapped operations with continuations (using ContinueWith)
    /// </summary>
    public class TaskContinuationDownloader
    {
        private class DownloadState
        {
            public Socket Socket { get; set; }
            public string Url { get; set; }
            public string Host { get; set; }
            public string Path { get; set; }
            public int Port { get; set; }
            public HttpParser Parser { get; set; }
            public List<byte> ReceivedData { get; set; }
        }

        public static Task<byte[]> DownloadFileAsync(string url)
        {
            try
            {
                // Parse URL to extract components
                Uri uri = new Uri(url);
                string host = uri.Host;
                // Use default ports if not specified (80 for HTTP, 443 for HTTPS)
                int port = uri.Port == -1 ? (uri.Scheme == "https" ? 443 : 80) : uri.Port;
                string path = uri.PathAndQuery;

                // Default to root path if empty
                if (string.IsNullOrEmpty(path))
                    path = "/";

                // Create state object to pass through async operations
                DownloadState state = new DownloadState
                {
                    Url = url,
                    Host = host,
                    Path = path,
                    Port = port,
                    Parser = new HttpParser(),
                    ReceivedData = new List<byte>()
                };

                // Resolve hostname to IP address
                IPHostEntry hostEntry = Dns.GetHostEntry(host);
                IPAddress ipAddress = hostEntry.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                // Create TCP socket
                state.Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                // Start connection and chain operations with ContinueWith
                Task connectTask = ConnectAsync(state, remoteEP);
                // First continuation: after connect, send HTTP request
                return connectTask.ContinueWith(prevTask =>
                {
                    // Check if previous task failed
                    if (prevTask.IsFaulted)
                    {
                        state.Socket?.Close();
                        throw prevTask.Exception.InnerException;
                    }

                    // Return next task (send request)
                    return SendRequestAsync(state);
                }).Unwrap().ContinueWith(prevTask =>
                {
                    // Second continuation: after send, receive data
                    if (prevTask.IsFaulted)
                    {
                        state.Socket?.Close();
                        throw prevTask.Exception.InnerException;
                    }

                    // Return final task (receive data)
                    return ReceiveDataAsync(state);
                }).Unwrap(); // Unwrap nested Task<Task<byte[]>> to Task<byte[]>
            }
            catch (Exception ex)
            {
                return Task.FromException<byte[]>(ex);
            }
        }

        private static Task ConnectAsync(DownloadState state, IPEndPoint remoteEP)
        {
            // Create TaskCompletionSource to wrap async socket operation in a Task
            var tcs = new TaskCompletionSource<object>();
            
            // Start asynchronous connection
            state.Socket.BeginConnect(remoteEP, ar =>
            {
                try
                {
                    // Complete the connection operation
                    state.Socket.EndConnect(ar);
                    // Signal task completion (success)
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    // Signal task failure
                    tcs.SetException(ex);
                }
            }, null);

            // Return the task that will complete when connection is done
            return tcs.Task;
        }

        private static Task SendRequestAsync(DownloadState state)
        {
            // Create TaskCompletionSource to wrap async send operation
            var tcs = new TaskCompletionSource<object>();

            // Build HTTP GET request string
            string request = $"GET {state.Path} HTTP/1.1\r\n" +
                           $"Host: {state.Host}\r\n" +
                           "Connection: close\r\n" +
                           "\r\n";

            // Convert request to bytes
            byte[] requestBytes = Encoding.ASCII.GetBytes(request);

            // Start asynchronous send operation
            state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, ar =>
            {
                try
                {
                    // Complete the send operation
                    state.Socket.EndSend(ar);
                    // Signal task completion (success)
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    // Signal task failure
                    tcs.SetException(ex);
                }
            }, null);

            // Return the task that will complete when send is done
            return tcs.Task;
        }

        private static Task<byte[]> ReceiveDataAsync(DownloadState state)
        {
            // Create TaskCompletionSource to wrap async receive operation
            var tcs = new TaskCompletionSource<byte[]>();
            // Buffer to hold received data (4KB chunks)
            byte[] buffer = new byte[4096];

            // Recursive action to keep receiving until complete
            Action receiveAction = null;
            receiveAction = () =>
            {
                // Start asynchronous receive operation
                state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
                {
                    try
                    {
                        // Complete the receive operation and get bytes received
                        int bytesReceived = state.Socket.EndReceive(ar);

                        // Check if we received data
                        if (bytesReceived > 0)
                        {
                            // Add received bytes to our data list
                            for (int i = 0; i < bytesReceived; i++)
                            {
                                state.ReceivedData.Add(buffer[i]);
                            }

                            // Parse HTTP headers if we haven't finished parsing yet
                            if (!state.Parser.HeadersComplete)
                            {
                                state.Parser.AppendData(buffer, bytesReceived);
                            }

                            // Check if we have received all data based on Content-Length
                            bool allDataReceived = false;
                            if (state.Parser.HeadersComplete)
                            {
                                // Calculate where headers end and body starts
                                int headerEnd = state.Parser.HeaderEndIndex;
                                // Calculate current body length
                                int bodyLength = state.ReceivedData.Count - headerEnd;
                                // If Content-Length header was found, compare
                                if (state.Parser.ContentLength >= 0)
                                {
                                    allDataReceived = bodyLength >= state.Parser.ContentLength;
                                }
                            }

                            // If we received all data, extract body and complete
                            if (allDataReceived)
                            {
                                // Extract body (everything after headers)
                                int headerEnd = state.Parser.HeaderEndIndex;
                                byte[] body = new byte[state.ReceivedData.Count - headerEnd];
                                for (int i = headerEnd; i < state.ReceivedData.Count; i++)
                                {
                                    body[i - headerEnd] = state.ReceivedData[i];
                                }
                                // Close socket and return result
                                state.Socket.Close();
                                tcs.SetResult(body);
                            }
                            else
                            {
                                // Not done yet, continue receiving more data
                                receiveAction();
                            }
                        }
                        else
                        {
                            // bytesReceived == 0 means connection was closed by server
                            // Connection closed
                            if (state.Parser.HeadersComplete)
                            {
                                // Headers were received, extract body from what we got
                                int headerEnd = state.Parser.HeaderEndIndex;
                                byte[] body = new byte[state.ReceivedData.Count - headerEnd];
                                for (int i = headerEnd; i < state.ReceivedData.Count; i++)
                                {
                                    body[i - headerEnd] = state.ReceivedData[i];
                                }
                                // Close socket and return result
                                state.Socket.Close();
                                tcs.SetResult(body);
                            }
                            else
                            {
                                // Headers not complete, this is an error
                                state.Socket.Close();
                                tcs.SetException(new Exception("Connection closed before headers received"));
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        // Handle any errors during receive
                        state.Socket?.Close();
                        tcs.SetException(ex);
                    }
                }, null);
            };

            // Start the first receive operation
            receiveAction();
            // Return the task that will complete when all data is received
            return tcs.Task;
        }
    }
}

