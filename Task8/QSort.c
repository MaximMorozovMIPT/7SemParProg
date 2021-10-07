#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

void quickSort(int* array, int n);
void swap(int* a, int* b);

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    int len = atoi(argv[1]);
    int* arr = (int*)calloc(len, sizeof(int*));

    int israndom = -142;

    if(strcmp(argv[2], "RANDOM") == 0)
    {
        srand(time(NULL));
        israndom=1;
    }
    else if(strcmp(argv[2], "ORDERED") == 0)
    {
        israndom=0;
    }
    else
    {
        srand(atoi(argv[2]));
        israndom=1;
    }

    if(israndom == 1)
    {
        for(int i = 0; i < len; ++i)
        {
            arr[i] = rand() % len;  
        }
    }
    else if(israndom == 0)
    {
        for(int i = 0; i < len; ++i)
        {
            arr[i] = i;  
        }
    }

    FILE * pFile;
    pFile = fopen ("sorted.txt", "w");
    for(int i = 0; i < len; ++i)
    {
        fprintf (pFile, "%d ", arr[i]);
    }
    fprintf(pFile, "\n");

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            quickSort(arr, len);
        }
    }
    
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Elapsed time  %.10fs\n", elapsed);

    for(int i = 0; i < len; ++i)
    {
        fprintf (pFile, "%d ", arr[i]);
    }
    fclose (pFile);

    return 0;
}

void quickSort(int* array, int n) 
{
    int i = 0, j = n - 1;
    int pivot = array[n / 2];
    do {
        while (array[i] < pivot) {
            i++;
        }
        while (array[j] > pivot) {
            j--;
        }
        if (i <= j) {
            swap(&array[i], &array[j]);
            i++; j--;
        }
    } while (i <= j);
    #pragma omp task shared(array)
    {
        if (j > 0) {
            quickSort(array, j+1);
        }
    }
    #pragma omp task shared(array)
    {
        if (n > i) {
            quickSort(array + i, n - i);
        }
    }
}

void swap(int* x, int* y)
{
    if (*x != *y)
    {
        *x ^= *y;
        *y ^= *x;
        *x ^= *y;
    }
}

 