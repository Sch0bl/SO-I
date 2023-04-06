#include "parsing.h"
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int parsing_input(char** imput_cmd, struct Cmd** cmds){
	char* token = strtok(*imput_cmd, DELIM);
	if (!strcmp(token, END_LINE))
		return 1;

	struct List list = list_create();
	char* redirect_file = NULL;
	char** tokens;
	int status = 0;
	int max_len = INIT_CMD;
	int counter = 0;	
	struct Cmd* new_cmds = malloc(sizeof(struct Cmd) * INIT_CMD);
	assert(new_cmds);

	while (token){
		if (!strcmp(token, REDIRECT)){
			token = strtok(NULL, DELIM);
			if ((token == NULL) || strcmp(token, PIPE) || strcmp(token, REDIRECT)){
				if (!token)
					token = END_LINE;
				fprintf(stderr, "MiniShell: Unexpected token \"%s\" after \"%s\".\n", token, REDIRECT);
				status = 1;
				token = NULL;
			} else {
				redirect_file = token;
				token = strtok(NULL, DELIM);
			}
		}	else if (!(token && strcmp(token, PIPE))) {
			if (token){
				token = strtok(NULL, DELIM);
			}
			if (token && !(strcmp(token, PIPE))){
				if (!token)
					token = END_LINE;
				fprintf(stderr, "MiniShell: Unexpected token \"%s\" after \"%s\".\n", token, PIPE);
				status = 1;
				token = NULL;
			} else {
				tokens = list_to_cmd(list);
				new_cmds[counter++] = cmd_make(&tokens, &redirect_file);
				redirect_file = NULL;
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
		cmd_destroy(new_cmds, counter);
		free(new_cmds);
	}
	else{
		tokens = list_to_cmd(list);
		new_cmds[counter++] = cmd_make(&tokens, &redirect_file);
		list_destroy(&list);
		*cmds = new_cmds;
	}
	return status ? -1 : counter;
}
