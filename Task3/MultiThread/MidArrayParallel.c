#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    const int arrSize = 100000;

    int i = 0;
    // Init first array as in problem's condition
    double *startArr = (double *)calloc(arrSize, sizeof(double));
    #pragma omp parallel for private(i) shared(startArr) schedule(guided)
    for(i = 0; i < arrSize; ++i)
    {
        startArr[i] = i + 1;
    }

    int j = 0;
    // Init second array as in problem's condition
    double *finalArr = (double *)calloc(arrSize, sizeof(double));
    #pragma omp parallel for private(j) shared(startArr, finalArr) schedule(guided)
    for(j = 1; j < arrSize - 1; ++j)
    {
        finalArr[j] = startArr[j-1] * startArr[j] * startArr[j+1] / 3.0;
    }

    finalArr[0] = startArr[0];
    finalArr[arrSize - 1] = startArr[arrSize - 1];
    free(startArr);
    
    // Print second array into txt
    FILE * pFile;
    pFile = fopen ("../multi.txt", "w");
    for(int i = 0; i < arrSize; ++i)
    {
        fprintf (pFile, "%.0f ", finalArr[i]);
        if (i % 100 == 0)
        {
            fprintf (pFile, "\n");
        }
    }
    fclose (pFile);
    free(finalArr);

    return 0;
}