#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
  printf("Soy el padre con el id: %d",getpid());
  pid_t child1_pid = fork();

  if (!child1_pid) {
    printf("Soy el hijo 1 con el id: %d",getpid());
    exit(1);
  }

  pid_t child2_pid = fork();

  if (!child2_pid) {
    printf("Soy el hijo 2 con el id: %d",getpid());
    exit(2);
  }

  //waitpid(child1_pid, NULL, 0);
  while(1);
  return 0;
}
