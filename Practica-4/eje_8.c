#include <mpi.h>
#include <stdio.h>

void global_share_sum(int private_data, int id, int num_procs){
    
}

int main(int argc, char **argv) {
  int num_procs, my_id, sum, buff, sumando;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    sumando = my_id;
    sum = buff = sumando; // 
    if (my_id == 0) {
    for (int i = 0; i < num_procs - 1; i++){
      printf("Soy el proceso %d, mi suma actual es %d y estoy en la iteración %d\n", my_id, sum, i);
      MPI_Send(&buff, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&buff, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD, NULL);
      sum += buff;
      printf("Total de la suma %d, en la iteración %d, id %d\n", sum, i, my_id);
    } 

    } else {
      for (int i = 0; i < num_procs - 1; i++){
        printf("Soy el proceso %d, mi suma actual es %d y estoy en la iteración %d\n", my_id, sum, i);
        MPI_Send(&buff, 1, MPI_INT, (my_id + 1) % num_procs, 0, MPI_COMM_WORLD);
        MPI_Recv(&buff, 1, MPI_INT,  (my_id - 1) % num_procs, 0, MPI_COMM_WORLD, NULL);
        sum += buff;
      printf("Total de la suma %d, en la iteración %d, id %d\n", sum, i, my_id);
      } 
    }
    printf("Soy el id %d y mi suma quedó %d\n", my_id, sum);
    MPI_Finalize();
    return 0;
}
