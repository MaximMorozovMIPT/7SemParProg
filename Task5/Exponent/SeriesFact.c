#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double GetFact(int n );

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    double fact = 1;
    double exp = 0;
    double exactExp = 2.71828182845904523;

    // Get arg for number of iterations
    int numIter = atoi(argv[1]);

    int i = 1;
    // Use reduction to give every thread his own copy of exp
    // Use dynamic cause count factorial is O(n) algorithm
    #pragma omp parallel for private(i) reduction(+: exp) schedule(dynamic)
    for(i = 0; i < numIter; ++i)
    {
        fact = GetFact(i);
        exp+=fact;
    }
    printf("counted e = %.17f\n", exp);
    printf("exact e  =  %.17f\n", exactExp);
    return 0;
}

// Count here 1/(n!) cause it simplier
double GetFact(int n)
{
    if (n == 0)
    {
        return 1.;
    }
    double fact = 1;
    for(int i = 1; i <= n; ++i)
    {
        fact/=i;
    }
    return fact;
}