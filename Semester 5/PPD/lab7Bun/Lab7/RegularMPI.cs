using System;
using System.Linq;
using MPI;

namespace Lab7
{
    /// <summary>
    /// Regular O(n²) polynomial multiplication using MPI
    /// Distributes computation across multiple processes
    /// </summary>
    public static class RegularMPI
    {
        /// <summary>
        /// Multiplies two polynomials using the regular O(n²) algorithm distributed across MPI processes
        /// </summary>
        public static Polynomial Multiply(Intracommunicator comm, Polynomial a, Polynomial b)
        {
            int n = a.Degree + 1;
            int m = b.Degree + 1;
            int resultSize = n + m - 1;

            double[] result = new double[resultSize];

            if (comm.Rank == 0)
            {
                // Root process: broadcast polynomials to all processes
                double[] aCoeffs = a.Coefficients;
                double[] bCoeffs = b.Coefficients;

                // Broadcast polynomial sizes
                int[] sizes = new int[] { n, m };
                comm.Broadcast(ref sizes, 0);

                // Broadcast polynomial coefficients
                comm.Broadcast(ref aCoeffs, 0);
                comm.Broadcast(ref bCoeffs, 0);

                // Distribute work: each process computes a portion of the result
                int numProcesses = comm.Size;
                int chunkSize = n / numProcesses;
                int remainder = n % numProcesses;

                // Send work assignments to each process
                for (int rank = 1; rank < numProcesses; rank++)
                {
                    int start = rank * chunkSize + Math.Min(rank, remainder);
                    int end = start + chunkSize + (rank < remainder ? 1 : 0);
                    int[] range = new int[] { start, end };
                    comm.Send(range, rank, 0);
                }

                // Root process computes its chunk
                int rootStart = 0;
                int rootEnd = chunkSize + (remainder > 0 ? 1 : 0);
                ComputeChunk(aCoeffs, bCoeffs, n, m, rootStart, rootEnd, result);

                // Receive results from other processes
                for (int rank = 1; rank < numProcesses; rank++)
                {
                    double[] partialResult = new double[resultSize];
                    comm.Receive(rank, 0, ref partialResult);

                    // Combine results
                    for (int i = 0; i < resultSize; i++)
                    {
                        result[i] += partialResult[i];
                    }
                }
            }
            else
            {
                // Worker processes: receive polynomials and compute assigned chunk
                int[] sizes = new int[2];
                comm.Broadcast(ref sizes, 0);
                n = sizes[0];
                m = sizes[1];

                double[] aCoeffs = new double[n];
                double[] bCoeffs = new double[m];
                comm.Broadcast(ref aCoeffs, 0);
                comm.Broadcast(ref bCoeffs, 0);

                // Receive work assignment
                int[] range = new int[2];
                comm.Receive(0, 0, ref range);
                int start = range[0];
                int end = range[1];

                // Compute assigned chunk
                double[] partialResult = new double[resultSize];
                ComputeChunk(aCoeffs, bCoeffs, n, m, start, end, partialResult);

                // Send result back to root
                comm.Send(partialResult, 0, 0);
            }

            return new Polynomial(result);
        }

        /// <summary>
        /// Computes a chunk of the result matrix
        /// </summary>
        private static void ComputeChunk(double[] a, double[] b, int n, int m, int start, int end, double[] result)
        {
            for (int i = start; i < end; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    result[i + j] += a[i] * b[j];
                }
            }
        }
    }
}

