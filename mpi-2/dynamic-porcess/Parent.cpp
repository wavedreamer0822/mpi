#include "mpi.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{


    int world_size, universe_size;
    MPI_Comm everyone;  /* intercommunicator */
    MPI_Comm intracomm; // intracommunicator
    char worker_program[100];
    int parent_id;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &parent_id); // parnent process  id

    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // total process in current communication group
    ofstream out("parnet_data.txt" + std::to_string(parent_id));
    if (!out)
    {
        cerr << "Can not open the file!" << endl;
        exit(1);
    }
    //    if (world_size != 1) // only set one parent process
    //         cerr<<"Top heavy with management";

    // child process size
    universe_size = 5;

    // the program for child process
    char child[100] = "./Child";

    // create child process
    MPI_Comm_spawn(child, MPI_ARGV_NULL, universe_size,
                   MPI_INFO_NULL, 0, MPI_COMM_WORLD, &everyone,
                   MPI_ERRCODES_IGNORE);

    // create intra communicator
    MPI_Intercomm_merge(everyone, 0, &intracomm);

    // data to be spreaded
    int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // broadcast the data to all child process
    MPI_Bcast(&data, 10, MPI_INT, 0, intracomm);

    // receive data from child process
    int receive[10];
    MPI_Status status;
     
    //if (parent_id == 0)
    //{
        for (int j = 0; j < universe_size; ++j)
        {

            MPI_Recv(&receive, 10, MPI_INT, j, j, everyone, &status);
            out << "receive data from " << j << endl;
            for (int i = 0; i < 10; ++i) // write all data to file
                out << receive[i] << "  ";

            out << endl;
        }
    //}
    std::cout<<parent_id<<std::endl;
    MPI_Finalize();

    out.close();
    return 0;
}