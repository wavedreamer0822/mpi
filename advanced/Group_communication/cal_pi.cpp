#include <iostream>
#include "mpi.h"

using namespace std;

int main()
{
    int N = 10;
    int size = 0;
    int rank = 0;
    int start;
    int end;
    int unit_space = 0;
    int i;
    double pi = 0.0;
    double result;
    double x;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    unit_space = N / size;
    if (rank == size - 1)
    {
        start = unit_space * rank;
        end = N;
    }
    else
    {
        start = unit_space * rank;
        end = unit_space * (rank + 1);
    }
    for (i = start + 1; i <= end; i++)
    {
        x = (i - 0.5) / N;
        pi += fx(x);
    }
    MPI_Reduce(&pi, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        result /= N;
        printf("result is %.16f\n", result);
    }
    MPI_Finalize();
}