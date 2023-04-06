#ifndef _COMMAND_
#define _COMMAND_

struct CMD{
	char** cmd;
	char* output_file;
	int len;
};


struct CMD make_cmd(char*** cmd, char** file);

int cmd_exec(struct CMD cmd, int status);

void destroy_cmd(struct CMD* cmd, int len);


#endif
