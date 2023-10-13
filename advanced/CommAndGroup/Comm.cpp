#include<iostream>
#include"mpi.h"

using namespace std;

// MPI中的通信域综合描述了通信进程之间的通信关系，一个通信域包括：自身对应的进程组，通信上下文，虚拟处理器拓扑，属性等等。需要注意的是，一旦一个通信域被创建，通信域和进程组是一一对应的（组间通信域除外）。
// 在下面的例子中，实现了两个新的通信域的建立，分别是只包括奇数进程与只包括偶数进程的两个通信域。分别在各自的通信域中进行reduce操作。在这里使用的是通过新建进程组进而新建通信域的方法，实际中MPI_Comm_split函数可能会更加方便。
// 需要注意两点：1、某一通信域的操作必须只能由对应进程组内的进程执行，例如下面例子中如果把reduce与free操作放在条件语句之外会报错。
// 2、不能忘记各进程在新的通信域内的编号可能已经发生变化。

int main(int argc,char** argv)
{
  int myrank,world_size,commsize1,commsize2;
  int ranksize;
  int value ;
  int *ranks;
  MPI_Init(NULL,NULL);
  MPI_Group WORLD_GROUP,MY_GROUP_ODD,MY_GROUP_NO;
  MPI_Comm MY_COMM_ODD,MY_COMM_NO;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_group(MPI_COMM_WORLD, &WORLD_GROUP);
  if(world_size%2==0) ranksize=world_size/2;
  else ranksize=world_size/2+1;
  ranks=(int *)malloc(ranksize*sizeof(int));
  for(int i=0;i<ranksize;i++){ranks[i]=2*i+1;}
  MPI_Group_excl(WORLD_GROUP,ranksize,ranks,&MY_GROUP_NO);//得到偶数进程组
  MPI_Group_difference(WORLD_GROUP,MY_GROUP_NO,&MY_GROUP_ODD);//奇数进程组
  MPI_Comm_create(MPI_COMM_WORLD,MY_GROUP_NO,&MY_COMM_NO);//生成通信域
  MPI_Comm_create(MPI_COMM_WORLD,MY_GROUP_ODD,&MY_COMM_ODD);

  if(myrank%2==1){
    MPI_Reduce(&myrank,&value,1,MPI_INT,MPI_SUM,0,MY_COMM_ODD);//需要注意此时进程1在新通信域中编号为0.
    if(myrank == 1)
    {
        cout<<value<<endl;
    }
    
    MPI_Comm_free(&MY_COMM_ODD);
    MPI_Group_free(&MY_GROUP_ODD);
  }
  else{
    MPI_Reduce(&myrank,&value,1,MPI_INT,MPI_SUM,0,MY_COMM_NO);
    if(myrank == 0)
    {
        cout<<value<<endl;
    }
    MPI_Comm_free(&MY_COMM_NO);
    MPI_Group_free(&MY_GROUP_NO);
  }
  MPI_Finalize();
}