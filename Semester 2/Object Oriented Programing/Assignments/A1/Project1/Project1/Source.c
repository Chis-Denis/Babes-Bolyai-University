//a. Generate all the prime numbers smaller than a given natural number n.
//b.Given a vector of numbers, find the longest increasing contiguous subsequence, such the sum of that any 2 consecutive elements is a prime number.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Check if a number is prime
int isPrime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Generate all prime numbers smaller than n
int* generatePrimes(int n, int* count) {
    int* primes = malloc(n * sizeof(int));
    *count = 0;
    for (int i = 2; i < n; i++) {
        if (isPrime(i)) {
            primes[(*count)++] = i;
        }
    }
    return primes;
}

// Find the longest increasing contiguous subsequence such that the sum of any two consecutive elements is prime
int* longestIncreasingContiguousSubsequence(int* v, int n, int* length) {
    //maximum length of any valid subsequence found so far
    int maxCount = 0;
    //array to store the longest subsequence
    int* maxSubsequence = malloc(n * sizeof(int));
    //Current count of elements in the current subsequence
    int count = 0;
    //array to temporarily hold the current subsequence
    int* subsequence = malloc(n * sizeof(int));

	// Check by 2 elements if the sum is prime
    for (int i = 0; i < n - 1; i++) {
        if (v[i] < v[i + 1] && isPrime(v[i] + v[i + 1])) {
            subsequence[count++] = v[i];
        }
		//if the next element is not prime, check if the current subsequence is the longest
        else {
            subsequence[count++] = v[i];
            if (count > maxCount) {
                maxCount = count;
                for (int j = 0; j < count; j++) {
                    maxSubsequence[j] = subsequence[j];
                }
            }
            count = 0;
            subsequence[0] = v[i];
            count = 1;
        }
    }
	//check if the last element is prime
    subsequence[count++] = v[n - 1];
	//check if the current subsequence is the longest
    if (count > maxCount) {
        maxCount = count;
        for (int j = 0; j < count; j++) {
            maxSubsequence[j] = subsequence[j];
        }
    }

    free(subsequence);
    *length = maxCount;
    return maxSubsequence;
}

//this is the main function that will test the above functions by generating primes smaller than n and finding the longest increasing contiguous subsequence
int main() {

    //i need an ui for this 2 options and exit

    while (1)
    {

		printf("1. Generate all the prime numbers smaller than a given natural number n.\n");
		printf("2. Find the longest increasing contiguous subsequence, such the sum of any 2 consecutive elements is a prime number.\n");
		printf("3. Exit\n");
		printf("Enter your choice: ");

		int choice;
		scanf("%d", &choice);

		if (choice == 1)
        {
            int n;
            printf("Enter the maximum number for prime generation: ");
            scanf("%d", &n);

            int countPrimes;
            int* primes = generatePrimes(n, &countPrimes);
            printf("Primes smaller than %d: ", n);
            for (int i = 0; i < countPrimes; i++) {
                printf("%d ", primes[i]);
            }
            printf("\n");
            free(primes);
        }

		else if (choice == 2)
		{
			printf("Enter the number of elements in the vector: ");
			int m;
			scanf("%d", &m);
			int* v = malloc(m * sizeof(int));
			printf("Enter the elements of the vector: ");
			for (int i = 0; i < m; i++) {
				scanf("%d", &v[i]);
			}
			int lengthSubseq;
			int* subsequence = longestIncreasingContiguousSubsequence(v, m, &lengthSubseq);
			printf("Longest increasing contiguous subsequence where consecutive sums are prime: ");
			for (int i = 0; i < lengthSubseq; i++) {
				printf("%d ", subsequence[i]);
			}
			printf("\n");
			free(v);
			free(subsequence);
		}
		else if (choice == 3)
		{
			return 0;
		}
    }
}