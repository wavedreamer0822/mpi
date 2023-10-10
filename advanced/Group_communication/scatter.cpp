#include <iostream>
#include "mpi.h"

using namespace std;

int main()
{
    int size, rank;
    int n = 10;
    int *send_array;
    int recv_array[n];

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0)
    {
        send_array = (int *)malloc(sizeof(int) * n * size);
        for (int i = 0; i < n * size; i++)
        {
            send_array[i] = i;
        }
    }
    MPI_Scatter(&send_array[0], n, MPI_INT, &recv_array[0], n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < size; i++)
    {
        if (rank == i)
        {
            for (int j = 0; j < n; j++)
            {
                printf("Process %d recv[%d] is %d\n", rank, j, recv_array[j]);
            }
        }
    }
}