#include "parsing.h"
#include "command.h"
#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


int parsing_input(char** imput_cmd, struct CMD* cmds){
	struct List list = list_create();
	char* token = strtok(*imput_cmd, DELIM);
	char* redirect_file;
	char** tokens;
	if (strcmp(token, END_LINE))
		return 1;
	int status = 0;
	int max_len = INIT_CMD;
	int counter = 0;	
	int flag = 1;
	

	struct CMD* new_cmds = malloc(sizeof(struct CMD) * INIT_CMD);
	assert(new_cmds);

	

	while (flag){
		if (!strcmp(token, REDIRECT)){
			token = strtok(NULL, DELIM);
			if (!(strcmp(token, END_LINE) || strcmp(token, PIPE) || strcmp(token, REDIRECT))){
				fprintf(stderr, "MiniShell: Unexpected token %s after %s.", token, REDIRECT);
				status = 1;
				flag = 0;
			} else {
				redirect_file = token;
			}
		}	else if (!((flag = strcmp(token, END_LINE)) || strcmp(token, PIPE))) {
			if (flag)
				token = strtok(NULL, DELIM);
			if (!(strcmp(token, END_LINE) || strcmp(token, PIPE))){
				fprintf(stderr, "MiniShell: Unexpected token %s after %s.", token, REDIRECT);
				status = 1;
				flag = 0;
			} else {
				tokens = list_to_cmd(list);
				new_cmds[counter++] = make_cmd(&tokens, &redirect_file);
				if ( counter == max_len )
					new_cmds = realloc(new_cmds, sizeof(struct List) * (max_len *= 2));
			}
		}
	}

}
