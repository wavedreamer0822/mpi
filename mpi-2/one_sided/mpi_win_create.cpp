#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//  IN base 窗口空间的初始地址
// IN size 以字节为单位的窗口空间大小
// IN disp_unit 一个偏移单位对应的字节数
// IN info 传递给运行时的信息
// IN comm 通信域
// OUT win 返回的窗口对象
// int MPI_Win_create(void * base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm,
// MPI_Win * win)

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    // Check that only 2 MPI processes are spawn
    int comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if(comm_size != 2)
    {
        printf("This application is meant to be run with 2 MPI processes, not %d.\n", comm_size);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Get my rank
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Create the window
    const int ARRAY_SIZE = 2;
    int window_buffer[ARRAY_SIZE];
    MPI_Win window;
    if(my_rank == 1)
    {
        window_buffer[1] = 67890;
    }
    MPI_Win_create(window_buffer, ARRAY_SIZE * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &window);
    MPI_Win_fence(0, window);

    int remote_value;
    if(my_rank == 0)
    {
        // Fetch the second integer in MPI process 1 window
        // OUT origin_addr 本地接收缓冲区的起始地址
        // IN origin_count 以指定的数据类型为单位 接收数据的个数
        // IN origin_datatype 接收数据的数据类型
        // IN target_rank 将要读的窗口所在的进程标识
        // IN target_disp 读取位置相对于窗口起始地址偏移单位的个数
        // IN target_count 以指定的数据类型为单位 读取数据的个数
        // IN target_datatype 读取数据的数据类型
        // IN win 窗口对象
        // int MPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int
        // target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype
        // target_datatype, MPI_Win win)

        MPI_Get(&remote_value, 1, MPI_INT, 1, 1, 1, MPI_INT, window);

        // Push my value into the first integer in MPI process 1 window
        int my_value = 12345;
        
        // IN origin_addr 本地发送缓冲区起始地址
        // IN origin_count 本地发送缓冲区中将要写到窗口内的数据个数
        // IN origin_datatype 本地发送缓冲区中的数据类型
        // IN target_rank 目标进程标识
        // IN target_disp 相对于写窗口起始地址的偏移单位 从该位置开始写
        // IN target_count 以指定的数据类型为单位 写入窗口的数据的个数
        // IN target_datatype 写数据的数据类型
        // IN win 窗口对象
        // int MPI_Put(void * origin_addr, int origin_count, MPI_Datatype origin_datatype,
        // int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype
        // target_datatype, MPI_Win win)

        MPI_Put(&my_value, 1, MPI_INT, 1, 0, 1, MPI_INT, window);
    }

    // Wait for the MPI_Get and MPI_Put issued to complete before going any further
    MPI_Win_fence(0, window); //类似于 MPI_Barrier(MPI_COMM_WORLD) 对窗口内容的操作都在这一对语句中

    if(my_rank == 0)
    {
        printf("[MPI process 0] Value fetched from MPI process 1 window_buffer[1]: %d.\n", remote_value);
    }
    else
    {
        printf("[MPI process 1] Value put in my window_buffer[0] by MPI process 0: %d.\n", window_buffer[0]);
    }

    // Destroy the window
    MPI_Win_free(&window);  

    MPI_Finalize();

    return EXIT_SUCCESS;
}