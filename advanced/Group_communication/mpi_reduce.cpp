#include <iostream>
#include "mpi.h"

using namespace std;

int main()
{
    int rank, size;
    int n = 2;
    int send_array[n];
    int recv_array[n];

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < n; i++)
    {
        send_array[i] = i + n * (rank + 1);
    }
    send_array[0] = 0;
    MPI_Reduce(&send_array[0], &recv_array[0], n, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (int j = 0; j < n; j++)
        {
            printf("Process %d recv[%d] is %d\n", rank, j, recv_array[j]);
        }
    }

    MPI_Finalize();
}