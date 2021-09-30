#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double GetDoubleFact(int n );

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    double addition = 0;
    double asin = 0;
    double exactAsin = 1.57079632679;

    // Get arg for number of iterations
    int numIter = atoi(argv[1]);

    int i = 1;
    // Use reduction to give every thread his own copy of exp
    // Use dynamic cause count factorial is O(n) algorithm
    #pragma omp parallel for private(i) reduction(+: asin) schedule(dynamic)
    for(i = 0; i < numIter; ++i)
    {
        addition = GetDoubleFact(i * 2 - 1) / GetDoubleFact(i * 2) / (2 * i + 1);
        asin+=addition;
    }
    printf("counted asin = %.17f\n", asin);
    printf("exact asin  =  %.17f\n", exactAsin);
    return 0;
}

double GetDoubleFact(int n)
{
    if (n <= 0)
    {
        return 1.;
    }

    double fact = 1;
    int i = (n % 2 == 1) ? 1 : 2;
    for(i; i <= n; i+=2)
    {
        fact*=i;
    }
    return fact;
}