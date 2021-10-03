#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int IsPrime(int value);

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    int numIter = atoi(argv[1]);
    // Create sieve of Eratosthenes
    char * eratosthenes = (char *)calloc(numIter + 1, sizeof(char));

    int i = 2;
    int maxIter = (int)sqrt(numIter) + 1;


    // Fill sieve of Eratosthenes
    // Use dynamic cause small values will longer fill the sieve than bigger
    #pragma omp parallel for private(i) shared(eratosthenes) schedule(dynamic)
    for(i = 2; i < maxIter; ++i)
    {
        if(eratosthenes[i] == 255)
        {
            // sieve of Eratosthenes is faster than simple methods cause of this section
            // Just skip counting primary for a lot of numbers
            // While brute force will check this numbers too
            continue;
        }
        for(int j = i * 2; j <= numIter; j += i)
        {
            // For instance
            // Some numbers such as 2 and 4 can fill at the same time number 8
            // To let it work correct use critical
            #pragma omp critical
            {
                eratosthenes[j] = 255;
            }
        }
    }

    int primeNum = 0;
    // Count number prime numbers
    // Use static cause we only sum prime numbers, do not use dynamic not to spend more time
    // Use reduction to let every thread have his own primeNum
    #pragma omp parallel for private(i) shared(eratosthenes) reduction(+: primeNum) schedule(static)
    for(i = 2; i <= numIter; ++i)
    {
        if(eratosthenes[i] == 0)
        {
            ++primeNum;
        }
    }

    printf("Num of prime numbers to %d is %d\n",numIter, primeNum);
    return 0;
}