#include <iostream>
#include "mpi.h"
using namespace std;

int main()
{
    int size, rank;
    int n = 10;
    int send_array[n];
    int *rec_array;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    for (int i = 0; i < n; i++)
    {
        send_array[i] = i+rank*n;
    }
    if(rank == 0)
    {
        rec_array = (int *)malloc(sizeof(int) * n * size);
    }
    MPI_Gather(&send_array[0], n, MPI_INT, &rec_array[0], n, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(int i = 0;i<n*size;i++)
        {
            printf("rec_array[%i] = %i",i, rec_array[i]);
        }
        cout<<endl;
        free(rec_array);
    }
    MPI_Finalize();
}