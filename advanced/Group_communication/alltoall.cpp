#include <iostream>
#include "mpi.h"

using namespace std;

int main()
{
    int size;
    int rank;
    int n = 2;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int send_array[n * size];
    int recv_array[n * size];
    for (int i = 0; i < n * size; i++)
    {
        send_array[i] = (rank + 1) * (i + 1);
    }
    MPI_Alltoall(send_array, n, MPI_INT, recv_array, n, MPI_INT, MPI_COMM_WORLD);
    for (int i = 0; i < size; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i)
        {
            for (int j = 0; j < n * size; j++)
            {
                printf("Process %d recv[%d] is %d\n", rank, j, recv_array[j]);
            }
        }
    }
    MPI_Finalize();
}