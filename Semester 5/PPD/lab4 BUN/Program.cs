using System;
using System.Threading.Tasks;

namespace Lab4
{
    class Program
    {
        static void Main(string[] args)
        {
            // Example URLs to download
            string[] urls = new string[]
            {
                "http://example.com",
                "http://httpbin.org/get",
                "http://www.google.com"
            };

            Console.WriteLine("=== Lab 4: HTTP File Download with TPL ===\n");

            // Test 1: Direct Callback Implementation
            Console.WriteLine("1. Direct Callback Implementation:");
            TestDirectCallback(urls);
            Console.WriteLine();

            // Test 2: Task-Wrapped with Continuations
            Console.WriteLine("2. Task-Wrapped with Continuations:");
            TestTaskContinuations(urls);
            Console.WriteLine();

            // Test 3: Task-Wrapped with Async/Await
            Console.WriteLine("3. Task-Wrapped with Async/Await:");
            TestAsyncAwait(urls);
            Console.WriteLine();

            Console.WriteLine("All tests completed. Press any key to exit...");
            Console.ReadKey();
        }

        static void TestDirectCallback(string[] urls)
        {
            int completed = 0;
            int total = urls.Length;

            foreach (string url in urls)
            {
                DirectCallbackDownloader.DownloadFile(url, (urlResult, data, success) =>
                {
                    completed++;
                    if (success && data != null)
                    {
                        Console.WriteLine($"  ✓ {urlResult}: Downloaded {data.Length} bytes");
                    }
                    else
                    {
                        Console.WriteLine($"  ✗ {urlResult}: Download failed");
                    }

                    if (completed == total)
                    {
                        Console.WriteLine("  All direct callback downloads completed.");
                    }
                });
            }

            // Wait for all downloads to complete
            while (completed < total)
            {
                System.Threading.Thread.Sleep(100);
            }
        }

        static void TestTaskContinuations(string[] urls)
        {
            Task[] tasks = new Task[urls.Length];

            for (int i = 0; i < urls.Length; i++)
            {
                string url = urls[i];
                tasks[i] = TaskContinuationDownloader.DownloadFileAsync(url)
                    .ContinueWith(task =>
                    {
                        if (task.IsCompletedSuccessfully && task.Result != null)
                        {
                            Console.WriteLine($"  ✓ {url}: Downloaded {task.Result.Length} bytes");
                        }
                        else
                        {
                            Console.WriteLine($"  ✗ {url}: Download failed");
                        }
                    });
            }

            // Wait for all tasks to complete (only Wait() allowed in Main)
            Task.WaitAll(tasks);
            Console.WriteLine("  All task continuation downloads completed.");
        }

        static void TestAsyncAwait(string[] urls)
        {
            Task[] tasks = new Task[urls.Length];

            for (int i = 0; i < urls.Length; i++)
            {
                string url = urls[i];
                tasks[i] = DownloadAndPrintAsync(url);
            }

            // Wait for all tasks to complete (only Wait() allowed in Main)
            Task.WaitAll(tasks);
            Console.WriteLine("  All async/await downloads completed.");
        }

        static async Task DownloadAndPrintAsync(string url)
        {
            try
            {
                byte[] data = await AsyncAwaitDownloader.DownloadFileAsync(url);
                if (data != null)
                {
                    Console.WriteLine($"  ✓ {url}: Downloaded {data.Length} bytes");
                }
                else
                {
                    Console.WriteLine($"  ✗ {url}: Download failed");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"  ✗ {url}: Download failed - {ex.Message}");
            }
        }
    }
}

