#include<iostream>
#include"mpi.h"
using namespace std;

int main(int  argc,char* argv[])
{
    int myid,numprocess;
    int namelen;
    char process_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocess);
    MPI_Get_processor_name(process_name,&namelen);
    cout<<"Hello world!"<<"process"<<myid<<"of"<<numprocess<<"in"<<process_name<<endl;
    MPI_Finalize();
}
