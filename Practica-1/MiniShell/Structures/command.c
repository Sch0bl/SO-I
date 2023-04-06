#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

struct Cmd cmd_make(char*** cmd, char** file){
	struct Cmd new_cmd;
	new_cmd.cmd = *cmd;
	new_cmd.output_file = *file;
	return new_cmd;
}

//void destroy_cmd(struct Cmd del_cmd){
//	free(del_cmd.cmd);
//	free(del_cmd.output_file);
//}

int cmd_exec(struct Cmd cmd){
	if(cmd.output_file != NULL){
		int output_fd = open(cmd.output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	execvp(cmd.cmd[0], cmd.cmd);
	return 1;
}


void cmd_destroy(struct Cmd* cmd, int len){
	for(int i = 0; i < len; i++)
		free(cmd[i].cmd);
}


