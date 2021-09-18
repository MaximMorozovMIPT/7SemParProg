#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    if(argc<=1) 
    {
        printf("You did not feed me arguments, I will die now :( ...\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    int sumN = 0;

    #pragma omp parallel for reduction(+: sumN) schedule(guided)
    for(int i = 0; i < N; ++i)
    {
        sumN+=N;
    }

    printf("Sum = %d\n", sumN);
}