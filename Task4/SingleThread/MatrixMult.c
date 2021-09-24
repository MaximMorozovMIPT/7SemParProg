#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int randomSeed = atoi(argv[4]);
    srand(randomSeed);   // Initialization, should only be called once.

    int numRows1 = atoi(argv[1]);
    int numColumns1 = atoi(argv[2]);
    int numRows2 = atoi(argv[2]);
    int numColumns2 = atoi(argv[3]);

    int **matrix1 = (int **)calloc(numRows1, sizeof(int*));
    for(int i = 0; i < numRows1; ++i)
    {
        matrix1[i] = (int *)calloc(numColumns1, sizeof(int));
        for(int j = 0; j < numColumns1; ++j)
        {
            matrix1[i][j] = rand() % 43 - 21;
        }
    }

    int **matrix2 = (int **)calloc(numRows2, sizeof(int*));
    for(int i = 0; i < numRows2; ++i)
    {
        matrix2[i] = (int *)calloc(numColumns2, sizeof(int));
        for(int j = 0; j < numColumns2; ++j)
        {
            matrix2[i][j] = rand() % 43 - 21;
        }
    }

    int **matrix3 = (int **)calloc(numRows1, sizeof(int*));
    for(int i = 0; i < numRows1; ++i)
    {
        matrix3[i] = (int *)calloc(numColumns2, sizeof(int));
    }

    for(int i = 0; i < numRows1; ++i)
    {
        for(int j = 0; j < numColumns2; ++j)
        {
            int val = 0;
            for(int k = 0; k < numColumns1; ++k)
            {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    FILE * pFile;
    pFile = fopen ("../single.txt", "w");
    for(int i = 0; i < numRows1; ++i)
    {
        for(int j = 0; j < numColumns2; ++j)
        {
            fprintf (pFile, "%d ", matrix3[i][j]);
        }
        fprintf (pFile, "\n");
    }
    fclose (pFile);

    for(int i = 0; i < numRows1; ++i)
    {
        free(matrix1[i]);
    }
    free(matrix1);

    for(int i = 0; i < numRows2; ++i)
    {
        free(matrix2[i]);
    }
    free(matrix2);

    for(int i = 0; i < numRows1; ++i)
    {
        free(matrix3[i]);
    }
    free(matrix3);

    return 0;
}