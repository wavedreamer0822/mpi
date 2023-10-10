#include<iostream>
#include<unistd.h>
#include"mpi.h"
using namespace std;

int main()
{
    MPI_Init(NULL,NULL);
    int a[5] = {1};
    int b[5];
    int rank ;
    int size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(rank == 0)
    {
        a[1] = 4;
        for(int i = 0;i<5;i++)
        {
            MPI_Send(&a[i],1,MPI_INT,1,99,MPI_COMM_WORLD);
            cout<<a[i]<<endl;
        }
    }
    sleep(2);
    //message cache!!!!
    if(rank==1)
    {
        for(int i = 0;i<5;i++)
        {
            MPI_Recv(&b[i],1,MPI_INT,0,99,MPI_COMM_WORLD,&status);
            cout<<b[i]<<endl;
        }
    }
    MPI_Finalize();
}