#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#define tambuf 4*32
int main(int argc, char **argv)
{
  int pid, npr; 
  int i, numdat;
  int buf[tambuf], buf2[tambuf], modoacceso;
  MPI_File dat1;
  MPI_Status info;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &npr);
  numdat = 4;
  if (pid == 0)
    for(i=0; i<npr*numdat; i++) buf[i] = i*i; // Genera potencias
  if (pid == 0){
    modoacceso = (MPI_MODE_CREATE | MPI_MODE_WRONLY); // Establece el modo el archivo a crear
    MPI_File_open(MPI_COMM_SELF, "dat1", modoacceso, MPI_INFO_NULL, &dat1); // Crea el archivo 
    MPI_File_seek(dat1, 0, MPI_SEEK_SET); // Se posiciona en el comienzo del archivo
    MPI_File_write(dat1, buf, npr*numdat, MPI_INT, &info); // Escribe las potencias cargadas en el buffer
    MPI_File_close(&dat1); // Cierra el archivo
    printf("\n El master escribi ́o %d datos, desde 0 hasta %d \n\n",
        npr*numdat, buf[npr*numdat-1]);
  }
  sleep(3);
  modoacceso = MPI_MODE_RDONLY;
  MPI_File_open(MPI_COMM_WORLD, "dat1", modoacceso, MPI_INFO_NULL, &dat1); // Abre el archvi para lectura
  MPI_File_seek(dat1, pid*numdat*sizeof(int), MPI_SEEK_SET); // Posicionamieto relativo al proceso
  MPI_File_read(dat1, buf2, numdat, MPI_INT, &info); // Escritura
  MPI_File_close(&dat1); // Cierre del archivo
  printf(" > %d ha leido %5d %5d %5d %5d \n",
      pid, buf2[0], buf2[1], buf2[2], buf2[3]);
  MPI_Finalize();
  return 0;
}

/*
  Entre las lineas 17 y 27, el proceso 0 se encarga de crear el archivo dat1 y cargarlo con las
  potencias enteras ordenadas del 0 hasta 4*npr (npr <=> número de procesos).
  Luego de esta carga, se uncuentra un sleep de 3 segundos que permite que el proceso 0 pueda 
  realizar la escritura sin race condition.
  Luego Del sleep, se cargan concurrentemente las potencias cargadas en el archivo dat1 en el
  buffer "buf2". Luego de que cada proceso termina de cargar sus potencias las imprime
  por pantalla.
*/
