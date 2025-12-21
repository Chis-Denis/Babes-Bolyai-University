using System;
using System.Linq;
using MPI;

namespace Lab7
{
    public static class KaratsubaMPI
    {
        public static Polynomial Multiply(Intracommunicator comm, Polynomial a, Polynomial b)
        {
            // Base case: use regular multiplication for small polynomials
            if (a.Degree < 10 || b.Degree < 10)
            {
                return RegularMPI.Multiply(comm, a, b);
            }

            int n = Math.Max(a.Degree + 1, b.Degree + 1);
            int half = n / 2;

            // Split polynomials into high and low parts
            Polynomial aLow = new Polynomial(a.Coefficients.Take(half).ToArray());
            Polynomial aHigh = new Polynomial(a.Coefficients.Skip(half).ToArray());
            Polynomial bLow = new Polynomial(b.Coefficients.Take(half).ToArray());
            Polynomial bHigh = new Polynomial(b.Coefficients.Skip(half).ToArray());

            // Pad to same length
            while (aHigh.Degree < aLow.Degree)
                aHigh = new Polynomial(new double[] { 0 }.Concat(aHigh.Coefficients).ToArray());
            while (bHigh.Degree < bLow.Degree)
                bHigh = new Polynomial(new double[] { 0 }.Concat(bHigh.Coefficients).ToArray());

            Polynomial? z0, z1, z2;

            int numProcesses = comm.Size;
            int rank = comm.Rank;

            if (numProcesses >= 3 && n > 50)
            {
                // Distribute the three recursive calls across processes
                if (rank == 0)
                {
                    // Root process coordinates
                    z0 = MultiplyRecursive(comm, aLow, bLow, 1);
                    z2 = MultiplyRecursive(comm, aHigh, bHigh, 2);

                    // Compute sums
                    Polynomial aSum = Add(aLow, aHigh);
                    Polynomial bSum = Add(bLow, bHigh);
                    z1 = MultiplyRecursive(comm, aSum, bSum, 3);
                }
                else if (rank == 1)
                {
                    // Process 1: compute z0 = aLow * bLow
                    z0 = MultiplyRecursive(comm, aLow, bLow, 1);
                    z1 = null;
                    z2 = null;
                }
                else if (rank == 2)
                {
                    // Process 2: compute z2 = aHigh * bHigh
                    z0 = null;
                    z1 = null;
                    z2 = MultiplyRecursive(comm, aHigh, bHigh, 2);
                }
                else if (rank == 3 && numProcesses > 3)
                {
                    // Process 3: compute z1 = (aLow + aHigh) * (bLow + bHigh)
                    Polynomial aSum = Add(aLow, aHigh);
                    Polynomial bSum = Add(bLow, bHigh);
                    z0 = null;
                    z1 = MultiplyRecursive(comm, aSum, bSum, 3);
                    z2 = null;
                }
                else
                {
                    // Other processes: help with computation if needed
                    z0 = null;
                    z1 = null;
                    z2 = null;
                }

                // Gather results at root
                if (rank == 0)
                {
                    // Receive z0 from process 1 if not computed locally
                    if (numProcesses > 1)
                    {
                        double[] z0Coeffs = new double[aLow.Degree + bLow.Degree + 1];
                        comm.Receive(1, 1, ref z0Coeffs);
                        z0 = new Polynomial(z0Coeffs);
                    }

                    // Receive z2 from process 2 if not computed locally
                    if (numProcesses > 2)
                    {
                        double[] z2Coeffs = new double[aHigh.Degree + bHigh.Degree + 1];
                        comm.Receive(2, 2, ref z2Coeffs);
                        z2 = new Polynomial(z2Coeffs);
                    }

                    // Receive z1 from process 3 if not computed locally
                    if (numProcesses > 3)
                    {
                        Polynomial aSum = Add(aLow, aHigh);
                        Polynomial bSum = Add(bLow, bHigh);
                        double[] z1Coeffs = new double[aSum.Degree + bSum.Degree + 1];
                        comm.Receive(3, 3, ref z1Coeffs);
                        z1 = new Polynomial(z1Coeffs);
                    }
                }
                else
                {
                    // Send results to root
                    if (rank == 1 && z0 != null)
                    {
                        comm.Send(z0.Coefficients, 0, 1);
                    }
                    else if (rank == 2 && z2 != null)
                    {
                        comm.Send(z2.Coefficients, 0, 2);
                    }
                    else if (rank == 3 && z1 != null)
                    {
                        comm.Send(z1.Coefficients, 0, 3);
                    }
                }
            }
            else
            {
                // Not enough processes or polynomial too small: sequential computation
                z0 = Multiply(comm, aLow, bLow);
                z2 = Multiply(comm, aHigh, bHigh);

                Polynomial aSum = Add(aLow, aHigh);
                Polynomial bSum = Add(bLow, bHigh);
                z1 = Multiply(comm, aSum, bSum);
            }

            // Only root process combines results
            if (rank == 0 && z0 != null && z1 != null && z2 != null)
            {
                // z1 = z1 - z0 - z2
                z1 = Subtract(Subtract(z1, z0), z2);

                // Result = z0 + z1*x^half + z2*x^(2*half)
                Polynomial result = Add(z0, Shift(z1, half));
                result = Add(result, Shift(z2, 2 * half));

                return result;
            }

            return new Polynomial(new double[] { 0 });
        }


        private static Polynomial MultiplyRecursive(Intracommunicator comm, Polynomial a, Polynomial b, int tag)
        {
            if (a.Degree < 10 || b.Degree < 10)
            {
                return RegularMPI.Multiply(comm, a, b);
            }

            return Multiply(comm, a, b);
        }

        private static Polynomial Add(Polynomial a, Polynomial b)
        {
            int maxLen = Math.Max(a.Degree + 1, b.Degree + 1);
            double[] result = new double[maxLen];

            for (int i = 0; i < maxLen; i++)
            {
                result[i] = a[i] + b[i];
            }

            return new Polynomial(result);
        }

        private static Polynomial Subtract(Polynomial a, Polynomial b)
        {
            int maxLen = Math.Max(a.Degree + 1, b.Degree + 1);
            double[] result = new double[maxLen];

            for (int i = 0; i < maxLen; i++)
            {
                result[i] = a[i] - b[i];
            }

            return new Polynomial(result);
        }

        private static Polynomial Shift(Polynomial p, int shift)
        {
            if (shift == 0) return p;

            double[] result = new double[p.Degree + 1 + shift];
            Array.Copy(p.Coefficients, 0, result, shift, p.Degree + 1);
            return new Polynomial(result);
        }
    }
}

