/******************************************************************************
* ЗАДАНИЕ: bugged5.c
* ОПИСАНИЕ:
*   Используя секции, два потока инициализируют свои личные массивы
*   и затем добавляют свой массив в массивы соседа.
*   Почему-то происходит deadlock...
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N     1000000
#define PI    3.1415926535
#define DELTA .01415926535

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i;
    float a[N], b[N];
    omp_lock_t locka, lockb;

    omp_init_lock(&locka);
    omp_init_lock(&lockb);

    for (i = 0; i < N; i++) {
        a[i]=0;
        b[i]=0;
    }

    #pragma omp parallel shared(a, b, nthreads, locka, lockb) private(tid, i)
    {
        tid = omp_get_thread_num();
        #pragma omp master
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);
        #pragma omp barrier

        #pragma omp sections nowait
        {
            // Thread 1 blocks and changes a[]
            // Thread 2 blocks and changes b[]

            // When they finished 
            // 1 waits access to b[]
            // 2 waits access to a[]

            // But they unvlock them only after having access 
            // to blocked arrays, leads to deadlock

            #pragma omp section
            {
                omp_set_lock(&locka);
                printf("Thread %d updating a[]\n", tid);
                for (i = 0; i < N; i++)
                    a[i] += DELTA * i;
                // We finished and must give opportunity 
                // another threads to have access to a[]
                omp_unset_lock(&locka);
                omp_set_lock(&lockb);
                printf("Thread %d updating b[]\n", tid);
                for (i = 0; i < N; i++)
                    b[i] += DELTA + i;
                omp_unset_lock(&lockb);
                // Too late
                // omp_unset_lock(&locka);
            }

            #pragma omp section
            {
                omp_set_lock(&lockb);
                printf("Thread %d updating b[]\n", tid);
                for (i = 0; i < N; i++)
                    b[i] += PI * i;
                // We finished and must give opportunity 
                // another threads to have access to b[]
                omp_unset_lock(&lockb);
                omp_set_lock(&locka);
                printf("Thread %d adding b[] to a[]\n", tid);
                for (i = 0; i < N; i++)
                    a[i] += PI + i;
                omp_unset_lock(&locka);
                // Too late
                // omp_unset_lock(&lockb);
                
            }
        }
    }
    printf("Sample results: %f %f %f %f\n", a[0], b[0], a[999999], b[999999]);
}
