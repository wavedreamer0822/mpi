#include <stdio.h>
#include "mpi.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    int myrank, world_size;
    int my_split_size;
    MPI_Comm mycomm, intercomm;
    MPI_Status status;
    int split_flag;
    int reduce_result_value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // 实现奇偶进程的分别在各自通信域reduce。
    split_flag = (myrank % 2 == 0) ? 0 : 1;
    MPI_Comm_split(MPI_COMM_WORLD, split_flag, myrank, &mycomm);
    MPI_Reduce(&myrank, &reduce_result_value, 1, MPI_INT, MPI_SUM, 0, mycomm); // 两个通信域都reduce，“0”是指归约结果都放在各通信域的0号进程。
    if (split_flag == 0)
    {
        MPI_Reduce(&myrank, &reduce_result_value, 1, MPI_INT, MPI_MAX, 0, mycomm);
    } // 也可以用color参数来区分创建的子通信域!!!!!!
    if (myrank == 1)
        printf("MY_COMM_ODD reduce result is %d \n", reduce_result_value);
    if (myrank == 0)
        printf("MY_COMM_NO reduce result is %d \n", reduce_result_value);

    // 组间通信域
//     组间通信域中包括两个进程组，分别称作本地组与远程组。
// MPI_Intercomm_create(local_comm,local_leader,peer_comm,remote_leader,tag,newintercomm);
// 其中需要注意的是local_leader参数是调用该函数的进程在local_comm中的进程号，
//而remote_leader参数是远程组中某个进程在peer_comm中的进程号。另外，建立组间通信域时，本地组与远程组都需要调用MPI_Intercomm_create函数。
// 在上面的mpi_comm_split例子中，进一步地建立了奇偶进程通信域之间的组间通信域。
//并且进行了两个通信域的leader进程之间的通信。注意此时的source与dest应该是进程在自身的进程组中的编号。即source是发送进程在本地组中编号，dest是接收进程在远程组中的编号。
    if (split_flag == 0)
    {
        MPI_Intercomm_create(mycomm, 0, MPI_COMM_WORLD, 1, 1, &intercomm);  //一般第二个都为MPI_COMM_WORLD tag相同则为相同组间通信域
    }
    if (split_flag == 1)
    {
        MPI_Intercomm_create(mycomm, 0, MPI_COMM_WORLD, 0, 1, &intercomm);
    }
    if (myrank == 0)
    {
        MPI_Send(&reduce_result_value, 1, MPI_INT, 0, 99, intercomm);
    }
    if (myrank == 1)
    {
        MPI_Recv(&reduce_result_value, 1, MPI_INT, 0, 99, intercomm, &status);
    }
    if (myrank == 1)
        printf("after intercommunication reduce result is %d \n", reduce_result_value);
    MPI_Finalize();
    return 0;
}