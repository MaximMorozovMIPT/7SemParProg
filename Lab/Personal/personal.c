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
    
    int rows_per_thread = ISIZE / world_size;
    int rows_num_last = ISIZE - rows_per_thread * (world_size - 1);
    
    int i, j;

    if(world_rank == 0)
    {
        printf("Number of threads is %d ", world_size);

        double* a = (double*)calloc(ISIZE * JSIZE, sizeof(double));

        int shift = rows_per_thread * JSIZE;
        if(world_size > 1)
        {   
            for (int i = 1; i < world_size - 1; ++i) 
            {
                MPI_Recv(&a[shift*i], shift, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            
            MPI_Recv(&a[shift*(world_size - 1)], rows_num_last * JSIZE, MPI_DOUBLE, world_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Count time of work algorithm
        struct timeval begin, lowmid, highmid, end;
        gettimeofday(&begin, 0);

        for (i=0; i<rows_per_thread; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*i +j;
            }
        }

        gettimeofday(&lowmid, 0);

        if(world_size > 1)
        {
            for (int i = 1; i < world_size - 1; ++i) {
                MPI_Send(&a[shift*i - 8 * JSIZE], shift + 8 * JSIZE, MPI_DOUBLE, i, SEND_TAG, MPI_COMM_WORLD);
            }

            int rows_num = ISIZE - rows_per_thread * (world_size - 1);
            MPI_Send(&a[shift*(world_size - 1) - 8 * JSIZE], rows_num_last * JSIZE + 8 * JSIZE, MPI_DOUBLE, world_size - 1, SEND_TAG, MPI_COMM_WORLD);
        }

        gettimeofday(&highmid, 0);

        for (i=8; i<rows_per_thread; i++)
        {
            for (j = 0; j < JSIZE-3; j++)
            {
                a[i * JSIZE + j] = sin(0.00001*a[(i-8) * JSIZE + j + 3]);
            }
        }

        // Count time of work
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("Elapsed time  %.10fs\n", elapsed);

        if(world_size > 1)
        {
            for (int i = 1; i < world_size - 1; ++i) 
            {
                MPI_Recv(&a[shift*i], shift, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            MPI_Recv(&a[shift*(world_size - 1)], rows_num_last * JSIZE, MPI_DOUBLE, world_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

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
        double* a = (double*)calloc(rows_per_thread * JSIZE + 8 * JSIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        for (i=0; i<rows_per_thread; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*(i + shift) +j;
            }
        }

        // Send the result of counting to thread 0
        MPI_Send(a, rows_per_thread * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);

        MPI_Recv(a, rows_per_thread * JSIZE + 8 * JSIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i=8; i<rows_per_thread + 8; i++)
        {
            for (j = 0; j < JSIZE-3; j++)
            {
                a[i * JSIZE + j] = sin(0.00001*a[(i-8) * JSIZE + j + 3]);
            }
        }

        MPI_Send(&a[8 * JSIZE], rows_per_thread * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }
    else
    {
        // Need to count last thread cause array size sometimes cannot be divided by number of threads
        int rows_num = ISIZE - rows_per_thread * (world_size - 1);

        // Same as 1.
        double* a = (double*)calloc(rows_num * JSIZE + 8 * JSIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        // Same as 2.
        for (i=0; i<rows_num; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*(i + shift) +j;
            }
        }
        
        // Send the result of counting to thread 0
        MPI_Send(a, rows_num_last * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);

        MPI_Recv(a, rows_num_last * JSIZE + 8 * JSIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i=8; i<rows_num_last + 8; i++)
        {
            for (j = 0; j < JSIZE-3; j++)
            {
                a[i * JSIZE + j] = sin(0.00001*a[(i-8) * JSIZE + j + 3]);
            }
        }
        MPI_Send(&a[8 * JSIZE], rows_num_last * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }

    ierr = MPI_Finalize();
    return 0;
}
