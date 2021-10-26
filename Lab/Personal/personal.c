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

        // Count time of work algorithm
        struct timeval begin, lowmid, highmid, end;
        gettimeofday(&begin, 0);
        int shift = rows_per_thread * JSIZE;

        if(world_size == 1)
        {
            for (i=0; i<ISIZE; i++)
            {
                for (j=0; j<JSIZE; j++)
                {
                    a[i*JSIZE + j] = 10*i +j;
                }
            }
            for (i=8; i<ISIZE; i++)
            {
                for (j = 0; j < JSIZE-3; j++)
                {
                    a[i*JSIZE + j] = sin(0.00001*a[(i - 8)*JSIZE + j + 3]);
                }
            }
        }

        if(world_size > 1)
        {   
            for (i=0; i<rows_per_thread; i++)
            {
                for (j=0; j<JSIZE; j++)
                {
                    a[i*ISIZE + j] = 10*i +j;
                }
            }

            for (int i = 1; i < world_size - 1; ++i) 
            {
                MPI_Recv(&a[shift*i], shift, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            
            MPI_Recv(&a[shift*(world_size - 1)], rows_num_last * JSIZE, MPI_DOUBLE, world_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        

            int curr_send_row = 0; 
            int curr_recv_row = 8;
            int work_done = 0;
            for(;;)
            {
                for(int i = 1; i < world_size; ++i)
                {
                    if(curr_send_row == ISIZE - 8)
                    {
                        break;
                    }
                    MPI_Send(a + curr_send_row * JSIZE, JSIZE, MPI_DOUBLE, i, SEND_TAG, MPI_COMM_WORLD);
                    ++curr_send_row;
                }
                for(int i = 1; i < world_size; ++i)
                {
                    if(curr_recv_row == ISIZE)
                    {
                        work_done = 1;
                        break;
                    }
                    MPI_Recv(a + curr_recv_row * JSIZE, JSIZE, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    ++curr_recv_row;
                }
                if(work_done == 1)
                {
                    break;
                }
            }
        }

        // Count time of work
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("Elapsed time  %.10fs\n", elapsed);


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
    else
    {
        int rows_num = rows_per_thread;
        if(world_rank == world_size - 1)
        {
            rows_num = rows_num_last;
        }

        double* a = (double*)calloc(rows_num * JSIZE + 8 * JSIZE, sizeof(double));
        int shift = rows_per_thread * world_rank;

        for (i=0; i<rows_num; i++)
        {
            for (j=0; j<JSIZE; j++)
            {
                a[i*ISIZE + j] = 10*(i + shift) +j;
            }
        }
        
        // Send the result of counting to thread 0
        MPI_Send(a, rows_num * JSIZE, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        free(a);


        double* receive = (double*)calloc(JSIZE, sizeof(double));
        for(;;)
        {
            MPI_Recv(receive, JSIZE, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < JSIZE-3; j++)
            {
                receive[j] = sin(0.00001*receive[j + 3]);
            }
            MPI_Send(receive, JSIZE - 3, MPI_DOUBLE, 0, SEND_TAG, MPI_COMM_WORLD);
        }
    }
    return 0;
}
