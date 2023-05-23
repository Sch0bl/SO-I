#include "command.h"
#include "parsing.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>



int main(){
	int termina = 1;
	while(termina){
		char* input_command = NULL;
		size_t input_command_buff_size = 255;
		size_t input_len = 0;
		int cmd_len;
		int status = 0;
		int** fd = NULL; 
		struct Cmd* cmds = NULL;
		printf("-> ");
		input_len = getline(&input_command,&input_command_buff_size,stdin);
		input_command[input_len - 1] = 0;

		if (!strcmp(input_command,"exit")){
			free(input_command);
			exit(0);
		}

		cmd_len = parsing_input(&input_command, &cmds);

		if (cmd_len <= 0){

			if (cmd_len == 1){
				pid_t pid = fork();

				if (pid == 0){
					exit(cmd_exec(cmds[0]));
				}

				printf("%d\n", status);
				wait(&status);
				printf("%d\n", status);
			} else {
				pid_t nasty = fork();
				
				if (nasty == 0){
					cmd_pipe_exec(cmds, fd, cmd_len);	
					cmds_destroy(cmds, cmd_len);
					free(cmds);
					free(input_command);
					exit(1);
				}

				waitpid(nasty,&status,0);
			}
			printf("%d\n", status);
			if (status == -1)
				puts("Command not found");
		}
		free(input_command);
		if (cmds){
		cmds_destroy(cmds, cmd_len);
		free(cmds);
		}
	}
	return 0;
}
