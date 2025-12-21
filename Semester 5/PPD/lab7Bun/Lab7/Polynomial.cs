using System;
using System.Linq;

namespace Lab7
{
    /// <summary>
    /// Represents a polynomial as an array of coefficients.
    /// Coefficient at index i represents the coefficient of x^i.
    /// </summary>
    public class Polynomial
    {
        private readonly double[] _coefficients;

        public int Degree => _coefficients.Length - 1;
        public double[] Coefficients => (double[])_coefficients.Clone();

        public Polynomial(double[] coefficients)
        {
            if (coefficients == null || coefficients.Length == 0)
                throw new ArgumentException("Polynomial must have at least one coefficient");
            
            // Remove leading zeros
            int lastNonZero = coefficients.Length - 1;
            while (lastNonZero > 0 && Math.Abs(coefficients[lastNonZero]) < 1e-10)
                lastNonZero--;

            _coefficients = new double[lastNonZero + 1];
            Array.Copy(coefficients, _coefficients, lastNonZero + 1);
        }

        public double this[int index]
        {
            get => index < _coefficients.Length ? _coefficients[index] : 0;
        }

        public override string ToString()
        {
            if (_coefficients.Length == 0) return "0";
            
            var terms = new System.Collections.Generic.List<string>();
            // Display in descending order: x^k + x^(k-1) + ...
            for (int i = _coefficients.Length - 1; i >= 0; i--)
            {
                if (Math.Abs(_coefficients[i]) < 1e-10) continue;
                
                string term = "";
                if (i == 0)
                {
                    term = _coefficients[i].ToString("F2");
                }
                else if (i == 1)
                {
                    term = _coefficients[i].ToString("F2") + "x";
                }
                else
                {
                    term = _coefficients[i].ToString("F2") + "x^" + i;
                }
                
                terms.Add(term);
            }
            
            return string.Join(" + ", terms);
        }

        /// <summary>
        /// Returns a string representation showing all coefficients, even for large polynomials
        /// </summary>
        public string ToStringFull()
        {
            if (_coefficients.Length == 0) return "0";
            
            var parts = new System.Collections.Generic.List<string>();
            
            // Show first few terms
            int showCount = Math.Min(10, _coefficients.Length);
            for (int i = 0; i < showCount; i++)
            {
                parts.Add($"{_coefficients[i]:F2}");
            }
            
            // If polynomial is large, show middle section
            if (_coefficients.Length > 20)
            {
                parts.Add("...");
                int midStart = _coefficients.Length / 2 - 5;
                int midEnd = midStart + 10;
                for (int i = midStart; i < midEnd && i < _coefficients.Length; i++)
                {
                    parts.Add($"{_coefficients[i]:F2}");
                }
                parts.Add("...");
            }
            
            // Show last few terms
            if (_coefficients.Length > 10)
            {
                int lastStart = Math.Max(showCount, _coefficients.Length - 10);
                for (int i = lastStart; i < _coefficients.Length; i++)
                {
                    parts.Add($"{_coefficients[i]:F2}");
                }
            }
            
            return $"[{string.Join(", ", parts)}] (degree {Degree}, {_coefficients.Length} coefficients)";
        }

        /// <summary>
        /// Creates a polynomial of given degree with all coefficients = 1
        /// </summary>
        public static Polynomial CreateOnes(int degree)
        {
            double[] coefficients = new double[degree + 1];
            for (int i = 0; i <= degree; i++)
            {
                coefficients[i] = 1.0;
            }
            return new Polynomial(coefficients);
        }
    }
}

