#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main() {
    int rank;
    int n = 100000;
    int a[n];
    int i;
    int has_finished;
    MPI_Request request;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        for(i = 0; i < n; i++) {
            a[i] = i;
        }
        MPI_Isend(a, n, MPI_INT, 1, 99, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &has_finished, &status);
        printf("before wait: send is %d\n", has_finished);
        MPI_Wait(&request, &status);
        MPI_Test(&request, &has_finished, &status);
        printf("after wait: send is %d\n", has_finished);

    }
    if(rank == 1) {
        MPI_Irecv(a, n, MPI_INT, 0, 99, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &has_finished, &status);
        printf("before wait: receive is %d\n", has_finished);
        printf("a[10] is %d\n", a[10]);
        MPI_Wait(&request, &status);
        MPI_Test(&request, &has_finished, &status);
        printf("after wait: receive is %d\n", has_finished);
        printf("a[10] is %d\n", a[10]);
    }
    MPI_Finalize();
}