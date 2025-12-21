using System;
using System.Diagnostics;
using System.Text;
using MPI;

namespace MPIparalele
{
    // Big integer represented as array of digits (base 10)
    // digits[0] is the least significant digit
    public class BigInt
    {
        public int[] Digits { get; set; }
        public int Size { get; set; }
        public int Capacity { get; set; }

        // Initialize a BigInt
        public BigInt(int capacity)
        {
            Digits = new int[capacity];
            Size = 0;
            Capacity = capacity;
        }

        // Create BigInt from string
        public static BigInt FromString(string str)
        {
            int len = str.Length;
            BigInt bi = new BigInt(len + 10);
            bi.Size = len;

            // Reverse the string to store least significant digit first
            for (int i = 0; i < len; i++)
            {
                bi.Digits[i] = str[len - 1 - i] - '0';
            }

            return bi;
        }

        // Create BigInt from integer
        public static BigInt FromInt(int value)
        {
            BigInt bi = new BigInt(20);

            if (value == 0)
            {
                bi.Size = 1;
                bi.Digits[0] = 0;
                return bi;
            }

            int idx = 0;
            while (value > 0)
            {
                bi.Digits[idx++] = value % 10;
                value /= 10;
            }
            bi.Size = idx;

            return bi;
        }

        // Convert BigInt to string
        public string ToString()
        {
            StringBuilder sb = new StringBuilder(Size);

            for (int i = 0; i < Size; i++)
            {
                sb.Append((char)(Digits[Size - 1 - i] + '0'));
            }

            return sb.ToString();
        }

        // Normalize BigInt (remove leading zeros, handle carries)
        public void Normalize()
        {
            // Handle carries
            for (int i = 0; i < Capacity - 1; i++)
            {
                if (Digits[i] >= 10)
                {
                    Digits[i + 1] += Digits[i] / 10;
                    Digits[i] %= 10;
                }
            }

            // Update size
            int newSize = Capacity;
            while (newSize > 1 && Digits[newSize - 1] == 0)
            {
                newSize--;
            }
            Size = newSize;
        }
    }

    public class BigIntMpi
    {
        // Regular O(n²) multiplication using MPI
        public static void BigIntMultiplyMpi(BigInt a, BigInt b, BigInt result, int rank, int size)
        {
            int resultCapacity = a.Size + b.Size + 10;

            // Initialize result
            Array.Clear(result.Digits, 0, result.Capacity);
            result.Size = 0;

            // Distribute work: each process multiplies a chunk of a with all of b
            int chunkSize = (a.Size + size - 1) / size;
            int startIdx = rank * chunkSize;
            int endIdx = (startIdx + chunkSize < a.Size) ? startIdx + chunkSize : a.Size;

            // Each process computes partial result
            for (int i = startIdx; i < endIdx; i++)
            {
                for (int j = 0; j < b.Size; j++)
                {
                    result.Digits[i + j] += a.Digits[i] * b.Digits[j];
                }
            }
        }

        // Regular sequential multiplication (for comparison)
        public static void BigIntMultiplySeq(BigInt a, BigInt b, BigInt result)
        {
            int resultCapacity = a.Size + b.Size + 10;

            Array.Clear(result.Digits, 0, result.Capacity);
            result.Size = 0;

            for (int i = 0; i < a.Size; i++)
            {
                for (int j = 0; j < b.Size; j++)
                {
                    result.Digits[i + j] += a.Digits[i] * b.Digits[j];
                }
            }

            result.Normalize();
        }

        // Karatsuba algorithm for big integers
        public static void BigIntKaratsubaMpi(BigInt a, BigInt b, BigInt result, int rank, int size, Intracommunicator comm)
        {
            // Base case: use regular multiplication for small numbers
            if (a.Size <= 16 || b.Size <= 16)
            {
                BigIntMultiplySeq(a, b, result);
                result.Normalize();
                return;
            }

            // Make both numbers same length
            int maxLen = (a.Size > b.Size) ? a.Size : b.Size;

            // Pad to even length
            if (maxLen % 2 == 1)
            {
                maxLen++;
            }

            // Split at midpoint
            int mid = maxLen / 2;

            // Create split BigInts
            BigInt aLow = new BigInt(mid + 10);
            BigInt aHigh = new BigInt(mid + 10);
            BigInt bLow = new BigInt(mid + 10);
            BigInt bHigh = new BigInt(mid + 10);

            // Copy digits
            int aLowSize = (mid < a.Size) ? mid : a.Size;
            int aHighSize = (a.Size > mid) ? a.Size - mid : 0;
            int bLowSize = (mid < b.Size) ? mid : b.Size;
            int bHighSize = (b.Size > mid) ? b.Size - mid : 0;

            Array.Copy(a.Digits, 0, aLow.Digits, 0, aLowSize);
            aLow.Size = aLowSize;
            if (aHighSize > 0)
            {
                Array.Copy(a.Digits, mid, aHigh.Digits, 0, aHighSize);
                aHigh.Size = aHighSize;
            }
            else
            {
                aHigh.Size = 1;
                aHigh.Digits[0] = 0;
            }

            Array.Copy(b.Digits, 0, bLow.Digits, 0, bLowSize);
            bLow.Size = bLowSize;
            if (bHighSize > 0)
            {
                Array.Copy(b.Digits, mid, bHigh.Digits, 0, bHighSize);
                bHigh.Size = bHighSize;
            }
            else
            {
                bHigh.Size = 1;
                bHigh.Digits[0] = 0;
            }

            // Allocate space for z0, z1, z2
            int zSize = 2 * mid + 10;
            BigInt z0 = new BigInt(zSize);
            BigInt z2 = new BigInt(zSize);

            // Distribute work: rank 0 does z0, rank 1 does z2
            if (rank == 0)
            {
                BigIntKaratsubaMpi(aLow, bLow, z0, rank, size, comm);
                // If only one process, also compute z2
                if (size == 1)
                {
                    BigIntKaratsubaMpi(aHigh, bHigh, z2, rank, size, comm);
                }
            }
            else if (rank == 1 && size > 1)
            {
                BigIntKaratsubaMpi(aHigh, bHigh, z2, rank, size, comm);
            }

            // Compute z1 at rank 0
            BigInt z1 = null;
            if (rank == 0)
            {
                // (a_low + a_high) * (b_low + b_high)
                BigInt aSum = new BigInt(mid + 10);
                BigInt bSum = new BigInt(mid + 10);

                // Add a_low + a_high
                int maxA = (aLow.Size > aHigh.Size) ? aLow.Size : aHigh.Size;
                for (int i = 0; i < maxA; i++)
                {
                    int val = 0;
                    if (i < aLow.Size) val += aLow.Digits[i];
                    if (i < aHigh.Size) val += aHigh.Digits[i];
                    aSum.Digits[i] = val;
                }
                aSum.Size = maxA;
                aSum.Normalize();

                // Add b_low + b_high
                int maxB = (bLow.Size > bHigh.Size) ? bLow.Size : bHigh.Size;
                for (int i = 0; i < maxB; i++)
                {
                    int val = 0;
                    if (i < bLow.Size) val += bLow.Digits[i];
                    if (i < bHigh.Size) val += bHigh.Digits[i];
                    bSum.Digits[i] = val;
                }
                bSum.Size = maxB;
                bSum.Normalize();

                BigInt z1Sum = new BigInt(zSize);
                BigIntKaratsubaMpi(aSum, bSum, z1Sum, rank, size, comm);

                // z1 = z1_sum - z0 - z2
                z1 = new BigInt(zSize);
                Array.Copy(z1Sum.Digits, z1.Digits, z1Sum.Size);
                z1.Size = z1Sum.Size;

                // Subtract z0
                for (int i = 0; i < z0.Size; i++)
                {
                    z1.Digits[i] -= z0.Digits[i];
                }

                // Subtract z2
                for (int i = 0; i < z2.Size; i++)
                {
                    z1.Digits[i] -= z2.Digits[i];
                }

                z1.Normalize();
            }

            // Gather z2 from rank 1
            if (size > 1)
            {
                if (rank == 1)
                {
                    comm.Send(z2.Size, 0, 0);
                    int[] z2Digits = new int[z2.Size];
                    Array.Copy(z2.Digits, z2Digits, z2.Size);
                    comm.Send(z2Digits, 0, 1);
                }
                else if (rank == 0)
                {
                    int z2Size;
                    comm.Receive(1, 0, out z2Size);
                    if (z2Size > z2.Capacity)
                    {
                        int[] newDigits = new int[z2Size];
                        Array.Copy(z2.Digits, newDigits, Math.Min(z2.Size, z2Size));
                        z2.Digits = newDigits;
                        z2.Capacity = z2Size;
                    }
                    int[] z2Digits = new int[z2Size];
                    comm.Receive(1, 1, out z2Digits);
                    Array.Copy(z2Digits, z2.Digits, z2Size);
                    z2.Size = z2Size;
                }
            }

            // Combine results at rank 0: result = z0 + z1*10^mid + z2*10^(2*mid)
            if (rank == 0)
            {
                Array.Clear(result.Digits, 0, result.Capacity);
                result.Size = 0;

                // Add z0
                for (int i = 0; i < z0.Size; i++)
                {
                    result.Digits[i] += z0.Digits[i];
                }

                // Add z1 shifted by mid
                if (z1 != null)
                {
                    for (int i = 0; i < z1.Size; i++)
                    {
                        result.Digits[i + mid] += z1.Digits[i];
                    }
                }

                // Add z2 shifted by 2*mid
                for (int i = 0; i < z2.Size; i++)
                {
                    result.Digits[i + 2 * mid] += z2.Digits[i];
                }

                result.Normalize();
            }
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
                        Console.WriteLine($"Usage: {System.Reflection.Assembly.GetExecutingAssembly().GetName().Name} <num1> <num2> [algorithm]");
                        Console.WriteLine("  num1, num2: big integers as strings");
                        Console.WriteLine("  algorithm: 'regular' or 'karatsuba' (default: regular)");
                    }
                    return;
                }

                string num1Str = args[0];
                string num2Str = args[1];
                string algorithm = (args.Length > 2) ? args[2] : "regular";

                BigInt a = null;
                BigInt b = null;
                BigInt result = null;

                if (rank == 0)
                {
                    a = BigInt.FromString(num1Str);
                    b = BigInt.FromString(num2Str);

                    Console.WriteLine($"Number 1: {num1Str}");
                    Console.WriteLine($"Number 2: {num2Str}");
                }

                // Broadcast sizes
                int aSize = 0, bSize = 0;
                if (rank == 0)
                {
                    aSize = a.Size;
                    bSize = b.Size;
                }
                comm.Broadcast(ref aSize, 0);
                comm.Broadcast(ref bSize, 0);

                // Allocate and broadcast BigInts
                if (rank != 0)
                {
                    a = new BigInt(aSize + 10);
                    b = new BigInt(bSize + 10);
                    a.Size = aSize;
                    b.Size = bSize;
                }

                int[] aDigits = new int[aSize];
                int[] bDigits = new int[bSize];
                if (rank == 0)
                {
                    Array.Copy(a.Digits, aDigits, aSize);
                    Array.Copy(b.Digits, bDigits, bSize);
                }
                comm.Broadcast(ref aDigits, 0);
                comm.Broadcast(ref bDigits, 0);
                if (rank != 0)
                {
                    Array.Copy(aDigits, a.Digits, aSize);
                    Array.Copy(bDigits, b.Digits, bSize);
                }

                int resultCapacity = aSize + bSize + 100;
                result = new BigInt(resultCapacity);

                // Time the computation
                Stopwatch sw = Stopwatch.StartNew();

                if (algorithm == "karatsuba")
                {
                    if (rank == 0)
                    {
                        Console.WriteLine($"Using Karatsuba algorithm with {size} processes");
                    }
                    BigIntKaratsubaMpi(a, b, result, rank, size, comm);
                }
                else
                {
                    if (rank == 0)
                    {
                        Console.WriteLine($"Using Regular O(n²) algorithm with {size} processes");
                    }

                    BigInt partialResult = new BigInt(resultCapacity);
                    BigIntMultiplyMpi(a, b, partialResult, rank, size);

                    // Reduce partial results
                    int[] partialDigits = new int[resultCapacity];
                    Array.Copy(partialResult.Digits, partialDigits, resultCapacity);
                    int[] resultDigits = new int[resultCapacity];
                    int root = 0;
                    comm.Reduce(partialDigits, Operation<int>.Add, resultDigits, ref root);

                    if (rank == 0)
                    {
                        Array.Copy(resultDigits, result.Digits, resultCapacity);
                        result.Normalize();
                    }
                }

                sw.Stop();
                double elapsed = sw.Elapsed.TotalSeconds;

                if (rank == 0)
                {
                    string resultStr = result.ToString();
                    Console.WriteLine($"\nResult: {resultStr}");
                    Console.WriteLine($"Time: {elapsed:F6} seconds");
                }
            }
        }
    }
}

