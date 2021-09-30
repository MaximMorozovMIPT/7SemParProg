#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    printf("Num of threads = %d\n", omp_get_max_threads());
    // Read launch argument
    int N = atoi(argv[1]);
    int sumN = 0;

    // Parallel loop to sum numbers to N
    // Using reduction to let every thread have his own sum and add them all at the end of the loop
    #pragma omp parallel for reduction(+: sumN) schedule(guided)
    for(int i = 1; i <= N; ++i)
    {
        sumN+=i;
    }

    printf("Sum = %d\n", sumN);
    return 0;
}