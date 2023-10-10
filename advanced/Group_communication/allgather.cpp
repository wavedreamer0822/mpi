int MPI_Allgather(
    void * sendbuf,         // 发送缓冲区的起始地址
    int  sendcount,         // 向每个进程发送的数据个数
    MPI_Datatype sendtype,  // 发送数据类型
    void * recvbuf,         // 接收缓冲区的起始地址
    int recvcount,          // 接收数据的个数
    MPI_Datatype recvtype,  // 接收数据的类型
    MPI_Comm comm           // 通信域
)  //相当于每个进程执行了一次MPI_Gather()