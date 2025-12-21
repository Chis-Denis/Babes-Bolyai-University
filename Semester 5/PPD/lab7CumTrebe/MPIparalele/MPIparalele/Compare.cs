using System;
using System.Diagnostics;
using MPI;

namespace MPIparalele
{
    public class Compare
    {
        // Regular O(n²) polynomial multiplication using MPI
        public static void RegularMultiplyMpi(int[] poly1, int n1, int[] poly2, int n2, 
            int[] result, int resultSize, int rank, int size)
        {
            Array.Clear(result, 0, resultSize);
            
            int chunkSize = (n1 + size - 1) / size;
            int startIdx = rank * chunkSize;
            int endIdx = (startIdx + chunkSize < n1) ? startIdx + chunkSize : n1;
            
            for (int i = startIdx; i < endIdx; i++)
            {
                for (int j = 0; j < n2; j++)
                {
                    result[i + j] += poly1[i] * poly2[j];
                }
            }
        }

        // Regular sequential multiplication (for comparison)
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

        // Helper functions for Karatsuba
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

        // Karatsuba sequential (for comparison)
        public static void KaratsubaSeq(int[] poly1, int n1, int[] poly2, int n2, int[] result)
        {
            if (n1 <= 16 || n2 <= 16)
            {
                RegularMultiplySeq(poly1, n1, poly2, n2, result);
                return;
            }
            
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
            
            int[] low1 = new int[mid];
            int[] high1 = new int[mid];
            int[] low2 = new int[mid];
            int[] high2 = new int[mid];
            
            Array.Copy(p1, 0, low1, 0, mid);
            Array.Copy(p1, mid, high1, 0, mid);
            Array.Copy(p2, 0, low2, 0, mid);
            Array.Copy(p2, mid, high2, 0, mid);
            
            int z0Size = 2 * mid - 1;
            int z1Size = 2 * mid - 1;
            int z2Size = 2 * mid - 1;
            int[] z0 = new int[z0Size];
            int[] z1 = new int[z1Size];
            int[] z2 = new int[z2Size];
            
            KaratsubaSeq(low1, mid, low2, mid, z0);
            KaratsubaSeq(high1, mid, high2, mid, z2);
            
            int[] sum1 = new int[mid];
            int[] sum2 = new int[mid];
            AddPolynomials(low1, mid, high1, mid, sum1);
            AddPolynomials(low2, mid, high2, mid, sum2);
            
            int[] z1Sum = new int[z1Size];
            KaratsubaSeq(sum1, mid, sum2, mid, z1Sum);
            
            SubtractPolynomials(z1Sum, z1Size, z0, z0Size, z1);
            SubtractPolynomials(z1, z1Size, z2, z2Size, z1);
            
            for (int i = 0; i < z0Size; i++)
            {
                result[i] += z0[i];
            }
            for (int i = 0; i < z1Size; i++)
            {
                result[i + mid] += z1[i];
            }
            for (int i = 0; i < z2Size; i++)
            {
                result[i + 2 * mid] += z2[i];
            }
        }

        // Karatsuba MPI
        public static void KaratsubaMpi(int[] poly1, int n1, int[] poly2, int n2, 
            int[] result, int rank, int size, Intracommunicator comm)
        {
            if (n1 <= 16 || n2 <= 16)
            {
                RegularMultiplySeq(poly1, n1, poly2, n2, result);
                return;
            }
            
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
            
            int[] low1 = new int[mid];
            int[] high1 = new int[mid];
            int[] low2 = new int[mid];
            int[] high2 = new int[mid];
            
            Array.Copy(p1, 0, low1, 0, mid);
            Array.Copy(p1, mid, high1, 0, mid);
            Array.Copy(p2, 0, low2, 0, mid);
            Array.Copy(p2, mid, high2, 0, mid);
            
            int z0Size = 2 * mid - 1;
            int z2Size = 2 * mid - 1;
            int[] z0 = new int[z0Size];
            int[] z2 = new int[z2Size];
            
            // Distribute work: rank 0 does z0, rank 1 does z2
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
            
            // Gather z2 from rank 1
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
        }

        // Compare results
        public static bool CompareResults(int[] result1, int[] result2, int size)
        {
            for (int i = 0; i < size; i++)
            {
                if (result1[i] != result2[i])
                {
                    return false;
                }
            }
            return true;
        }

        public static void Main(string[] args)
        {
            using (new MPI.Environment(ref args))
            {
                Intracommunicator comm = Communicator.world;
                int rank = comm.Rank;
                int size = comm.Size;

                if (args.Length < 2)
                {
                    if (rank == 0)
                    {
                        Console.WriteLine("Usage: Compare <n1> <n2> [num_runs]");
                        Console.WriteLine("  n1, n2: sizes of the two polynomials");
                        Console.WriteLine("  num_runs: number of benchmark runs (default: 3)");
                    }
                    return;
                }

                int n1 = int.Parse(args[0]);
                int n2 = int.Parse(args[1]);
                int numRuns = (args.Length > 2) ? int.Parse(args[2]) : 3;

                // Generate random polynomials
                int[] poly1 = null;
                int[] poly2 = null;

                if (rank == 0)
                {
                    poly1 = new int[n1];
                    poly2 = new int[n2];

                    Random rand = new Random(42);  // Fixed seed for reproducibility
                    for (int i = 0; i < n1; i++)
                    {
                        poly1[i] = rand.Next(-10, 11);
                    }
                    for (int i = 0; i < n2; i++)
                    {
                        poly2[i] = rand.Next(-10, 11);
                    }
                }

                // Broadcast sizes
                comm.Broadcast(ref n1, 0);
                comm.Broadcast(ref n2, 0);

                if (rank != 0)
                {
                    poly1 = new int[n1];
                    poly2 = new int[n2];
                }

                comm.Broadcast(ref poly1, 0);
                comm.Broadcast(ref poly2, 0);

                int resultSize = n1 + n2 - 1;

                if (rank == 0)
                {
                    Console.WriteLine("\n======================================================================");
                    Console.WriteLine("Polynomial Multiplication Benchmark");
                    Console.WriteLine($"Poly1 degree: {n1 - 1}, Poly2 degree: {n2 - 1}");
                    Console.WriteLine($"MPI Processes: {size}");
                    Console.WriteLine("======================================================================\n");
                }

                // Benchmark regular sequential (only on rank 0)
                double regularSeqTime = 0.0;
                int[] regularSeqResult = null;
                if (rank == 0)
                {
                    regularSeqResult = new int[resultSize];
                    Console.WriteLine("Testing Regular O(n²) Sequential...");

                    for (int run = 0; run < numRuns; run++)
                    {
                        Stopwatch sw = Stopwatch.StartNew();
                        RegularMultiplySeq(poly1, n1, poly2, n2, regularSeqResult);
                        sw.Stop();
                        regularSeqTime += sw.Elapsed.TotalSeconds;
                    }
                    regularSeqTime /= numRuns;
                    Console.WriteLine($"  Average time: {regularSeqTime:F6} seconds\n");
                }

                // Benchmark regular MPI
                double regularMpiTime = 0.0;
                int[] regularMpiResult = new int[resultSize];
                int[] regularMpiPartial = new int[resultSize];

                if (rank == 0)
                {
                    Console.WriteLine("Testing Regular O(n²) MPI...");
                }

                for (int run = 0; run < numRuns; run++)
                {
                    comm.Barrier();
                    Stopwatch sw = Stopwatch.StartNew();

                    Array.Clear(regularMpiPartial, 0, resultSize);
                    RegularMultiplyMpi(poly1, n1, poly2, n2, regularMpiPartial, resultSize, rank, size);
                    int root = 0;
                    comm.Reduce(regularMpiPartial, Operation<int>.Add, regularMpiResult, ref root);

                    sw.Stop();
                    if (rank == 0)
                    {
                        regularMpiTime += sw.Elapsed.TotalSeconds;
                    }
                }

                if (rank == 0)
                {
                    regularMpiTime /= numRuns;
                    Console.WriteLine($"  Average time: {regularMpiTime:F6} seconds");
                    Console.WriteLine($"  Speedup: {regularSeqTime / regularMpiTime:F2}x\n");
                }

                // Benchmark Karatsuba sequential (only on rank 0)
                double karatsubaSeqTime = 0.0;
                int[] karatsubaSeqResult = null;
                if (rank == 0)
                {
                    karatsubaSeqResult = new int[resultSize];
                    Console.WriteLine("Testing Karatsuba Sequential...");

                    for (int run = 0; run < numRuns; run++)
                    {
                        Stopwatch sw = Stopwatch.StartNew();
                        KaratsubaSeq(poly1, n1, poly2, n2, karatsubaSeqResult);
                        sw.Stop();
                        karatsubaSeqTime += sw.Elapsed.TotalSeconds;
                    }
                    karatsubaSeqTime /= numRuns;
                    Console.WriteLine($"  Average time: {karatsubaSeqTime:F6} seconds\n");
                }

                // Benchmark Karatsuba MPI
                double karatsubaMpiTime = 0.0;
                int[] karatsubaMpiResult = new int[resultSize];

                if (rank == 0)
                {
                    Console.WriteLine("Testing Karatsuba MPI...");
                }

                for (int run = 0; run < numRuns; run++)
                {
                    comm.Barrier();
                    Stopwatch sw = Stopwatch.StartNew();

                    Array.Clear(karatsubaMpiResult, 0, resultSize);
                    KaratsubaMpi(poly1, n1, poly2, n2, karatsubaMpiResult, rank, size, comm);

                    sw.Stop();
                    if (rank == 0)
                    {
                        karatsubaMpiTime += sw.Elapsed.TotalSeconds;
                    }
                }

                if (rank == 0)
                {
                    karatsubaMpiTime /= numRuns;
                    Console.WriteLine($"  Average time: {karatsubaMpiTime:F6} seconds");
                    Console.WriteLine($"  Speedup: {karatsubaSeqTime / karatsubaMpiTime:F2}x\n");
                }

                // Verify correctness
                if (rank == 0)
                {
                    Console.WriteLine("Verifying correctness...");
                    bool allMatch = true;

                    if (!CompareResults(regularSeqResult, regularMpiResult, resultSize))
                    {
                        Console.WriteLine("  ERROR: Regular MPI result doesn't match sequential!");
                        allMatch = false;
                    }

                    if (!CompareResults(regularSeqResult, karatsubaSeqResult, resultSize))
                    {
                        Console.WriteLine("  ERROR: Karatsuba sequential result doesn't match!");
                        allMatch = false;
                    }

                    if (!CompareResults(regularSeqResult, karatsubaMpiResult, resultSize))
                    {
                        Console.WriteLine("  ERROR: Karatsuba MPI result doesn't match!");
                        allMatch = false;
                    }

                    if (allMatch)
                    {
                        Console.WriteLine("  [OK] All results match!\n");
                    }

                    // Print summary
                    Console.WriteLine("======================================================================");
                    Console.WriteLine("Performance Summary:");
                    Console.WriteLine("======================================================================");
                    Console.WriteLine("{0,-30} {1,15} {2,15}", "Algorithm", "Time (s)", "Speedup vs Seq");
                    Console.WriteLine("----------------------------------------------------------------------");

                    Console.WriteLine("{0,-30} {1,15:F6} {2,15}", "Regular Sequential", regularSeqTime, "1.00x");
                    Console.WriteLine("{0,-30} {1,15:F6} {2,15:F2}x", "Regular MPI", regularMpiTime, regularSeqTime / regularMpiTime);
                    Console.WriteLine("{0,-30} {1,15:F6} {2,15:F2}x", "Karatsuba Sequential", karatsubaSeqTime, regularSeqTime / karatsubaSeqTime);
                    Console.WriteLine("{0,-30} {1,15:F6} {2,15:F2}x", "Karatsuba MPI", karatsubaMpiTime, regularSeqTime / karatsubaMpiTime);
                    Console.WriteLine("======================================================================\n");
                }
            }
        }
    }
}

