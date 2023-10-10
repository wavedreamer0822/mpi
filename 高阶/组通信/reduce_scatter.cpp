#include <iostream>
#include "mpi.h"

using namespace std;

int main()
{
    int size, rank;
    int n = 2;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int send_array[n];
    int recv_array[n];
    int num[size];

    for (int i = 0; i < size; i++)
    {
        num[i] = n;
    }
    for(int i = 0; i < n* size ; i++) {

        send_array[i] = i + n * (rank + 1);
    }
    MPI_Reduce_scatter(&send_array[0], &recv_array[0], num, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < size; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i)
        {
            for (int j = 0; j < n; j++)
            {
                printf("Process %d recv[%d] is %d\n", rank, j, recv_array[j]);
            }
        }
    }
}