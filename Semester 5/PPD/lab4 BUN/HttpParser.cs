using System;
using System.Text;

namespace Lab4
{
    public class HttpParser
    {
        private StringBuilder buffer = new StringBuilder();
        private bool headersComplete = false;
        private int contentLength = -1;
        private int headerEndIndex = -1;

        public void AppendData(byte[] data, int count)
        {
            // Skip parsing if headers are already complete
            if (headersComplete)
                return;

            // Convert bytes to ASCII string
            string text = Encoding.ASCII.GetString(data, 0, count);
            // Append to buffer (headers may arrive in multiple chunks)
            buffer.Append(text);

            string bufferStr = buffer.ToString();
            int crlfIndex = bufferStr.IndexOf("\r\n\r\n");
            int lfIndex = bufferStr.IndexOf("\n\n");

            // If we found Windows-style header end
            if (crlfIndex >= 0)
            {
                headerEndIndex = crlfIndex + 4;
                headersComplete = true;
                ParseHeaders(bufferStr.Substring(0, crlfIndex + 2));
            }
            else if (lfIndex >= 0)
            {
                headerEndIndex = lfIndex + 2;
                headersComplete = true;
                ParseHeaders(bufferStr.Substring(0, lfIndex + 1));
            }
        }

        private void ParseHeaders(string headerText)
        {
            // Split headers into individual lines
            string[] lines = headerText.Split(new[] { "\r\n", "\n" }, StringSplitOptions.None);

            // Search through each header line
            foreach (string line in lines)
            {
                // Look for Content-Length header (case-insensitive)
                if (line.StartsWith("Content-Length:", StringComparison.OrdinalIgnoreCase))
                {
                    // Extract the value after "Content-Length:"
                    string lengthStr = line.Substring("Content-Length:".Length).Trim();
                    // Parse the value as integer
                    if (int.TryParse(lengthStr, out int length))
                    {
                        // Store the content length for later use
                        contentLength = length;
                    }
                }
            }
        }

        public bool HeadersComplete => headersComplete;
        public int ContentLength => contentLength;
        public int HeaderEndIndex => headerEndIndex;
        public string GetFullBuffer() => buffer.ToString();
    }
}

