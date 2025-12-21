namespace MPIparalele
{
    internal class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Usage: specify which program to run:");
                Console.WriteLine("  compare     - Run polynomial multiplication benchmark (Compare)");
                Console.WriteLine("  polynomial  - Run polynomial multiplication example (PolynomialMpi)");
                Console.WriteLine("  bigint      - Run big integer multiplication (BigIntMpi)");
                Console.WriteLine("");
                Console.WriteLine("Example: mpiexec -n 4 dotnet run -- polynomial");
                Console.WriteLine("         mpiexec -n 4 dotnet run -- compare 500 500");
                Console.WriteLine("         mpiexec -n 4 dotnet run -- bigint 123456789 987654321");
                return;
            }

            string program = args[0].ToLower();
            string[] programArgs = new string[args.Length - 1];
            Array.Copy(args, 1, programArgs, 0, args.Length - 1);

            switch (program)
            {
                case "compare":
                    Compare.Main(programArgs);
                    break;
                case "polynomial":
                    PolynomialMpi.Main(programArgs);
                    break;
                case "bigint":
                    BigIntMpi.Main(programArgs);
                    break;
                default:
                    Console.WriteLine($"Unknown program: {program}");
                    Console.WriteLine("Available programs: compare, polynomial, bigint");
                    break;
            }
        }
    }
}
