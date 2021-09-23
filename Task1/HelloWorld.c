#include <omp.h>
#include <stdio.h>

int main() 
{
    int max_threads_id = omp_get_max_threads() - 1;
    
    // Every thread waiting when it will be called to print it's id and break the loop
    #pragma omp parallel shared(max_threads_id)
    while(1)
    {
        int tid = omp_get_thread_num();
        if(max_threads_id == omp_get_thread_num())
        {
            printf("Hello world from omp thread %d\n", tid);
            --max_threads_id;
            break;
        }
    }

    return 0;  
}