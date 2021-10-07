/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

float dotprod(float sum)
{
    int i, tid;
    // float sum; this is a creation of private sum

    // Move parallel here cause otherwise sum will be here private
    // but it was declared shared, so we can take from main copy of 
    // sum and return counter value here back to main

    #pragma omp parallel private(i, tid) reduction(+:sum)
    {
        tid = omp_get_thread_num();
        #pragma omp for
        for (i = 0; i < VECLEN; i++)
        {
            sum = sum + (a[i] * b[i]);
            printf("  tid= %d i=%d\n", tid, i);
        }
    }
    return sum;
}


int main (int argc, char *argv[])
{
    int i;
    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;

    // #pragma omp parallel shared(sum)
    sum = dotprod(sum);

    printf("Sum = %f\n",sum);

    // Added it just to check that program works correctly
    float check = 99 * 100 * 199 / 6;
    printf("Check sum = %f\n",check);
}


