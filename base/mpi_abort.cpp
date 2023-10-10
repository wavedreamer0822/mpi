#include <iostream>
#include <string.h>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[])
{
    int myid, size;
    int masterid = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 1; i < argc; i++)
    {
        cout << "myid = " << myid << "  proc = " << size << "  argv[" << i << "] = " << argv[i] << endl;
        if (argv[i] && strcmp("lastmaster", argv[i]) == 0)
        {
            masterid = i - 1;
        }
    }
    if (myid == masterid)
    {
        cout << "Abort id is " << myid << endl;
        MPI_Abort(MPI_COMM_WORLD, 99);
    }
    else
    {
        cout << "waiting...." << endl;
        MPI_Barrier(MPI_COMM_WORLD);
    }
}