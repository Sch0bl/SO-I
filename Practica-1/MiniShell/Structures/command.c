#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

struct CMD cmd_make(char*** cmd, char** file){
	struct CMD new_cmd;
	new_cmd.cmd = *cmd;
	new_cmd.output_file = *file;
	return new_cmd;
}

//void destroy_cmd(struct CMD del_cmd){
//	free(del_cmd.cmd);
//	free(del_cmd.output_file);
//}

int cmd_exec(struct CMD cmd, int status){
	if(cmd.output_file != NULL){
		int output_fd = open(cmd.output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	execvp(cmd.cmd[0], cmd.cmd);
	return 1;
}



