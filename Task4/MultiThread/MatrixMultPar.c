#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    printf("Num of threads = %d\n", omp_get_max_threads());

    // Seed to make random values equal in this and single thread program
    int randomSeed = atoi(argv[4]);
    srand(randomSeed);

    // Init sizes of matrices
    int numRows1 = atoi(argv[1]);
    int numColumns1 = atoi(argv[2]);
    int numRows2 = atoi(argv[2]);
    int numColumns2 = atoi(argv[3]);

    // Dont use parallel init for matrices cause they will be different with single thread init

    // Init first matrix with random values
    int **matrix1 = (int **)calloc(numRows1, sizeof(int*));
    int i, j;
    for(i = 0; i < numRows1; ++i)
    {
        matrix1[i] = (int *)calloc(numColumns1, sizeof(int));
        for(j = 0; j < numColumns1; ++j)
        {
            matrix1[i][j] = rand() % 43 - 21;
        }
    }

    //Init second matrix with random values
    int **matrix2 = (int **)calloc(numRows2, sizeof(int*));
    for(i = 0; i < numRows2; ++i)
    {
        matrix2[i] = (int *)calloc(numColumns2, sizeof(int));
        for(j = 0; j < numColumns2; ++j)
        {
            matrix2[i][j] = rand() % 43 - 21;
        }
    }

    // Alloc memory for third matrix
    // Do it in parrallel way cause matrices can be big
    // Use dynamic cause any thread can be stressed be other programs
    int **matrix3 = (int **)calloc(numRows1, sizeof(int*));
    #pragma omp parallel for private(i) shared(matrix3) schedule(dynamic)
    for(i = 0; i < numRows1; ++i)
    {
        matrix3[i] = (int *)calloc(numColumns2, sizeof(int));
    }

    int k;
    // Multiply matrices one and two
    // Use dynamic cause any thread can be stressed be other programs
    // Use collapse to parallel calculations for every element in third matrix
    #pragma omp parallel for \
    private(i, j, k) \
    shared(matrix1, matrix2, matrix3) \
    schedule(dynamic) \
    collapse(2)
    for(i = 0; i < numRows1; ++i)
    {
        for(j = 0; j < numColumns2; ++j)
        {
            for(k = 0; k < numColumns1; ++k)
            {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    // Write third matrix into txt to compare results with singlethread program
    FILE * pFile;
    pFile = fopen ("../multi.txt", "w");
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