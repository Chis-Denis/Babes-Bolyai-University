using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Lab4
{
    public class AsyncAwaitDownloader
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

        public static async Task<byte[]> DownloadFileAsync(string url)
        {
            DownloadState state = null;
            try
            {
                Uri uri = new Uri(url);
                string host = uri.Host;
                int port = uri.Port == -1 ? (uri.Scheme == "https" ? 443 : 80) : uri.Port;
                string path = uri.PathAndQuery;

                if (string.IsNullOrEmpty(path))
                    path = "/";

                state = new DownloadState
                {
                    Url = url,
                    Host = host,
                    Path = path,
                    Port = port,
                    Parser = new HttpParser(),
                    ReceivedData = new List<byte>()
                };

                IPHostEntry hostEntry = Dns.GetHostEntry(host);
                IPAddress ipAddress = hostEntry.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                state.Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                await ConnectAsync(state, remoteEP);
                await SendRequestAsync(state);
                byte[] result = await ReceiveDataAsync(state);
                return result;
            }
            catch (Exception)
            {
                state?.Socket?.Close();
                throw;
            }
        }

        private static Task ConnectAsync(DownloadState state, IPEndPoint remoteEP)
        {
            var tcs = new TaskCompletionSource<object>();
            
            state.Socket.BeginConnect(remoteEP, ar =>
            {
                try
                {
                    state.Socket.EndConnect(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);

            return tcs.Task;
        }

        private static Task SendRequestAsync(DownloadState state)
        {
            var tcs = new TaskCompletionSource<object>();

            string request = $"GET {state.Path} HTTP/1.1\r\n" +
                           $"Host: {state.Host}\r\n" +
                           "Connection: close\r\n" +
                           "\r\n";

            byte[] requestBytes = Encoding.ASCII.GetBytes(request);

            state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, ar =>
            {
                try
                {
                    state.Socket.EndSend(ar);
                    tcs.SetResult(null);
                }
                catch (Exception ex)
                {
                    tcs.SetException(ex);
                }
            }, null);

            return tcs.Task;
        }

        private static async Task<byte[]> ReceiveDataAsync(DownloadState state)
        {
            byte[] buffer = new byte[4096];

            while (true)
            {
                int bytesReceived = await ReceiveChunkAsync(state, buffer);

                if (bytesReceived > 0)
                {
                    // Add received data
                    for (int i = 0; i < bytesReceived; i++)
                    {
                        state.ReceivedData.Add(buffer[i]);
                    }

                    // Parse headers if not complete
                    if (!state.Parser.HeadersComplete)
                    {
                        state.Parser.AppendData(buffer, bytesReceived);
                    }

                    // Check if we've received all data
                    if (state.Parser.HeadersComplete)
                    {
                        int headerEnd = state.Parser.HeaderEndIndex;
                        int bodyLength = state.ReceivedData.Count - headerEnd;
                        if (state.Parser.ContentLength >= 0)
                        {
                            if (bodyLength >= state.Parser.ContentLength)
                            {
                                // Extract body
                                byte[] body = new byte[state.ReceivedData.Count - headerEnd];
                                for (int i = headerEnd; i < state.ReceivedData.Count; i++)
                                {
                                    body[i - headerEnd] = state.ReceivedData[i];
                                }
                                state.Socket.Close();
                                return body;
                            }
                        }
                    }
                }
                else
                {
                    // Connection closed
                    if (state.Parser.HeadersComplete)
                    {
                        int headerEnd = state.Parser.HeaderEndIndex;
                        byte[] body = new byte[state.ReceivedData.Count - headerEnd];
                        for (int i = headerEnd; i < state.ReceivedData.Count; i++)
                        {
                            body[i - headerEnd] = state.ReceivedData[i];
                        }
                        state.Socket.Close();
                        return body;
                    }
                    else
                    {
                        state.Socket.Close();
                        throw new Exception("Connection closed before headers received");
                    }
                }
            }
        }

        private static Task<int> ReceiveChunkAsync(DownloadState state, byte[] buffer)
        {
            var tcs = new TaskCompletionSource<int>();

            state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
            {
                try
                {
                    int bytesReceived = state.Socket.EndReceive(ar);
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
}

