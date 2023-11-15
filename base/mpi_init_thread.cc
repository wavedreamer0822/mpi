#include"mpi.h"
#include"omp.h"
#include<iostream>

// MPI.THREAD_SINGLE，仅允许单线程；
// MPI.THREAD_FUNNELED，可以多线程，单只允许主线程执行 MPI 函数；
// MPI.THREAD_SERIALIZED，可以多线程，每个线程都可执行 MPI 函数，但某一时刻仅允许一个线程执行 MPI 函数；
// MPI.THREAD_MULTIPLE，多线程，线程可任意执行 MPI 函数



int main()
{
    int provided;
    int rank,size;
    MPI_Init_thread(NULL,NULL,MPI_THREAD_MULTIPLE,&provided);   
    //MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(provided < MPI_THREAD_MULTIPLE)
    {
        printf("The threading support level is lesser than that demanded.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    else
    {
        printf("The threading support level corresponds to that demanded.\n");
    }

    if(rank == 0)
    {
#pragma omp parallel for num_threads(4)
        for(int i = 0;i<4;i++)
        {
            int thread = omp_get_thread_num();
            MPI_Send(&thread,1,MPI_INT,1,i + 1,MPI_COMM_WORLD);  
        }   
    }
    else
    {
#pragma omp parallel for num_threads(4)
        for(int i = 0;i<4;i++)
        {
            MPI_Status stat;
            int recv;
            MPI_Recv(&recv,1,MPI_INT,0, i + 1, MPI_COMM_WORLD , &stat);
            std::cout<<recv<<std::endl;
        }

    }

    MPI_Finalize();

}