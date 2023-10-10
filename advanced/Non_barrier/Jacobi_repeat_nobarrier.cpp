#include<iostream>
#include"mpi.h"

using namespace std;

int main()
{
    const int col = 2000;
    const int row = 4;
    const int step = 10;
    double A[row][col] = {0};
    double B[row][col] = {0};
    double buf_up[col] = {0};
    double buf_down[col] = {0};
    MPI_Status status[4];
    MPI_Request requst[4];
    int rank,size;
    MPI_Init(NULL, NULL);
    double t1 = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0)
    {
        for (int j = 0; j < col; j++)
        {
            A[0][j] = 8;
            buf_up[j] = 8;
        }
    }
    if (rank == 3)
    {
        for (int j = 0; j < col; j++)
        {
            A[row - 1][j] = 8;
            buf_down[j] = 8;
        }
    }
    for (int i = 0; i < row; i++)
    {
        A[i][0] = 8;
        A[i][col - 1] = 8;
    }
    int up = rank - 1;
    int down = rank + 1;
    if (rank == 0)
    {
        up = MPI_PROC_NULL;
    }
    if (rank == 3)
    {
        down = MPI_PROC_NULL;
    }
    int k = 0;
    while(k <= step)
    {
        MPI_Isend(&A[0][0],col,MPI_DOUBLE,up,99,MPI_COMM_WORLD, &requst[0]);
        MPI_Isend(&A[row - 1][0],col,MPI_DOUBLE,down,0,MPI_COMM_WORLD, &requst[1]);
        MPI_Irecv(&buf_down[0],col,MPI_DOUBLE,down,99,MPI_COMM_WORLD,&requst[2]);
        MPI_Irecv(&buf_up[0],col,MPI_DOUBLE,up,0,MPI_COMM_WORLD,&requst[3]);
        for (int i = 1; i < row - 1; i++)
        {
            for (int j = 1; j < col-1; j++)
            {
                B[i][j] = 0.25*(A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]);
            }
        }
        for(int i = 0;i<4;i++)
        {
            MPI_Wait(&requst[i],&status[i]);
        }
        for (int j = 1; j < col - 1; j++)
        {
            B[0][j] = 0.25*(buf_up[j] + A[1][j] + A[0][j - 1] + A[0][j + 1]);
        }
        for(int j = 1;j<col-1;j++)
        {
            B[row - 1][j] = 0.25*(buf_down[j] + A[row - 2][j] + A[row - 1][j - 1] + A[row - 1][j + 1]);
        }
        for(int i = 0;i<row ;i++)
        {
            for(int j = 1;j<col - 1;j++)
            {
                A[i][j] = B[i][j];
            }
        }
        k++;
    }
    double t2 = MPI_Wtime();
    cout<<"time = "<<t2 - t1<<"s"<<endl;
    // if(rank == 1)
    // {
    //     std::cout<<"rank = "<<rank<<endl;
    //     for(int i = 0;i<row;i++)
    //     {
    //         for(int j = 0;j<col;j++)
    //         {
    //             std::cout<<A[i][j]<<",   ";
    //         }
    //         std::cout<<endl;
    //     }
    // }

    MPI_Finalize();

}