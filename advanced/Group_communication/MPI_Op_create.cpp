#include <iostream>
#include "mpi.h"

using namespace std;
//必须定义满足结合律的操作
void my_Op(void *in, void *inout, int *len, MPI_Datatype *datatype)
{
    int *in_tmp = (int *)in;
    int *inout_tmp = (int *)inout;
    for (int i = 0; i < *len; i++)
    {
        cout << *in_tmp << "   " << *inout_tmp << endl;
        *inout_tmp = *in_tmp - *inout_tmp;
        in_tmp++;
        inout_tmp++;
    }
}

int main()
{
    int rank;
    int size;
    int n = 10;
    int in[n],out[n];
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    for(int i = 0;i<n;i++)
    {
        in[i] = i*rank;
    }

    MPI_Op my_op;
    MPI_Op_create(my_Op,0,&my_op);
    MPI_Reduce(&in[0],&out[0], n, MPI_INT, my_op, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        for(int i = 0; i < n; i++) {
            printf("out[%d] is %d\n", i, out[i]);
        }
    }
    MPI_Finalize();
    
}