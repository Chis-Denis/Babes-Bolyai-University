import multiprocessing as mp
from concurrent.futures import ProcessPoolExecutor
from functools import lru_cache
import time
from typing import List, Tuple


def regular_multiply_sequential(poly1: List[int], poly2: List[int]) -> List[int]:
    # Basic nested loop approach - O(n^2)
    n, m = len(poly1), len(poly2)
    result = [0] * (n + m - 1)
    
    for i in range(n):
        for j in range(m):
            result[i + j] += poly1[i] * poly2[j]
    
    return result


def _multiply_chunk(args: Tuple[List[int], List[int], int, int, int]) -> List[int]:
    # Helper for parallel version - multiplies one chunk of poly1 with all of poly2
    poly1, poly2, start_idx, end_idx, result_size = args
    partial_result = [0] * result_size
    
    for i in range(start_idx, end_idx):
        for j in range(len(poly2)):
            partial_result[i + j] += poly1[i] * poly2[j]
    
    return partial_result


def regular_multiply_parallel(poly1: List[int], poly2: List[int], num_processes: int = None) -> List[int]:
    # Same as sequential but splits work across multiple processes
    n, m = len(poly1), len(poly2)
    result_size = n + m - 1
    
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    # Skip parallelization for small polynomials - overhead isn't worth it
    if n < num_processes * 2:
        return regular_multiply_sequential(poly1, poly2)
    
    # Split into chunks, one per process
    chunk_size = max(1, n // num_processes)
    chunks = []
    for i in range(0, n, chunk_size):
        end_idx = min(i + chunk_size, n)
        chunks.append((poly1, poly2, i, end_idx, result_size))
    
    # Run chunks in parallel
    with mp.Pool(processes=num_processes) as pool:
        partial_results = pool.map(_multiply_chunk, chunks)
    
    # Add up all the partial results
    result = [0] * result_size
    for partial in partial_results:
        for i in range(result_size):
            result[i] += partial[i]
    
    return result


def karatsuba_sequential(poly1: List[int], poly2: List[int]) -> List[int]:
    # Karatsuba divide-and-conquer
    
    # For small polynomials, just use regular multiplication
    if len(poly1) <= 16 or len(poly2) <= 16:
        return regular_multiply_sequential(poly1, poly2)
    
    # Pad to same length
    max_len = max(len(poly1), len(poly2))
    poly1 = poly1 + [0] * (max_len - len(poly1))
    poly2 = poly2 + [0] * (max_len - len(poly2))
    
    # Make length even for easier splitting
    if max_len % 2 == 1:
        poly1.append(0)
        poly2.append(0)
        max_len += 1
    
    mid = max_len // 2
    
    # Split in half
    low1, high1 = poly1[:mid], poly1[mid:]
    low2, high2 = poly2[:mid], poly2[mid:]
    
    # Three recursive calls (the magic of Karatsuba)
    z0 = karatsuba_sequential(low1, low2)  # low * low
    z2 = karatsuba_sequential(high1, high2)  # high * high
    
    # (low + high) * (low + high)
    sum1 = [low1[i] + high1[i] for i in range(mid)]
    sum2 = [low2[i] + high2[i] for i in range(mid)]
    z1 = karatsuba_sequential(sum1, sum2)
    
    # Extract the middle part: z1 = z1 - z0 - z2
    z1 = _subtract_poly(_subtract_poly(z1, z0), z2)
    
    # Combine: result = z0 + z1*x^mid + z2*x^(2*mid)
    result = [0] * (2 * max_len - 1)
    
    for i in range(len(z0)):
        result[i] += z0[i]
    for i in range(len(z1)):
        result[i + mid] += z1[i]
    for i in range(len(z2)):
        result[i + 2 * mid] += z2[i]
    
    # Clean up trailing zeros
    while len(result) > 1 and result[-1] == 0:
        result.pop()
    
    return result


def _subtract_poly(poly1: List[int], poly2: List[int]) -> List[int]:
    # Just subtracts two polynomials
    max_len = max(len(poly1), len(poly2))
    result = [0] * max_len
    
    for i in range(len(poly1)):
        result[i] += poly1[i]
    for i in range(len(poly2)):
        result[i] -= poly2[i]
    
    return result


def _karatsuba_parallel_internal(poly1: List[int], poly2: List[int], depth: int = 0) -> List[int]:
    
    # Base case: if polynomials are small, use regular multiplication
    if len(poly1) <= 16 or len(poly2) <= 16:
        return regular_multiply_sequential(poly1, poly2)
    
    # Make polynomials same length
    max_len = max(len(poly1), len(poly2))
    poly1 = poly1 + [0] * (max_len - len(poly1))
    poly2 = poly2 + [0] * (max_len - len(poly2))
    
    if max_len % 2 == 1:
        poly1.append(0)
        poly2.append(0)
        max_len += 1
    
    mid = max_len // 2
    
    # Split polynomials
    low1, high1 = poly1[:mid], poly1[mid:]
    low2, high2 = poly2[:mid], poly2[mid:]
    
    # Recursive calls (sequential to avoid nested pools)
    z0 = _karatsuba_parallel_internal(low1, low2, depth + 1)
    z2 = _karatsuba_parallel_internal(high1, high2, depth + 1)
    
    sum1 = [low1[i] + high1[i] for i in range(mid)]
    sum2 = [low2[i] + high2[i] for i in range(mid)]
    z1_sum = _karatsuba_parallel_internal(sum1, sum2, depth + 1)
    
    z1 = _subtract_poly(_subtract_poly(z1_sum, z0), z2)
    
    # Combine results
    result = [0] * (2 * max_len - 1)
    
    for i in range(len(z0)):
        result[i] += z0[i]
    
    for i in range(len(z1)):
        result[i + mid] += z1[i]
    
    for i in range(len(z2)):
        result[i + 2 * mid] += z2[i]
    
    # Remove trailing zeros
    while len(result) > 1 and result[-1] == 0:
        result.pop()
    
    return result


def karatsuba_parallel(poly1: List[int], poly2: List[int], num_processes: int = None) -> List[int]:

    # Base case: if polynomials are small, use regular multiplication
    if len(poly1) <= 16 or len(poly2) <= 16:
        return regular_multiply_sequential(poly1, poly2)
    
    # Make polynomials same length
    max_len = max(len(poly1), len(poly2))
    poly1 = poly1 + [0] * (max_len - len(poly1))
    poly2 = poly2 + [0] * (max_len - len(poly2))
    
    if max_len % 2 == 1:
        poly1.append(0)
        poly2.append(0)
        max_len += 1
    
    mid = max_len // 2
    
    # Split polynomials
    low1, high1 = poly1[:mid], poly1[mid:]
    low2, high2 = poly2[:mid], poly2[mid:]
    
    # Only worth it for bigger polynomials
    if max_len > 64 and (num_processes is None or num_processes > 1):
        if num_processes is None:
            num_processes = mp.cpu_count()
        
        # Run the three top-level multiplications in parallel
        with ProcessPoolExecutor(max_workers=min(3, num_processes)) as executor:
            futures = [
                executor.submit(_karatsuba_parallel_internal, low1, low2, 0),
                executor.submit(_karatsuba_parallel_internal, high1, high2, 0),
                executor.submit(_karatsuba_parallel_internal,
                               [low1[i] + high1[i] for i in range(mid)],
                               [low2[i] + high2[i] for i in range(mid)], 0)
            ]
            
            results = [f.result() for f in futures]
            z0, z2, z1_sum = results
    else:
        # Too small, just do it sequentially
        z0 = _karatsuba_parallel_internal(low1, low2, 0)
        z2 = _karatsuba_parallel_internal(high1, high2, 0)
        
        sum1 = [low1[i] + high1[i] for i in range(mid)]
        sum2 = [low2[i] + high2[i] for i in range(mid)]
        z1_sum = _karatsuba_parallel_internal(sum1, sum2, 0)
    
    # Compute z1 = z1_sum - z0 - z2
    z1 = _subtract_poly(_subtract_poly(z1_sum, z0), z2)
    
    # Combine results
    result = [0] * (2 * max_len - 1)
    
    for i in range(len(z0)):
        result[i] += z0[i]
    
    for i in range(len(z1)):
        result[i + mid] += z1[i]
    
    for i in range(len(z2)):
        result[i + 2 * mid] += z2[i]
    
    # Clean up trailing zeros
    while len(result) > 1 and result[-1] == 0:
        result.pop()
    
    return result


def verify_result(result1: List[int], result2: List[int]) -> bool:
    # Check if two results are the same (ignoring trailing zeros)
    def trim(poly):
        while len(poly) > 1 and poly[-1] == 0:
            poly = poly[:-1]
        return poly
    
    r1 = trim(result1.copy())
    r2 = trim(result2.copy())
    return r1 == r2


def benchmark(poly1: List[int], poly2: List[int], num_runs: int = 5):
    # Run all 4 variants and compare their performance
    print(f"\n{'='*70}")
    print(f"Benchmarking polynomial multiplication")
    print(f"Poly1 degree: {len(poly1)-1}, Poly2 degree: {len(poly2)-1}")
    print(f"{'='*70}\n")
    
    results = {}
    times = {}
    
    # Test regular sequential
    print("Testing Regular O(n^2) Sequential...")
    times_list = []
    for _ in range(num_runs):
        start = time.perf_counter()
        result = regular_multiply_sequential(poly1, poly2)
        end = time.perf_counter()
        times_list.append(end - start)
    results['regular_seq'] = result
    times['regular_seq'] = sum(times_list) / num_runs
    print(f"  Average time: {times['regular_seq']:.6f} seconds")
    
    # Test regular parallel
    print("Testing Regular O(n²) Parallel...")
    times_list = []
    for _ in range(num_runs):
        start = time.perf_counter()
        result = regular_multiply_parallel(poly1, poly2)
        end = time.perf_counter()
        times_list.append(end - start)
    results['regular_par'] = result
    times['regular_par'] = sum(times_list) / num_runs
    print(f"  Average time: {times['regular_par']:.6f} seconds")
    
    # Test Karatsuba sequential
    print("Testing Karatsuba Sequential...")
    times_list = []
    for _ in range(num_runs):
        start = time.perf_counter()
        result = karatsuba_sequential(poly1, poly2)
        end = time.perf_counter()
        times_list.append(end - start)
    results['karatsuba_seq'] = result
    times['karatsuba_seq'] = sum(times_list) / num_runs
    print(f"  Average time: {times['karatsuba_seq']:.6f} seconds")
    
    # Test Karatsuba parallel
    print("Testing Karatsuba Parallel...")
    times_list = []
    for _ in range(num_runs):
        start = time.perf_counter()
        result = karatsuba_parallel(poly1, poly2)
        end = time.perf_counter()
        times_list.append(end - start)
    results['karatsuba_par'] = result
    print(results)
    times['karatsuba_par'] = sum(times_list) / num_runs
    print(f"  Average time: {times['karatsuba_par']:.6f} seconds")
    
    # Verify all results are the same
    print("\nVerifying correctness...")
    base_result = results['regular_seq']
    all_match = True
    for name, result in results.items():
        if not verify_result(base_result, result):
            print(f"  ERROR: {name} result doesn't match!")
            all_match = False
    if all_match:
        print("  [OK] All results match!")
    
    # Print comparison
    print(f"\n{'='*70}")
    print("Performance Comparison:")
    print(f"{'='*70}")
    print(f"{'Algorithm':<30} {'Time (s)':<15} {'Speedup':<15}")
    print(f"{'-'*70}")
    
    baseline = times['regular_seq']
    for name, time_val in times.items():
        speedup = baseline / time_val
        print(f"{name:<30} {time_val:<15.6f} {speedup:<15.2f}x")
    
    print(f"\n{'='*70}\n")
    
    return times, results


if __name__ == "__main__":

    poly1 = [3, 2, 1]  # x^2 + 2x + 3
    poly2 = [1, 1]     # x + 1
    
    print("Small test case:")
    result = regular_multiply_sequential(poly1, poly2)
    print(f"Poly1: {poly1}")
    print(f"Poly2: {poly2}")
    print(f"Result: {result}")
    print(f"Expected: [3, 5, 3, 1] (3 + 5x + 3x^2 + x^3)")
    
    # Now test with bigger polynomials
    print("\n" + "="*70)
    print("Larger test cases for benchmarking:")
    print("="*70)
    
    import random
    random.seed(42)
    
    # Medium size test
    poly1_medium = [random.randint(-10, 10) for _ in range(100)]
    poly2_medium = [random.randint(-10, 10) for _ in range(100)]
    benchmark(poly1_medium, poly2_medium, num_runs=3)
    
    # Large size test
    poly1_large = [random.randint(-10, 10) for _ in range(500)]
    poly2_large = [random.randint(-10, 10) for _ in range(500)]
    benchmark(poly1_large, poly2_large, num_runs=2)
    
    # Very large test
    print("\n" + "="*70)
    print("Very large test case:")
    print("="*70)
    poly1_huge = [1 for _ in range(1000)]
    poly2_huge = [1 for _ in range(1000)]
    benchmark(poly1_huge, poly2_huge, num_runs=2)

