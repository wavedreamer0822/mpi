#include <iostream>
#include "mpi.h"
#define SIZE 32  //不能超过64，否则newtype的空间量不够!!!!!!!!!!!

using namespace std;

int main()
{
    int rank;
    int size;
    int steps = 0;
    double A[SIZE / 2][SIZE / 2] = {0};
    double B[SIZE / 2][SIZE / 2] = {0};
    double buf_up[SIZE / 2] = {0};
    double buf_down[SIZE / 2] = {0};
    double buf_left[SIZE / 2] = {0};
    double buf_right[SIZE / 2] = {0};
    int dims[2] = {2, 2};
    MPI_Comm my_cart;
    int periods[2] = {0, 0};
    int reorder = 0;
    int coords[2];
    MPI_Request reqs[8];
    MPI_Status status[8];
    int left, right, up, down;
    for (int i = 0; i < 8; i++)
    {
        reqs[i] = MPI_REQUEST_NULL;
    }
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &my_cart);
    MPI_Cart_coords(my_cart, rank, 2, coords);
    MPI_Cart_shift(my_cart, 0, 1, &up, &down);
    MPI_Cart_shift(my_cart, 1, 1, &left, &right);
    if (up == MPI_PROC_NULL)
    {
        for (int i = 0; i < SIZE / 2; i++)
        {
            A[0][i] = 8;
            B[0][i] = 8;
            buf_up[i] = 8;
        }
    }

    if (down == MPI_PROC_NULL)
    {
        for (int i = 0; i < SIZE / 2; i++)
        {
            A[SIZE / 2 - 1][i] = 8;
            B[SIZE / 2 - 1][i] = 8;
            buf_down[i] = 8;
        }
    }
    if (left == MPI_PROC_NULL)
    {
        for (int i = 0; i < SIZE / 2; i++)
        {
            A[i][0] = 8;
            B[i][0] = 8;
            buf_left[i] = 8;
        }
    }
    if (right == MPI_PROC_NULL)
    {
        for (int i = 0; i < SIZE / 2; i++)
        {
            A[i][SIZE / 2 - 1] = 8;
            B[i][SIZE / 2 - 1] = 8;
            buf_right[i] = 8;
        }
    }
    MPI_Datatype newtype;
    MPI_Type_vector(SIZE / 2, 1, SIZE / 2, MPI_DOUBLE, &newtype);
    MPI_Type_commit(&newtype);
    int nbrs[4] = {left, right, down, up};
    int left_bound = 0,right_bound = SIZE / 2,up_bound = 0,down_bound = SIZE / 2;
    while (steps < 1)
    {
        MPI_Isend(&A[0][0], 1, newtype, left, 99, my_cart, &reqs[0]);
        MPI_Irecv(&buf_right, 1, newtype, right, 99, my_cart, &reqs[1]);

        MPI_Isend(&A[0][SIZE / 2 - 1], 1 , newtype, right, 0, my_cart, &reqs[2]);
        MPI_Irecv(&buf_left, 1, newtype, left, 0, my_cart, &reqs[3]);

        MPI_Isend(&A[0][0], SIZE / 2, MPI_DOUBLE, up, 1, my_cart, &reqs[4]);
        MPI_Irecv(&buf_down, SIZE / 2, MPI_DOUBLE, down, 1, my_cart, &reqs[5]);

        MPI_Isend(&A[SIZE / 2 - 1][0], SIZE / 2, MPI_DOUBLE, down, 2, my_cart, &reqs[6]);
        MPI_Irecv(&buf_up, SIZE / 2, MPI_DOUBLE, up, 2, my_cart, &reqs[7]);
        
        for (int i = 1; i < SIZE / 2 - 1; i++)
        {
            for (int j = 1; j < SIZE / 2 - 1; j++)
            {
                B[i][j] = 0.25 * (A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]);
            }
        }

        MPI_Waitall(8, reqs, status);

        if (left != -1)
        {
            for (int i = 1; i < SIZE / 2 - 1; i++)
            {
                B[i][0] = 0.25 * (buf_left[i] + A[i][1] + A[i + 1][0] + A[i - 1][0]);
            }
        }
        if (right != -1)
        {
            for (int i = 1; i < SIZE / 2 - 1; i++)
            {
                B[i][SIZE / 2 - 1] = 0.25 * (buf_right[i] + A[i][SIZE / 2 - 2] + A[i + 1][SIZE / 2 - 1] + A[i - 1][SIZE / 2 - 1]);
            }          
        }
        if (up != -1)
        {
            for (int i = 1; i < SIZE / 2 - 1; i++)
            {
                B[0][i] = 0.25 * (buf_up[i] + A[1][i] + A[0][i - 1] + A[0][i + 1]);
            }
        }
        if (down != -1)
        {
            for (int i = 1; i < SIZE / 2 - 1; i++)
            {
                B[SIZE / 2 - 1][i] = 0.25 * (buf_down[i] + A[SIZE / 2 - 2][i] + A[SIZE / 2 - 1][i - 1] + A[SIZE / 2 - 1][i + 1]);
            }
        }
        B[0][0] = 0.25 * (buf_left[0] + buf_up[0] + A[1][0] + A[0][1]);
        B[SIZE / 2 - 1][0] = 0.25 * (buf_down[0] + buf_left[SIZE / 2 - 1] + A[SIZE / 2 - 1][1] + A[SIZE / 2 - 2][0]);
        B[0][SIZE / 2 - 1] = 0.25 * (buf_right[0] + buf_up[SIZE / 2 - 1] + A[0][SIZE / 2 - 2] + A[1][SIZE / 2 - 1]);
        B[SIZE / 2 - 1][SIZE / 2 - 1] = 0.25 * (buf_right[SIZE / 2 - 1] + buf_down[SIZE / 2 ] + A[SIZE / 2 - 2][SIZE / 2 - 1] + A[SIZE / 2 - 1][SIZE / 2 - 2]);
        if(left == -1 )
        {
            left_bound = 1;
        }
        if(up == -1 )
        {
            up_bound = 1;
        }
        if(right == -1 )
        {
            right_bound = SIZE / 2 - 1;
        }
        if(down == -1 )
        {
            down_bound = SIZE / 2 - 1;
        }        
        for (int i = up_bound ; i < down_bound ; i++)
        {
            for (int j = left_bound; j < right_bound ; j++)
            {
                A[i][j] = B[i][j];
            }
        }
        steps++;
    }
    if (rank == 0)
    {
        for (int i = 0; i < SIZE / 2; i++)
        {
            for (int j = 0; j < SIZE / 2; j++)
            {
                cout << A[i][j] << ",  ";
            }
            cout << endl;
        }
       // cout<<A[0][0]<<endl;
    }
    MPI_Comm_free(&my_cart);
    MPI_Type_free(&newtype);
    MPI_Finalize();
}