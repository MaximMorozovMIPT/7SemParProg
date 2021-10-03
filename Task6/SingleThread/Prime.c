#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int IsPrime(int value);

int main(int argc, char* argv[])
{
    int numIter = atoi(argv[1]);
    int primeNum = 0;
    int i = 2;

    // Check for every value is it prime
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
    for(int i = 2; i*i <= value; ++i)
    {
        if (value % i == 0 && value != i)
        {
            return 0;
        }
    }
    return 1;
}