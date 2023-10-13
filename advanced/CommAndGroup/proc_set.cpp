#include<iostream>
#include"mpi.h"

using namespace std;

//打印某个进程组中进程在MPI_COMM_WORLD中的进程号
void printf_ranknumber_in_world(MPI_Group group,MPI_Group world_group)
{
  int size;
  int *rank1;
  int *rank2;
  MPI_Group_size(group,&size);
  rank1=(int *)malloc(size*sizeof(int));
  rank2=(int *)malloc(size*sizeof(int));
  for(int i=0;i<size;i++){rank1[i]=i;}
  MPI_Group_translate_ranks(group,size,rank1,world_group,rank2);
  for(int j=0;j<size;j++){printf("%d,",rank2[j]);}
  printf("\n");
  MPI_Group_free(&group);
}

int main(int argc,char** argv)
{
  int myrank,proc_nums;
  MPI_Group WORLD_GROUP,MY_GROUP12345,MY_GROUP67,MY_GROUP54321,MY_GROUP13567;
  MPI_Group MY_GROUP12345_,MY_GROUP1234567;
  MPI_Comm MY_COMM;
  int rank_operation[]={0,6,7};
  int *rank_resort;
  int resort_size;
  int ranges[2][3];

  MPI_Init(&argc,&argv);
  //MPI_COMM_WORLD对应的进程组是全体进程，该进程组不是以预定义常数出现的，但是可以用MPI_COMM_GROUP来得到。
  MPI_Comm_group(MPI_COMM_WORLD,&WORLD_GROUP);  //返回指定通信域对应的进程组
  cout<<WORLD_GROUP<<endl;
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);//得到每个进程在COMM_WORLD中的进程号，作为基准。
  //从COMM_WORLD进程组中删去rank号为0，6，7的进程，剩下的组成新进程组MY_GROUP12345。
  //挑出6，7构成进程组MY_GROUP67。
  MPI_Group_excl(WORLD_GROUP,3,rank_operation,&MY_GROUP12345);
  rank_operation[0]=6;
  rank_operation[1]=7;
  MPI_Group_incl(WORLD_GROUP,2,rank_operation,&MY_GROUP67);
  //把MY_GROUP12345中的进程逆序得到MY_GROUP
  MPI_Group_size(MY_GROUP12345,&resort_size);
  rank_resort=(int *)malloc(sizeof(int)*resort_size);
  for(int i=0;i<resort_size;i++){
    rank_resort[i]=resort_size-i-1;
  }
  MPI_Group_incl(MY_GROUP12345,resort_size,rank_resort,&MY_GROUP54321);

  //按照三元组指定的进程构建新的进程组MY_GROUP135

  ranges[0][0]=0;
  ranges[0][1]=5;
  ranges[0][2]=2;
  MPI_Group_range_excl(WORLD_GROUP,1,ranges,&MY_GROUP13567);
  //∩
  MPI_Group_intersection(MY_GROUP12345,WORLD_GROUP,&MY_GROUP12345_);
  //∪
  MPI_Group_union(MY_GROUP12345_,MY_GROUP67,&MY_GROUP1234567);

  if(myrank==0){
    printf("MY_GROUP67 has process:");
    printf_ranknumber_in_world(MY_GROUP67,WORLD_GROUP);
  }
  cout<<"end"<<endl;
  MPI_Finalize();
}