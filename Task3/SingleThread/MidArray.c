#include <stdio.h>
#include <stdlib.h>

int main()
{
    const int arrSize = 100000;

    // Init first array as in problem's condition
    double *startArr = (double *)calloc(arrSize, sizeof(double));
    for(int i = 0; i < arrSize; ++i)
    {
        startArr[i] = i + 1;
    }

    // Init second array as in problem's condition
    double *finalArr = (double *)calloc(arrSize, sizeof(double));
    for(int i = 1; i < arrSize - 1; ++i)
    {
        finalArr[i] = startArr[i-1] * startArr[i] * startArr[i+1] / 3.0;
    }
    finalArr[0] = startArr[0];
    finalArr[arrSize - 1] = startArr[arrSize - 1];
    free(startArr);

    // Print second array into txt
    FILE * pFile;
    pFile = fopen ("../single.txt", "w");
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