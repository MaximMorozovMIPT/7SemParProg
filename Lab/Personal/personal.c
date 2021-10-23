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
    
    int rows_per_thread = JSIZE / world_size;
    int rows_num_last = JSIZE - rows_per_thread * (world_size - 1);
    
    int i, j;

    if(world_rank == 0)
    {
        printf("Number of threads is %d ", world_size);

        double* a = (double*)calloc(ISIZE * JSIZE, sizeof(double));

        int shift = rows_per_thread * ISIZE;

        // Count time of work algorithm
        struct timeval begin, lowmid, highmid, end;
        gettimeofday(&begin, 0);
        if(world_size == 1)
        {
            for (j=0; j<rows_per_thread; j++)
            {
                for (i=0; i<ISIZE; i++)
                {
                    a[j*ISIZE + i] = 10*i +j;
                }
            }

            for (i = 8; i < ISIZE; i++)
            {
                for (j=0; j<rows_per_thread - 3; j++)
                {
                    a[j * ISIZE + i] = sin(0.00001*a[(j + 3)* ISIZE + i - 8]);
                }
            }
        }
        else
        {
            for (j=0; j<rows_per_thread + 6; j++)
            {
                for (i=0; i<ISIZE; i++)
                {
                    a[j*ISIZE + i] = 10*i +j;
                }
            }

            for (i = 8; i < ISIZE; i++)
            {
                for (j=0; j<rows_per_thread+3; j++)
                {
                    a[j * ISIZE + i] = sin(0.00001*a[(j + 3)* ISIZE + i - 8]);
                }
            }
        }
        if(world_size > 1)
        {
            for (int i = 1; i < world_size - 1; ++i) 
            {
                MPI_Recv(&a[shift*i], shift, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            MPI_Recv(&a[shift*(world_size - 1)], rows_num_last * JSIZE, MPI_DOUBLE, world_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Count time of work
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("Elapsed time  %.10fs\n", elapsed);
    
        double* b = (double*)calloc(ISIZE * JSIZE, sizeof(double));

        for(i = 0; i < ISIZE; ++i)
        {
            for(j = 0; j < JSIZE; ++j)
            {
                b[i*JSIZE + j]=a[j * ISIZE + i];
            }
        }

        FILE *ff;
        ff = fopen("result.txt","w");
        for(i=0; i < ISIZE; i++)
        {
            for (j=0; j < JSIZE; j++)
            {
                fprintf(ff,"%f ",b[i*JSIZE + j]);
            }
            fprintf(ff,"\n");
        }
        fclose(ff);
        free(a);
        free(b);
    }
    else if(world_rank != world_size - 1)
    {
        double* a = (double*)calloc(rows_per_thread * ISIZE + 6 * ISIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;
        
        for (j=0; j<rows_per_thread + 6; j++)
        {
            for (i=0; i<ISIZE; i++)
            {
                a[j*ISIZE + i] = 10*i + j + rows_per_thread * world_rank;
            }
        }

        for (i = 8; i < ISIZE; i++)
        {
            for (j=0; j<rows_per_thread+3; j++)
            {
                a[j * ISIZE + i] = sin(0.00001*a[(j + 3)* ISIZE + i - 8]);
            }
        }

        MPI_Send(a, rows_per_thread * ISIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }
    else
    {
        // Same as 1.
        double* a = (double*)calloc(rows_num_last * ISIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        for (j=0; j<rows_num_last; j++)
        {
            for (i=0; i<ISIZE; i++)
            {
                a[j*ISIZE + i] = 10*i + j + rows_per_thread * world_rank;
            }
        }

        for (i = 8; i < ISIZE; i++)
        {
            for (j=0; j<rows_num_last-3; j++)
            {
                a[j * ISIZE + i] = sin(0.00001*a[(j + 3)* ISIZE + i - 8]);
            }
        }

        MPI_Send(a, rows_num_last * ISIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);
    }

    ierr = MPI_Finalize();
    return 0;
}
