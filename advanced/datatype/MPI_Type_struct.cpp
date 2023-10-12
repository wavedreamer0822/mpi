#include<iostream>
#include"mpi.h"

using namespace std;
    typedef struct _my_struct {
    double d;
    double d2;
    int i;
    char c;
} my_struct;
#define NELEM 25
int main() {
    int numtask,rank,source = 0,dest,tag = 1,i,j,k;
    typedef struct{
        char pt;
        double pos[3];
        double vel[3];
        double orientation[3][3];
        int n,type;
    } Particle;
    Particle p[NELEM],particles[NELEM];
    MPI_Datatype particletype;
    MPI_Datatype oldtype[5]= {MPI_CHAR,MPI_DOUBLE,MPI_DOUBLE,MPI_DOUBLE,MPI_INT};
    int blockcount[5]= {1,3,3,9,2};
    MPI_Aint offset[5];
    MPI_Status stat;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtask);
    MPI_Address(&particles[0].pt,&offset[0]);
    MPI_Address(particles[0].pos,&offset[1]);
    MPI_Address(particles[0].vel,&offset[2]);
    MPI_Address(particles[0].orientation,&offset[3]);
    MPI_Address(&(particles[0].n),&offset[4]);
    for(i=4;i >= 0;i--)
        offset[i] -= offset[0];
    MPI_Type_struct(5,blockcount,offset,oldtype,&particletype);
    MPI_Type_commit(&particletype);
    if(rank == 0)
    {
        for(i=0;i<NELEM;i++){
            particles[i].pt = 'H';
            particles[i].pos[0] = 0.0;
            particles[i].pos[1] = 1.0;
            particles[i].pos[2] = 2.0;
            particles[i].vel[0] = 100.0;
            particles[i].vel[1] = 200.1;
            particles[i].vel[2] = 300.2;
            particles[i].n = 5;
            particles[i].type = 10;
            for(j=0;j<3;j++)
                for(k=0;k<3;k++)
                    particles[i].orientation[j][k] = 0.99;
        }
        for(i=1;i<numtask;i++)
        {
            MPI_Send(particles,NELEM,particletype,i,tag,MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(p,NELEM,particletype,source,tag,MPI_COMM_WORLD,&stat);
        cout<<p[2].pt<<std::endl;
        cout<<p[2].pos[0]<<'\t'<<p[2].pos[1]<<'\t'<<p[2].pos[2]<<std::endl;
        cout<<p[2].vel[0]<<'\t'<<p[2].vel[1]<<'\t'<<p[2].vel[2]<<std::endl;
        cout<<p[2].n<<'\t'<<p[2].type<<std::endl;
        for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                {
                  cout<<p[2].orientation[i][j]<<'\t';
                }
                cout<<std::endl;
            }
    }
    MPI_Finalize();

}