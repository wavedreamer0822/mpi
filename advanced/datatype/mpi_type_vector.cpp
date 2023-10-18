#include<iostream>
#include"mpi.h"

using namespace std;


// int MPI_Type_vector(
//     int count,              // 块的数量
//     int blocklength,        // 每个块中所含元素的个数
//     int stride,             // 各块第一个元素之间相隔的元素数
//     MPI_Datatype oldtype,   // 旧数据类型
//     MPI_Datatype *newtype   // 新数据类型
// )
//允许复制的数据之间有空隙
int main()
{
    int rank;
    int n = 10;
    int buffer[10];
    int i;
    MPI_Datatype newtype;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Type_vector(3,1, 3, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);
    MPI_Request reqs;
    MPI_Status stats;

    if(rank == 0) {
        for(i = 0; i < n; i++) {
            buffer[i] = i + 1;
        }
        MPI_Isend(&buffer, 1, newtype, 1, 99, MPI_COMM_WORLD,&reqs);
        MPI_Wait(&reqs,&stats);

    }
    if(rank == 1) {

        for(int i = 0; i < n; i++) {
            buffer[i] = 100;
        }
        MPI_Irecv(&buffer, 1, newtype, 0, 99, MPI_COMM_WORLD, &reqs);
        MPI_Wait(&reqs,&stats);
        int count;
        MPI_Get_count(&status,newtype,&count);
        //cout<<count<<endl;
        for(i = 0; i < n; i++) {
            printf("buffer[%d] is %d\n", i, buffer[i]);
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
    {
        for(int i = 0;i<n;i++)
        {
            cout<<buffer[i]<<endl;
        }
    }
    MPI_Type_free(&newtype);
    MPI_Finalize();
}