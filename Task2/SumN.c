#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    printf("Num of threads = %d\n", omp_get_max_threads());
    int N = atoi(argv[1]);
    int sumN = 0;

    #pragma omp parallel for reduction(+: sumN) schedule(guided)
    for(int i = 0; i < N; ++i)
    {
        sumN+=i;
    }

    printf("Sum = %d\n", sumN);
}