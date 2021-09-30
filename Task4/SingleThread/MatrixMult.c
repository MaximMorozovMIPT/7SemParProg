#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    // Seed to make random values equal in this and multi thread program
    int randomSeed = atoi(argv[4]);
    srand(randomSeed);

    // Init sizes of matrices
    int numRows1 = atoi(argv[1]);
    int numColumns1 = atoi(argv[2]);
    int numRows2 = atoi(argv[2]);
    int numColumns2 = atoi(argv[3]);

    //Init first matrix with random values
    int **matrix1 = (int **)calloc(numRows1, sizeof(int*));
    for(int i = 0; i < numRows1; ++i)
    {
        matrix1[i] = (int *)calloc(numColumns1, sizeof(int));
        for(int j = 0; j < numColumns1; ++j)
        {
            matrix1[i][j] = rand() % 43 - 21;
        }
    }

    //Init second matrix with random values
    int **matrix2 = (int **)calloc(numRows2, sizeof(int*));
    for(int i = 0; i < numRows2; ++i)
    {
        matrix2[i] = (int *)calloc(numColumns2, sizeof(int));
        for(int j = 0; j < numColumns2; ++j)
        {
            matrix2[i][j] = rand() % 43 - 21;
        }
    }

    // Alloc memory for third matrix
    int **matrix3 = (int **)calloc(numRows1, sizeof(int*));
    for(int i = 0; i < numRows1; ++i)
    {
        matrix3[i] = (int *)calloc(numColumns2, sizeof(int));
    }

    // Multiply matrices one and two
    for(int i = 0; i < numRows1; ++i)
    {
        for(int j = 0; j < numColumns2; ++j)
        {
            for(int k = 0; k < numColumns1; ++k)
            {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    // Write third matrix into txt to compare results with multithread program
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

    // Free all memory
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