/**
 * Generates the first 2000 prime numbers
 * To compile: gcc -o firts200primes primos.c -lm
 */


#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// This function is to check
// if a given number is prime
bool isPrime(int n)
{
    // 0 and 1 are not prime numbers
    if (n == 1 || n == 0)
        return false;

    // Check for divisibility from 2 to sqrt(n)
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Driver code
int main()
{
    int N = 0, i = 0;;

    printf("{}");

    // Check every number from 1 to N
    while(N < 2000) {
        i++;
        if (isPrime(i)) {
            printf("%d, ", i);
            N++;
        }
    }

    printf("}\n");

    return 0;
}