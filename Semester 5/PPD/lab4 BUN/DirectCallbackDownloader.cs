using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;

namespace Lab4
{
    public class DirectCallbackDownloader
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
            public Action<string, byte[], bool> Callback { get; set; }
        }

        public static void DownloadFile(string url, Action<string, byte[], bool> callback)
        {
            try
            {
                Uri uri = new Uri(url);
                string host = uri.Host;
                int port = uri.Port == -1 ? (uri.Scheme == "https" ? 443 : 80) : uri.Port;
                string path = uri.PathAndQuery;

                if (string.IsNullOrEmpty(path))
                    path = "/";

                DownloadState state = new DownloadState
                {
                    Url = url,
                    Host = host,
                    Path = path,
                    Port = port,
                    Parser = new HttpParser(),
                    ReceivedData = new List<byte>(),
                    Callback = callback
                };

                IPHostEntry hostEntry = Dns.GetHostEntry(host);
                IPAddress ipAddress = hostEntry.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                state.Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                state.Socket.BeginConnect(remoteEP, OnConnect, state);
            }
            catch (Exception)
            {
                callback(url, null, false);
            }
        }

        private static void OnConnect(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState;
            try
            {
                state.Socket.EndConnect(ar);

                // Build HTTP request
                string request = $"GET {state.Path} HTTP/1.1\r\n" +
                               $"Host: {state.Host}\r\n" +
                               "Connection: close\r\n" +
                               "\r\n";

                byte[] requestBytes = Encoding.ASCII.GetBytes(request);
                state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, OnSend, state);
            }
            catch (Exception)
            {
                state.Callback(state.Url, null, false);
                state.Socket?.Close();
            }
        }

        private class ReceiveState
        {
            public DownloadState State { get; set; }
            public byte[] Buffer { get; set; }
        }

        private static void OnSend(IAsyncResult ar)
        {
            DownloadState state = (DownloadState)ar.AsyncState;
            try
            {
                state.Socket.EndSend(ar);
                byte[] buffer = new byte[4096];
                state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, OnReceive, new ReceiveState { State = state, Buffer = buffer });
            }
            catch (Exception)
            {
                state.Callback(state.Url, null, false);
                state.Socket?.Close();
            }
        }

        private static void OnReceive(IAsyncResult ar)
        {
            ReceiveState receiveState = (ReceiveState)ar.AsyncState;
            DownloadState state = receiveState.State;
            byte[] buffer = receiveState.Buffer;

            try
            {
                int bytesReceived = state.Socket.EndReceive(ar);

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
                    bool allDataReceived = false;
                    if (state.Parser.HeadersComplete)
                    {
                        int headerEnd = state.Parser.HeaderEndIndex;
                        int bodyLength = state.ReceivedData.Count - headerEnd;
                        if (state.Parser.ContentLength >= 0)
                        {
                            allDataReceived = bodyLength >= state.Parser.ContentLength;
                        }
                        else
                        {
                            // No Content-Length, continue receiving until connection closes
                            allDataReceived = false;
                        }
                    }

                    if (allDataReceived)
                    {
                        // Extract body
                        int headerEnd = state.Parser.HeaderEndIndex;
                        byte[] body = new byte[state.ReceivedData.Count - headerEnd];
                        for (int i = headerEnd; i < state.ReceivedData.Count; i++)
                        {
                            body[i - headerEnd] = state.ReceivedData[i];
                        }
                        state.Callback(state.Url, body, true);
                        state.Socket.Close();
                    }
                    else
                    {
                        // Continue receiving
                        state.Socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, OnReceive, receiveState);
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
                        state.Callback(state.Url, body, true);
                    }
                    else
                    {
                        state.Callback(state.Url, null, false);
                    }
                    state.Socket.Close();
                }
            }
            catch (Exception)
            {
                state.Callback(state.Url, null, false);
                state.Socket?.Close();
            }
        }
    }
}

