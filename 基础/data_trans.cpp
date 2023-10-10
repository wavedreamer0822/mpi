#include <iostream>
#include "mpi.h"
using namespace std;
int main()
{
    int rank, size, value;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    do
    {
        if (rank == 0)
        {
            cout << "please give new value: ";
            cin >> value;
            cout << "rank = " << rank << "  size = "<< size <<"  value = " << value;
            if (size > 1)
            {
                MPI_Send(&value, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD);
            }
        }
        else if (rank == size - 1)
        {
            if(size > 1)
            {
                MPI_Recv(&value,1,MPI_INT,rank - 1,99,MPI_COMM_WORLD,&status);
            }
            else
            {
                MPI_Recv(&value,1,MPI_INT,rank,99,MPI_COMM_WORLD,&status);
            }  
            cout<<"rank = "<<rank<<"  size = "<< size <<"  value = "<<value<<endl;
        }
        else
        {
            MPI_Recv(&value,1,MPI_INT,rank - 1,99,MPI_COMM_WORLD,&status);
            MPI_Send(&value, 1, MPI_INT, rank + 1, 99, MPI_COMM_WORLD);
            cout<<"rank = "<<rank<<"  size = "<< size <<"  value = "<<value<<endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);

    } while (value >= 0);
    MPI_Finalize();
}