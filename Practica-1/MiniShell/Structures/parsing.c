#include "parsing.h"
#include "command.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void token_errors(char** unxp_token, const char* token, int* status){
	if (!*unxp_token)
		*unxp_token = END_LINE;
	fprintf(stderr, "MiniShell: Unexpected token \"%s\" after \"%s\".\n", *unxp_token, token);
	*status = 1;
	*unxp_token = NULL;
}

int parsing_input(char** imput_cmd, struct Cmd** cmds){
	char* token = strtok(*imput_cmd, DELIM);
	if (token == NULL)
		return 0;
	if (!strcmp(token, END_LINE))
		return 1;

	struct List list = list_create();
	int redirect_fd = NO_FD;
	char** tokens;
	int status = 0;
	int max_len = INIT_CMD;
	int counter = 0;	
	struct Cmd* new_cmds = malloc(sizeof(struct Cmd) * INIT_CMD);
	assert(new_cmds);

	while (token){
		if (strcmp(token, REDIRECT) == 0){ //token ~ ">"
			token = strtok(NULL, DELIM);
			if ((token == NULL) || !(strcmp(token, PIPE) && strcmp(token, REDIRECT))){
				printf("Hola\n");
				token_errors(&token, REDIRECT, &status);
			} else {
				if (redirect_fd > NO_FD)
					close(redirect_fd);
				assert((redirect_fd = open(token, O_CREAT | O_WRONLY | O_TRUNC, 0644)) >= 0);
				token = strtok(NULL, DELIM);
			}
		}	else if (!(token && strcmp(token, PIPE))) { //toke ~ "|"
			if (token){
				token = strtok(NULL, DELIM);
			}
			if (token && !(strcmp(token, PIPE))){
				token_errors(&token,PIPE, &status);
			} else {
				tokens = list_to_cmd(list);
				new_cmds[counter++] = cmd_make(&tokens, redirect_fd);
				redirect_fd = NO_FD;
				list_destroy(&list);
				if ( counter == max_len )
					assert((new_cmds = realloc(new_cmds, sizeof(struct List) * (max_len *= 2))));
			}
		} else {
			list = list_add(list, &token, TAIL);
			token = strtok(NULL, DELIM);
		}
	}
	if (status){
		list_destroy(&list);
		cmds_destroy(new_cmds, counter);
		free(new_cmds);
	}
	else{
		tokens = list_to_cmd(list);
		new_cmds[counter++] = cmd_make(&tokens, redirect_fd);
		list_destroy(&list);
		*cmds = new_cmds;
	}
	return status ? -1 : counter;
}
