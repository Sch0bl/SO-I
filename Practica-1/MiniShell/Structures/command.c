#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

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

 void cmd_pipe_exec(struct Cmd* cmds, int **fd, int cmd_len){
	pid_t last_pid;
	fd = malloc(sizeof(int*) * (cmd_len));
	for (int i = 0; i < cmd_len ; i++) 
		fd[i] = malloc(sizeof(int) * 2);	

	for(int i = 0; i < cmd_len; i++){
		pipe(fd[i]);
		pid_t pid = fork();
		if ( i == cmd_len - 1)
			last_pid = pid;

		if ( pid == 0 ){
			if ( i != cmd_len - 1 ){
				dup2(fd[i][1], STDOUT_FILENO);
				close(fd[i][0]);
				close(fd[i][1]);
			} 
			if ( i != 0 ){
				dup2(fd[i - 1][0],STDIN_FILENO);
				close(fd[i - 1][0]);
			}
			cmd_exec(cmds[i]);
		} else {
			if ( i != 0)
				close(fd[i - 1][0]);
			if ( i == cmd_len )
				close(fd[i][0]);
			close(fd[i][1]);
		}
	}
	waitpid(last_pid, NULL,0);
	for(int i = 0; i < cmd_len ; i++)
		free(fd[i]);
	free(fd);
}

