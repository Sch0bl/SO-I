#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#define INPUT 0
#define OUTPUT 1 

struct Cmd cmd_make(char*** cmd, int fd){
	struct Cmd new_cmd;
	new_cmd.cmd = *cmd;
	new_cmd.output_fd = fd;
	return new_cmd;
}

//void destroy_cmd(struct Cmd del_cmd){
//	free(del_cmd.cmd);
//	free(del_cmd.output_file);
//}

int cmd_exec(struct Cmd cmd){
	if(cmd.output_fd>= 0){
		dup2(cmd.output_fd, STDOUT_FILENO);
		close(cmd.output_fd);
	}
	return execvp(cmd.cmd[0], cmd.cmd);
}


void cmds_destroy(struct Cmd* cmd, int len){
	for(int i = 0; i < len; i++)
		cmd_destroy(cmd[i]);
}

void cmd_destroy(struct Cmd cmd){
	free(cmd.cmd);
	if (cmd.output_fd > -1)
		close(cmd.output_fd);
}

 void cmd_pipe_exec(struct Cmd* cmds, int cmd_len){
	pid_t last_pid;
	int **fd = malloc(sizeof(int*) * (cmd_len));
	for (int i = 0; i < cmd_len ; i++) 
		fd[i] = malloc(sizeof(int) * 2);	

	for(int i = 0; i < cmd_len; i++){ 
		pipe(fd[i]);
		pid_t pid = fork();
		if ( i == cmd_len - 1)
			last_pid = pid;

		if ( pid == 0 ){ // Estoy en el hijo
                     
			if ( i != 0 ){ // Si no soy el primero
				dup2(fd[i - 1][INPUT],STDIN_FILENO);
				close(fd[i - 1][INPUT]);
			}

			if ( i != cmd_len - 1 ){ // Si no soy el último
				dup2(fd[i][OUTPUT], STDOUT_FILENO);
				close(fd[i][INPUT]);
				close(fd[i][OUTPUT]);
			} 
      
			cmd_exec(cmds[i]);
		} else {
			if (i != 0)
				close(fd[i - 1][INPUT]);
      if (i != cmd_len - 1)
			  close(fd[i][OUTPUT]);
		}
	}
	waitpid(last_pid, NULL,0);
	for(int i = 0; i < cmd_len ; i++)
		free(fd[i]);
	free(fd);
}

