#include<iostream>
#include"mpi.h"

using namespace std;

typedef struct
{
    double a;
    double b;
    int c;
    char s;
} buf;

int main()
{
    MPI_Init(NULL,NULL);
    buf B;
    MPI_Aint a1,a2,a3,a4;
    MPI_Get_address(&B.a,&a1);
    MPI_Get_address(&B.b,&a2);
    MPI_Get_address(&B.c,&a3);
    MPI_Get_address(&B.s,&a4);
    cout<<a1<<"  "<<a2<<"  "<<a3<<"  "<<a4<<endl;
    MPI_Finalize();
}