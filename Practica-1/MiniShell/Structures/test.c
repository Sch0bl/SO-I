#include "parsing.h"
#include <string.h>

int main(){
	char input_command[21] = "ls -a | cat | sort -r";
	struct List list = list_create();
	char* token;
	token = strtok(input_command, " ");
	list_add(list, &token, TAIL);
	while((token = strtok(NULL, " "))){
		list_add(list, &token, TAIL);	
	}

	
}
