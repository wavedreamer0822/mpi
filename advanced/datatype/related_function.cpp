// int MPI_Type_extent(
//     MPI_Datatype datatype,  // 数据类型
//     MPI_Aint * extent       // 数据类型跨度
// )

// int MPI_Type_size(
//     MPI_Datatype datatype,  // 数据类型
//     int * size              // 数据类型跨度
// )

// MPI_Type_size 以字节为单位，返回给定数据有用部分所占空间的大小，
// 即跨度减去类型中的空隙后的空间大小，和 MPI_Type_extent 相比，MPI_Type_size 
// 不包括由于对齐等原因导致的数据类型中的空隙所占的空间。