#include<iostream>
#include"mpi.h"

using namespace std;
 
int main()
{
    int size;
    int rank;
    int n = 10;
    int send_array[n];
    int * recv_array;
    int i;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int recv_count[size];
    int displs[size];
    for(i = 0; i < size; i++) {
        recv_count[i] = i + 1;
        displs[i] = 10 * i;
    }
    // 初始化其它进程的数据
    for(i = 0; i < n; i++) {
        send_array[i] = i + rank * n;
    }
    if(rank == 0) {
        recv_array = (int *)malloc(sizeof(int) * n * size);
    }
    MPI_Gatherv(send_array, recv_count[rank], MPI_INT, recv_array, recv_count, displs, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        for(i = 0; i < n * size; i++) {
            printf("recv_array[%d] id %d\n", i, recv_array[i]);
        }
        free(recv_array);
    }
    MPI_Finalize();
}