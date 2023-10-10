#include<iostream>
#include"mpi.h"

using namespace std;

int main()
{
    double a[10000] = {0};
    double b[10000] = {0};
    MPI_Init(NULL,NULL);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Status status;
    if(rank == 0)
    {
        a[1] = 1;
        MPI_Send(a,10000,MPI_DOUBLE,1,99,MPI_COMM_WORLD);
    }
    else if(rank == 1)
    {
        MPI_Recv(a,10000,MPI_DOUBLE,0,99,MPI_COMM_WORLD,&status); ///can transfer because Send & Recv start simultaneously;
        cout<<a[1]<<endl;
    }
    else if(rank == 2)
    {
        a[1] = 2;
        MPI_Send(a,10000,MPI_DOUBLE,3,1,MPI_COMM_WORLD);
        MPI_Recv(b,10000,MPI_DOUBLE,3,1,MPI_COMM_WORLD,&status);
        cout<<b[1]<<endl;
    }
    else
    {
        a[1] = 2;
        //MPI_Recv(b,10000,MPI_DOUBLE,2,1,MPI_COMM_WORLD,&status); //can transfer because Send & Recv start simultaneously; or use  Isend & Irecv
        MPI_Send(a,10000,MPI_DOUBLE,2,1,MPI_COMM_WORLD);
        MPI_Recv(b,10000,MPI_DOUBLE,2,1,MPI_COMM_WORLD,&status);  //cant transfer because of data size > buffer size(64KB)

        cout<<b[1]<<endl;
    }
    MPI_Finalize();
}