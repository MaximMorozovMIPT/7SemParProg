/******************************************************************************
* ЗАДАНИЕ: bugged2.c
* ОПИСАНИЕ:
*   Еще одна программа на OpenMP с багом. 
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    double total;
    // --------------------------------------------------------------------
    // #pragma omp parallel 
    // {
    //     tid = omp_get_thread_num();
    //     if (tid == 0)
    //     {
    //         nthreads = omp_get_num_threads();
    //         printf("Number of threads = %d\n", nthreads);
    //     }
    //     printf("Thread %d is starting...\n", tid);

    //     #pragma omp barrier

    //     total = 0.0;
    //     #pragma omp for schedule(dynamic, 10)
    //     for (i = 0; i < 1000000; i++) 
    //         total = total + i*1.0;

    //     printf ("Thread %d is done! Total= %e\n", tid, total);
    // }
    // --------------------------------------------------------------------

    // Problem in shared tid that overwrites
    // Problem in race condition in shared variable total
    // Problem in random values of total also in shared i
    // Solution private for tid, private for i, reduction for total
    // Also float is too small, value of it depends on summation order so use double
    // Also barrier should be after total=0 or we can move assignment to single thread section
    // cause sometimes thread can make this assignment in the middle of the loop, or at the end
    // And now we also should print total after parallel section cause every thread has his own copy
    #pragma omp parallel private(tid, i) reduction(+: total) 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d is starting...\n", tid);

        total = 0.0;
        #pragma omp barrier

        #pragma omp for schedule(dynamic, 10) 
        for (i = 0; i < 1000000; i++) 
        {
            total = total + i*1.0;
        }
        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
    printf ("Total= %e\n", total);
}
