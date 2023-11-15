#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>

int main()
{
    MPI_Init(NULL,NULL);
    int rank ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int buffer = 0;
    MPI_Win window;
    MPI_Win_create(&buffer,sizeof(int), sizeof(int),MPI_INFO_NULL,MPI_COMM_WORLD,&window);
    if(rank == 0)
    {
        printf("MPI process value in my window before accumulatre:%d \n",buffer);
    }
    MPI_Win_fence(0,window);
    if(rank>0)
    {
        // IN origin_addr 本地缓冲区起始地址
        // IN origin_count 指定数据个数
        // IN origin_datatype 数据类型
        // IN target_rank 累计窗口所在的进程标识
        // IN target_disp 累计数据起始位置相对于窗口开始位置的偏移
        // IN target_count 窗口中累计数据个数
        // IN target_datatype 累计数据的数据类型
        // IN op 具体的累计操作
        // IN win 窗口对象
        // int MPI_Accumulate(void * origin_addr, int origin_count, MPI_Datatype origin_datatype,
        // int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype
        // target_datatype, MPI_Op op, MPI_Win win)
        MPI_Accumulate(&rank, 1, MPI_INT,0,0,1,MPI_INT,MPI_SUM,window);
    }
    MPI_Win_fence(0,window);
    if(rank == 0)
    {
        printf("after accumulate：%d \n", buffer);
    }
    if(rank == 1)
    {
        printf("after accumulate：%d \n", buffer);
    }
    MPI_Win_fence(0,window);
    MPI_Win_free(&window);
    MPI_Finalize();


}