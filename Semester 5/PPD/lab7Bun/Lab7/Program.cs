using System;
using System.Diagnostics;
using System.Linq;
using MPI;

namespace Lab7
{
    class Program
    {
        static void Main(string[] args)
        {
            using (var env = new MPI.Environment(ref args))
            {
                Intracommunicator comm = Communicator.world;

                if (comm.Rank == 0)
                {
                    Console.WriteLine("=== Lab 7: Distributed Polynomial Multiplication using MPI ===\n");
                    Console.WriteLine($"Running with {comm.Size} processes\n");
                }

                // Test Case: Polynomial of rank 1000 with all coefficients = 1
                TestCase1(comm);

                // Additional test cases
                TestCase2(comm);
                TestCase3(comm);
            }
        }

        static void TestCase1(Intracommunicator comm)
        {
            if (comm.Rank == 0)
            {
                Console.WriteLine(new string('=', 60));
                Console.WriteLine("Test Case 1: Polynomials of rank 1000 (all coefficients = 1)");
                Console.WriteLine(new string('=', 60) + "\n");
            }

            int degree = 1000;
            Polynomial poly1 = Polynomial.CreateOnes(degree);
            Polynomial poly2 = Polynomial.CreateOnes(degree);

            if (comm.Rank == 0)
            {
                Console.WriteLine($"Polynomial 1: {poly1.ToStringFull()}");
                Console.WriteLine($"Polynomial 2: {poly2.ToStringFull()}\n");
            }

            // Regular O(n²) algorithm
            TestRegularAlgorithm(comm, poly1, poly2, "Regular O(n²) Algorithm");

            // Karatsuba algorithm
            TestKaratsubaAlgorithm(comm, poly1, poly2, "Karatsuba Algorithm");
        }

        static void TestCase2(Intracommunicator comm)
        {
            if (comm.Rank == 0)
            {
                Console.WriteLine(new string('=', 60));
                Console.WriteLine("Test Case 2: Medium-sized polynomials (degree 100)");
                Console.WriteLine(new string('=', 60) + "\n");
            }

            int degree = 100;
            Polynomial poly1 = Polynomial.CreateOnes(degree);
            Polynomial poly2 = Polynomial.CreateOnes(degree);

            if (comm.Rank == 0)
            {
                Console.WriteLine($"Polynomial 1: {poly1.ToStringFull()}");
                Console.WriteLine($"Polynomial 2: {poly2.ToStringFull()}\n");
            }

            TestRegularAlgorithm(comm, poly1, poly2, "Regular O(n²) Algorithm");
            TestKaratsubaAlgorithm(comm, poly1, poly2, "Karatsuba Algorithm");
        }

        static void TestCase3(Intracommunicator comm)
        {
            if (comm.Rank == 0)
            {
                Console.WriteLine(new string('=', 60));
                Console.WriteLine("Test Case 3: Small polynomials (degree 20)");
                Console.WriteLine(new string('=', 60) + "\n");
            }

            int degree = 20;
            Polynomial poly1 = Polynomial.CreateOnes(degree);
            Polynomial poly2 = Polynomial.CreateOnes(degree);

            if (comm.Rank == 0)
            {
                Console.WriteLine($"Polynomial 1: {poly1.ToStringFull()}");
                Console.WriteLine($"Polynomial 2: {poly2.ToStringFull()}\n");
            }

            TestRegularAlgorithm(comm, poly1, poly2, "Regular O(n²) Algorithm");
            TestKaratsubaAlgorithm(comm, poly1, poly2, "Karatsuba Algorithm");
        }

        static void TestRegularAlgorithm(Intracommunicator comm, Polynomial a, Polynomial b, string algorithmName)
        {
            if (comm.Rank == 0)
            {
                Console.WriteLine($"--- {algorithmName} ---");
            }

            comm.Barrier();
            var sw = Stopwatch.StartNew();

            Polynomial result = RegularMPI.Multiply(comm, a, b);

            comm.Barrier();
            sw.Stop();

            if (comm.Rank == 0)
            {
                Console.WriteLine($"Time: {sw.ElapsedMilliseconds} ms");
                Console.WriteLine($"Result: {result.ToStringFull()}");
                Console.WriteLine();
            }
        }

        static void TestKaratsubaAlgorithm(Intracommunicator comm, Polynomial a, Polynomial b, string algorithmName)
        {
            if (comm.Rank == 0)
            {
                Console.WriteLine($"--- {algorithmName} ---");
            }

            comm.Barrier();
            var sw = Stopwatch.StartNew();

            Polynomial result = KaratsubaMPI.Multiply(comm, a, b);

            comm.Barrier();
            sw.Stop();

            if (comm.Rank == 0)
            {
                Console.WriteLine($"Time: {sw.ElapsedMilliseconds} ms");
                Console.WriteLine($"Result: {result.ToStringFull()}");
                Console.WriteLine();
            }
        }
    }
}

