#include<iostream>
#include<string.h>
#include"mpi.h"
using namespace std;

int main(int argc, char*argv[])
{
    char message[20];
    int myrank;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    if(myrank==0)
    {
        strcpy(message,"Hello,process 1");
        MPI_Send(message,strlen(message),MPI_CHAR,1,99,MPI_COMM_WORLD);
        cout<<message<<endl;
    }
    else if(myrank == 1)
    {
        MPI_Status status;
        MPI_Recv(message,20,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        cout<<message<<endl;
    }
    MPI_Finalize();
}