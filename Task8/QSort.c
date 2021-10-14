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

    // Just init it
    int israndom = -142;

    // Choose the mode described in README.md
    if(strcmp(argv[2], "RANDOM") == 0)
    {
        srand(time(NULL));
        israndom=1;
    }
    else if(strcmp(argv[2], "ORDERED") == 0)
    {
        israndom=0;
    }
    else if(strcmp(argv[2], "LINE") == 0)
    {
        israndom=2;
    }
    else if(strcmp(argv[2], "FILE") == 0)
    {
        israndom=3;
    }
    else
    {
        srand(atoi(argv[2]));
        israndom=1;
    }

    int len = atoi(argv[1]);
    int* arr = (int*)calloc(len, sizeof(int));

    // Fill the array according to mode in README.md
    if (israndom == 0)
    {
        for(int i = 0; i < len; ++i)
        {
            arr[i] = i;  
        }
    }
    else if(israndom == 1)
    {
        for(int i = 0; i < len; ++i)
        {
            arr[i] = rand() % len;  
        }
    }
    else if(israndom == 2)
    {
        for(int i = 0; i < len; ++i)
        {
            scanf("%d", &arr[i]);
        }
    }
    else if(israndom == 3)
    {
        FILE * iFile;
        iFile = fopen("input.txt", "r");
        for(int i = 0; i < len; ++i)
        {
            fscanf(iFile, "%d", &arr[i]);
        }
        fclose (iFile);
    }

    // Use it to count time exactly
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    // First call should be done in one thread
    // We dont want to sort whole array several times
    // Want to sort parts in parallel way
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

    // Print sorted array
    FILE * pFile;
    pFile = fopen ("sorted.txt", "w");
    for(int i = 0; i < len; ++i)
    {
        fprintf (pFile, "%d ", arr[i]);
    }
    fclose (pFile);

    return 0;
}

// The exact algo of qsort without randomization
void quickSort(int* array, int n) 
{
    int i = 0, j = n - 1;
    // Wee need to chose it
    // This is fast way and still working
    int pivot = array[n / 2];
    // Sort iteration
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

    // Start recursion, tasks are the easiest way of paralleling it
    // We put a begin pointer in sorting part and the number of elements to sort
    // Shared array not to alloc new memory
    #pragma omp task shared(array)
    {
        // Sort first half
        if (j > 0) {
            quickSort(array, j+1);
        }
    }
    #pragma omp task shared(array)
    {
        // Sort second part
        if (n > i) {
            quickSort(array + i, n - i);
        }
    }
}

// Fast xor way to swap
void swap(int* x, int* y)
{
    if (*x != *y)
    {
        *x ^= *y;
        *y ^= *x;
        *x ^= *y;
    }
}

 