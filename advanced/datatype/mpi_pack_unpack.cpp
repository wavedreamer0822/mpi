#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include "mpi.h"

void pack_and_unpack() {
    int rank;
    int i;
    double d;
    char c;
    char buffer[13];
    int position = 0;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        i = 1;
        d = 4.5;
        c = 'a';
        MPI_Pack(&i, 1, MPI_INT, buffer, 20, &position, MPI_COMM_WORLD);
        MPI_Pack(&d, 1, MPI_DOUBLE, buffer, 20, &position, MPI_COMM_WORLD);
        MPI_Pack(&c, 1, MPI_CHAR, buffer, 20, &position, MPI_COMM_WORLD);
        printf("postion is %d\n", position);

        int count;
        MPI_Pack_size(1,MPI_INT,MPI_COMM_WORLD,&count);  //计算打包所需空间
        std::cout<<count<<std::endl;

        MPI_Send(buffer, position, MPI_PACKED, 1, 99, MPI_COMM_WORLD);
    }

    if(rank == 1) {
        MPI_Recv(buffer, 20, MPI_PACKED, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, 20, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 20, &position, &d, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 20, &position, &c, 1, MPI_CHAR, MPI_COMM_WORLD);

        printf("i is %d, d id %.2f, c is %c\n", i, d, c);
    }
    MPI_Finalize();
}

int main() {
   pack_and_unpack();
}