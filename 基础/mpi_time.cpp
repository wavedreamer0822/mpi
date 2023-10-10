#include <iostream>
#include<unistd.h>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    double t1, t2;
    t1 = MPI_Wtime();
    t2 = MPI_Wtime();
    cout << t2 - t1 << endl;
    cout << MPI_Wtick() << endl;
    for (int i = 0; i < 10; i++)
    {
        t1 = MPI_Wtime();
        sleep(1);
        t2 = MPI_Wtime();
        cout << t2 - t1 << endl;
    }
    MPI_Finalize();
}
