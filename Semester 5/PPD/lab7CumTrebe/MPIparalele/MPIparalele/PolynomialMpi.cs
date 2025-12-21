using System;
using System.Diagnostics;
using MPI;

namespace MPIparalele
{
    public class PolynomialMpi
    {
        // Regular O(n²) polynomial multiplication using MPI
        public static void RegularMultiplyMpi(int[] poly1, int n1, int[] poly2, int n2, 
            int[] result, int resultSize, int rank, int size)
        {
            // Initialize result to zero
            Array.Clear(result, 0, resultSize);
            
            // Calculate chunk size for this process
            int chunkSize = (n1 + size - 1) / size;  // Ceiling division
            int startIdx = rank * chunkSize;
            int endIdx = (startIdx + chunkSize < n1) ? startIdx + chunkSize : n1;
            
            // Each process computes its chunk: poly1[start_idx:end_idx] * poly2
            for (int i = startIdx; i < endIdx; i++)
            {
                for (int j = 0; j < n2; j++)
                {
                    result[i + j] += poly1[i] * poly2[j];
                }
            }
        }

        // Karatsuba algorithm helper functions
        public static void AddPolynomials(int[] poly1, int n1, int[] poly2, int n2, int[] result)
        {
            int maxLen = (n1 > n2) ? n1 : n2;
            Array.Clear(result, 0, maxLen);
            
            for (int i = 0; i < n1; i++)
            {
                result[i] += poly1[i];
            }
            for (int i = 0; i < n2; i++)
            {
                result[i] += poly2[i];
            }
        }

        public static void SubtractPolynomials(int[] poly1, int n1, int[] poly2, int n2, int[] result)
        {
            int maxLen = (n1 > n2) ? n1 : n2;
            Array.Clear(result, 0, maxLen);
            
            for (int i = 0; i < n1; i++)
            {
                result[i] += poly1[i];
            }
            for (int i = 0; i < n2; i++)
            {
                result[i] -= poly2[i];
            }
        }

        // Regular sequential multiplication (used as base case for Karatsuba)
        public static void RegularMultiplySeq(int[] poly1, int n1, int[] poly2, int n2, int[] result)
        {
            int resultSize = n1 + n2 - 1;
            Array.Clear(result, 0, resultSize);
            
            for (int i = 0; i < n1; i++)
            {
                for (int j = 0; j < n2; j++)
                {
                    result[i + j] += poly1[i] * poly2[j];
                }
            }
        }

        // Karatsuba algorithm using MPI
        public static void KaratsubaMpi(int[] poly1, int n1, int[] poly2, int n2, 
            int[] result, int rank, int size, Intracommunicator comm)
        {
            // Base case
            if (n1 <= 16 || n2 <= 16)
            {
                RegularMultiplySeq(poly1, n1, poly2, n2, result);
                return;
            }
            
            // Pad to same length
            int maxLen = (n1 > n2) ? n1 : n2;
            int[] p1 = new int[maxLen];
            int[] p2 = new int[maxLen];
            Array.Copy(poly1, p1, n1);
            Array.Copy(poly2, p2, n2);
            
            if (maxLen % 2 == 1)
            {
                Array.Resize(ref p1, maxLen + 1);
                Array.Resize(ref p2, maxLen + 1);
                p1[maxLen] = 0;
                p2[maxLen] = 0;
                maxLen++;
            }
            
            int mid = maxLen / 2;
            int resultSize = 2 * maxLen - 1;
            Array.Clear(result, 0, resultSize);
            
            // Split
            int[] low1 = new int[mid];
            int[] high1 = new int[mid];
            int[] low2 = new int[mid];
            int[] high2 = new int[mid];
            
            Array.Copy(p1, 0, low1, 0, mid);
            Array.Copy(p1, mid, high1, 0, mid);
            Array.Copy(p2, 0, low2, 0, mid);
            Array.Copy(p2, mid, high2, 0, mid);
            
            // Allocate results
            int z0Size = 2 * mid - 1;
            int z2Size = 2 * mid - 1;
            int[] z0 = new int[z0Size];
            int[] z2 = new int[z2Size];
            
            // Initialize z2 to zero on all processes
            Array.Clear(z2, 0, z2Size);
            
            // Distribute work: rank 0 does z0, rank 1 does z2, rank 0 also does z1
            if (rank == 0)
            {
                KaratsubaMpi(low1, mid, low2, mid, z0, rank, size, comm);
                // If only one process, also compute z2
                if (size == 1)
                {
                    KaratsubaMpi(high1, mid, high2, mid, z2, rank, size, comm);
                }
            }
            else if (rank == 1 && size > 1)
            {
                KaratsubaMpi(high1, mid, high2, mid, z2, rank, size, comm);
            }
            
            // Compute z1 at rank 0
            int[] z1 = null;
            int z1Size = 2 * mid - 1;
            if (rank == 0)
            {
                int[] sum1 = new int[mid];
                int[] sum2 = new int[mid];
                AddPolynomials(low1, mid, high1, mid, sum1);
                AddPolynomials(low2, mid, high2, mid, sum2);
                
                int[] z1Sum = new int[z1Size];
                KaratsubaMpi(sum1, mid, sum2, mid, z1Sum, rank, size, comm);
                
                z1 = new int[z1Size];
                SubtractPolynomials(z1Sum, z1Size, z0, z0Size, z1);
                SubtractPolynomials(z1, z1Size, z2, z2Size, z1);
            }
            
            // Gather z2 from rank 1 if needed
            if (size > 1)
            {
                if (rank == 1)
                {
                    comm.Send(z2, 0, 0);
                }
                else if (rank == 0)
                {
                    comm.Receive(1, 0, out z2);
                }
                // Synchronize all processes
                comm.Barrier();
            }
            
            // Combine at rank 0
            if (rank == 0)
            {
                for (int i = 0; i < z0Size; i++)
                {
                    result[i] += z0[i];
                }
                if (z1 != null)
                {
                    for (int i = 0; i < z1Size; i++)
                    {
                        result[i + mid] += z1[i];
                    }
                }
                for (int i = 0; i < z2Size; i++)
                {
                    result[i + 2 * mid] += z2[i];
                }
            }
            else
            {
                // Other ranks: ensure result buffer is zeroed (already done by Array.Clear)
                // No additional work needed
            }
        }

        public static void Main(string[] args)
        {
            using (new MPI.Environment(ref args))
            {
                Intracommunicator comm = Communicator.world;
                int rank = comm.Rank;
                int size = comm.Size;

                // Example: polynomials of size 1000 with all coefficients = 1
                int n1 = 1000;
                int n2 = 1000;
                string algorithm = "karatsuba";

                if (rank == 0)
                {
                    Console.WriteLine("Running example: Karatsuba with two 1000-element polynomials (all coefficients = 1)");
                }

                // Generate polynomials with all coefficients = 1
                int[] poly1 = null;
                int[] poly2 = null;

                if (rank == 0)
                {
                    poly1 = new int[n1];
                    poly2 = new int[n2];

                    // All coefficients = 1
                    for (int i = 0; i < n1; i++)
                    {
                        poly1[i] = 1;
                    }
                    for (int i = 0; i < n2; i++)
                    {
                        poly2[i] = 1;
                    }

                    Console.Write($"Polynomial 1 (degree {n1 - 1}): ");
                    for (int i = 0; i < n1 && i < 10; i++)
                    {
                        Console.Write($"{poly1[i]} ");
                    }
                    if (n1 > 10) Console.Write("...");
                    Console.WriteLine();

                    Console.Write($"Polynomial 2 (degree {n2 - 1}): ");
                    for (int i = 0; i < n2 && i < 10; i++)
                    {
                        Console.Write($"{poly2[i]} ");
                    }
                    if (n2 > 10) Console.Write("...");
                    Console.WriteLine();
                }

                // Broadcast polynomial sizes
                comm.Broadcast(ref n1, 0);
                comm.Broadcast(ref n2, 0);

                // Allocate space for polynomials on all processes
                if (rank != 0)
                {
                    poly1 = new int[n1];
                    poly2 = new int[n2];
                }

                // Broadcast polynomials
                comm.Broadcast(ref poly1, 0);
                comm.Broadcast(ref poly2, 0);

                int resultSize = n1 + n2 - 1;
                // Allocate extra space for Karatsuba padding (max_len can be padded to even)
                int maxPadded = ((n1 > n2 ? n1 : n2) % 2 == 1) ? (n1 > n2 ? n1 : n2) + 1 : (n1 > n2 ? n1 : n2);
                int safeResultSize = 2 * maxPadded;  // Extra safety margin
                int[] result = new int[safeResultSize];
                int[] partialResult = new int[safeResultSize];

                // Time the computation
                Stopwatch sw = Stopwatch.StartNew();

                if (algorithm == "karatsuba")
                {
                    if (rank == 0)
                    {
                        Console.WriteLine($"Using Karatsuba algorithm with {size} processes");
                    }
                    KaratsubaMpi(poly1, n1, poly2, n2, partialResult, rank, size, comm);
                }
                else
                {
                    if (rank == 0)
                    {
                        Console.WriteLine($"Using Regular O(n²) algorithm with {size} processes");
                    }
                    RegularMultiplyMpi(poly1, n1, poly2, n2, partialResult, resultSize, rank, size);
                }

                // Reduce all partial results
                int root = 0;
                comm.Reduce(partialResult, Operation<int>.Add, result, ref root);

                sw.Stop();
                double elapsed = sw.Elapsed.TotalSeconds;

                if (rank == 0)
                {
                    // Show ALL result elements
                    Console.WriteLine($"\nResult (all {resultSize} coefficients):");
                    for (int i = 0; i < resultSize; i++)
                    {
                        Console.Write($"{result[i]} ");
                        // Print newline every 20 elements for readability
                        if ((i + 1) % 20 == 0) Console.WriteLine();
                    }
                    if (resultSize % 20 != 0) Console.WriteLine();
                    Console.WriteLine($"Time: {elapsed:F6} seconds");
                }
            }
        }
    }
}

