#include <iostream>
#include "mpi.h"

using namespace std;
// MPI_Scan 都对排在他前面的进程进行规约操作
int main()
{
    int n = 2;
    int s[n];
    int r[n];
    MPI_Init(NULL, NULL);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 0; i < n; i++)
    {
        s[i] = i + n*(rank + 1);
    }
    MPI_Scan(&s[0],&r[0],n,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    for (int i = 0; i < size; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i)
        {
            for (int j = 0; j < n; j++)
            {
                printf("Process %d recv[%d] is %d\n", rank, j, r[j]);
            }
        }
    }
}