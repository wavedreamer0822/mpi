
/* child process */
#include <stdlib.h>
#include "mpi.h"
#include <iostream>
 
using namespace std;
int main(int argc, char *argv[])
{
   int size, num_proc;
   MPI_Status status;
   MPI_Comm everyone;
   int process_id;
   MPI_Comm parent;
   MPI_Comm intracomm;
   MPI_Init(&argc, &argv);
   MPI_Comm_get_parent(&parent);
   if (parent == MPI_COMM_NULL)
        cerr<<"No parent!";
   MPI_Comm_remote_size(parent, &size);
//    if (size != 1)
//         cerr<<"Something's wrong with the parent";
   MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
   printf("num proc = %d \n", num_proc);
 
   MPI_Comm_rank( MPI_COMM_WORLD, &process_id );
 
 
   MPI_Intercomm_merge(parent, 1, &intracomm);  //创立组内通信域 可以用一些组通信函数在此通信域内
 
   int buffer[10];
 
    //receive data from parent process
    // note that Bcast use the intracommunicator
   MPI_Bcast(&buffer, 10, MPI_INT, 0, intracomm );
 
   for( int i=0; i< 10; ++i )
        buffer[i] *= 10;
 
    //send data to parent process
    //note that Send use the intercommunicator
    for(int i = 0;i<size;i++)
    {
        MPI_Send(buffer, 10, MPI_INT, i, process_id, parent );
    }
    
 
 
   MPI_Finalize();
   return 0;
}