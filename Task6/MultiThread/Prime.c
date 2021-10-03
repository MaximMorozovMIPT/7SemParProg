#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int IsPrime(int value);

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    int numIter = atoi(argv[1]);
    int primeNum = 0;
    int i = 2;

    // Check for every value is it prime
    // Use reduction to let every thread have his own primeNum
    // Use dynamic cause IsPrime is O(n) algorithm
    #pragma omp parallel for private(i) reduction(+: primeNum) schedule(dynamic)
    for(i = 2; i <= numIter; ++i)
    {
        if(IsPrime(i))
        {
            ++primeNum;
        }
    }
    printf("Num of prime numbers to %d is %d\n",numIter, primeNum);
    return 0;
}


int IsPrime(int value)
{
    // Iteration over all divisors up to sqrt of value
    for(int i = 2; i < sqrt(value) + 1; ++i)
    {
        if (value % i == 0 && value != i)
        {
            return 0;
        }
    }
    return 1;
}