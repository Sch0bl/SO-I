#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

pid_t father_pid;
pid_t child_pid;

void INThandler(int);

void CHINThandler(int);
int main(void)
{
  father_pid = getpid();
  child_pid = fork();

  if (!child_pid){ 
    signal(SIGUSR1, CHINThandler);
    while (1);
  } else {
    signal(SIGUSR1, INThandler);
    sleep(1);
    kill(child_pid, SIGUSR1);
    while(1);
  }
  return 0;
}

void INThandler(int sig) {
  printf("PADRE: RECIBI SEÑAL\n");
  kill(child_pid, sig);
}

void CHINThandler(int sig) {
  printf("CHILD: RECIBI UNA SEÑAL\n");
  kill(father_pid, sig);
}

