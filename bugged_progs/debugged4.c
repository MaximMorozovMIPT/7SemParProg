/******************************************************************************
* ЗАДАНИЕ: bugged4.c
* ОПИСАНИЕ:
*   Очень простая программа с segmentation fault.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;

    // Have pity on the stack, it can contain about 1-8MB data
    // double a[N][N];

    double ** a;
    #pragma omp parallel shared(nthreads) private(i, j, tid, a)
    {
        // Can't find info about pointers and private but I think
        // if we create array outside of the parallel section
        // and use private(a), every thread will have own a
        // which is double ** - pointer, poiner without allocation =
        // seg fault, so alloc here
        a = (double **)calloc(N, sizeof(double*));
        for(int k = 0; k < N; ++k)
        {
            a[k]=(double*)calloc(N, sizeof(double));
        }

        tid = omp_get_thread_num();
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        printf("Thread %d starting...\n", tid);
        #pragma omp barrier

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                a[i][j] = tid + i + j;
            }
        }

        // Added barrier to show that every thread has his own copy of the array
        #pragma omp barrier
        printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);
    } 
}
