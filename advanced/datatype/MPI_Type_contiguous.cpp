#include<iostream>
#include"mpi.h"

using namespace std;
//MPI_Type_contiguous 多个相同的数据类型合成一个数据类型
typedef struct 
{
    int a;
    int b;
} contiguous_type;


int main()
{
    int rank;
    int size;
    contiguous_type data;
    MPI_Datatype newtype;
    MPI_Status status;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Type_contiguous(2, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);
    
    if(rank == 0)
    {
        data.a = 1;
        data.b = 2;
        MPI_Send(&data, 1, newtype, 1, 99, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&data, 1, newtype, 0,99,MPI_COMM_WORLD, &status);
        cout<<data.b<<data.a<<endl;
    }
    MPI_Type_free(&newtype);
    MPI_Finalize();

}