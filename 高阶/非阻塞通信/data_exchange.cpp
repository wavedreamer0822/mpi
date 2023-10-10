#include<iostream>
#include<string>
#include<string.h>
#include"mpi.h"

using namespace std;

int main()
{
    string s;
    int buffer[2];
    MPI_Status status;
    int rank,size;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(size<2)
    {
        MPI_Abort(MPI_COMM_WORLD,1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for(int i =0;i<size;i++)
    {
        if(i!=rank)
        {
            buffer[0] = rank;
            buffer[1] = i;
            MPI_Send(buffer, 2, MPI_INT, i, 99, MPI_COMM_WORLD);
            MPI_Recv(buffer,2,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            cout<<"hello from "<<buffer[0]<<" to "<<buffer[1]<<endl;
        }
    }
    MPI_Finalize();
    
}