#include <mpi.h>
#include <stdio.h>


int main(int argc, char **argv) {
  int num_procs, my_id, msg, buff;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (my_id == 0) {
      printf("Ingrese un mesage para mandar: \n");
      scanf("%d",&buff);
      msg = buff;
      MPI_Send(&buff, 1, MPI_INT, (my_id + 1) % num_procs, 0, MPI_COMM_WORLD);
      MPI_Recv(&buff, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD, NULL);
      if (buff == msg)
        printf("Finalizo.\n");
      else
        printf("Mensaje erroneo.\n");
    } else {
      MPI_Recv(&buff, 1, MPI_INT, (my_id - 1) % num_procs, 0, MPI_COMM_WORLD, NULL);
      printf("Soy el proceso %d, recivi el mensaje %d de %d, se lo mando a %d.\n", my_id, buff , (my_id - 1) % num_procs,  (my_id + 1) % num_procs);
      MPI_Send(&buff, 1, MPI_INT, (my_id + 1) % num_procs, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
