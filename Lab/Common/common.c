#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <sys/time.h>

#define ISIZE 1000
#define JSIZE 1000
#define SEND_TAG 2001

int main(int argc, char **argv)
{
    int world_rank, world_size;
    // Itit parallel part
    int ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // Need it to equal threads' load 
    int rows_per_thread = ISIZE / world_size;
    
    int i, j;
    
    if(world_rank == 0)
    {
        printf("Number of threads is %d ", world_size);

        // 1. Alloc memory for this thread
        double* a = (double*)calloc(ISIZE * JSIZE, sizeof(double));

        // Count time of work algorithm
        struct timeval begin, end;
        gettimeofday(&begin, 0);

        // 2. The same as in the original but changed numeration from 2d to 1d array
        for (i=0; i<rows_per_thread; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*i +j;
            }
        }
        for (i=0; i<rows_per_thread; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                a[i*ISIZE + j] = sin(0.00001*a[i*ISIZE + j]);
            }
        }

        // Receive data from other threads
        int shift = rows_per_thread * JSIZE;
        for (int i = 1; i < world_size - 1; ++i) {
            MPI_Recv(&a[shift*i], shift, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if(world_size > 1)
        {
            int rows_num = ISIZE - rows_per_thread * (world_size - 1);
            MPI_Recv(&a[shift*(world_size - 1)], rows_num * JSIZE, MPI_DOUBLE, world_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Count time of work
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("Elapsed time  %.10fs\n", elapsed);

        // Write results
        FILE *ff;
        ff = fopen("result.txt","w");
        for(i=0; i < ISIZE; i++)
        {
            for (j=0; j < JSIZE; j++)
            {
                fprintf(ff,"%f ",a[i*ISIZE + j]);
            }
        fprintf(ff,"\n");
        }
        fclose(ff);
        free(a);
    }
    else if(world_rank != world_size - 1)
    {
        // Same as 1.
        double* a = (double*)calloc(rows_per_thread * JSIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        // Same as 2.
        for (i=0; i<rows_per_thread; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*(i + shift) +j;
            }
        }
        for (i=0; i<rows_per_thread; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                a[i*ISIZE + j] = sin(0.00001*a[i*ISIZE + j]);
            }
        }

        // Send the result of counting to thread 0
        MPI_Send(a, rows_per_thread * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }
    else
    {
        // Need to count last thread cause array size sometimes cannot be divided by number of threads
        int rows_num = ISIZE - rows_per_thread * (world_size - 1);

        // Same as 1.
        double* a = (double*)calloc(rows_num * JSIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        // Same as 2.
        for (i=0; i<rows_num; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*(i + shift) +j;
            }
        }
        for (i=0; i<rows_num; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                a[i*ISIZE + j] = sin(0.00001*a[i*ISIZE + j]);
            }
        }
        
        // Send the result of counting to thread 0
        MPI_Send(a, rows_num * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }
    ierr = MPI_Finalize();
    return 0;
}
