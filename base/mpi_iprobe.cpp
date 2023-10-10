#include<iostream>
#include"mpi.h"
using namespace std;

int main()
{
    int rank = 0;
    int size = 0;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Request requst;
    MPI_Status status;
    int value;
    if(rank == 0)
    {
        value = 0;
        MPI_Isend(&value,1,MPI_INT,1,99,MPI_COMM_WORLD,&requst);
        MPI_Request_free(&requst);
    }
    else if(rank == 1)
    {
        int have_msg;
        for(int i = 0;i<5;i++)
        {
            MPI_Iprobe(0,99,MPI_COMM_WORLD,&have_msg,&status); //check msg if arrive at receive buffer
            cout<<have_msg<<endl;
        }
        if(have_msg)
        {
            int reve_count = 0;
            MPI_Get_count(&status,MPI_INT,&reve_count);
            cout<<"receive count = "<<reve_count<<endl;
        }
    }
}