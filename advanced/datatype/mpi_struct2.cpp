#include<iostream>
#include"mpi.h"

using namespace std;

typedef struct _my_struct {
    char c;
    double d;
    double d2;
    int i;
    
} my_struct;

int main() {
    int rank;
    int blocklength[3];
    MPI_Aint index[3];
    MPI_Datatype oldtype[3];
    MPI_Datatype newtype;
    MPI_Status status;
    my_struct data;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    blocklength[0] = 1;
    blocklength[1] = 2;
    blocklength[2] = 1;
    // index[0] = 0;
    // index[1] = sizeof(MPI_DOUBLE) * 2;
    // index[2] = sizeof(MPI_DOUBLE) * 2 + sizeof(MPI_INT) + sizeof(MPI_CHAR);
    MPI_Address(&data.c,&index[0]);
    MPI_Address(&data.d,&index[1]);
    MPI_Address(&data.i,&index[2]);
    for(int i = 2;i>=0;i--)
    {
        index[i] = index[i] - index[0];
    }
    cout<<index[0]<<"  "<<index[1]<<"  "<<index[2]<<endl;
    oldtype[0] = MPI_CHAR;
    oldtype[1] = MPI_DOUBLE;
    oldtype[2] = MPI_INT;

    MPI_Type_struct(3, blocklength, index, oldtype, &newtype);
    MPI_Type_commit(&newtype);

    if(rank == 0) {
        data.d = 2.0;
        data.d2 = 3.5;
        data.i = 4;
        data.c = 'c';
        MPI_Send(&data, 1, newtype, 1, 99, MPI_COMM_WORLD);
    }
    if(rank == 1) {
        MPI_Recv(&data, 1, newtype, 0, 99, MPI_COMM_WORLD, &status);
        printf("data.d is %.2f, data.d2 is %.2f, i is %d, data.c is %c\n", data.d, data.d2, data.i, data.c);
    }
    MPI_Type_free(&newtype);
    MPI_Finalize();
}