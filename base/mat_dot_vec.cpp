#include <iostream>
#include "mpi.h"
using namespace std;

const int col = 10;
const int row = 10;
int rank = 0;
int size = 0;
double A[row][col] = {0};
double B[col] = {0};
double temp[col] = {0};
double r[row] = {0};
MPI_Status status;
void master();
void slave();
int main()
{
    MPI_Init(NULL, NULL);
    // int rank = 0;
    // int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &::rank);
    MPI_Comm_size(MPI_COMM_WORLD, &::size);
    if (::rank == 0)
    {
        master();
    }
    else
    {
        slave();
    }
    MPI_Finalize();
}

void master()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            A[i][j] = i + j;
        }
    }
    for (int j = 0; j < col; j++)
    {
        B[j] = j;
    }
    MPI_Bcast(&B[0], col, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < row; i++)
    {
        MPI_Send(&A[i][0], col, MPI_DOUBLE, (i) % (::size - 1) + 1, (i + 1), MPI_COMM_WORLD);
    }
    for (int i = 0; i < row; i++)
    {
        double result;
        MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int row_tag = status.MPI_TAG - 1;
        r[row_tag] = result;
    }

    for(int i = 1;i<::size;i++)
    MPI_Send(&A[0][0], col, MPI_DOUBLE, i, (row + 1), MPI_COMM_WORLD);
    
    for (int i = 0; i < row; i++)
    {
        cout << r[i] << endl;
    }
}

void slave()
{
    MPI_Bcast(&B[0],col,MPI_DOUBLE,0,MPI_COMM_WORLD);
    
    while(1)
    {
        MPI_Recv(&temp[0],col,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if(status.MPI_TAG == row+1)
        {
            break;
        }
        int row_tag = status.MPI_TAG - 1;
        double result = 0;
        for(int j = 0;j<col;j++)
        {
            result += temp[j] * B[j];
        }
        MPI_Send(&result,1,MPI_DOUBLE,0,row_tag+1,MPI_COMM_WORLD);
    }
}