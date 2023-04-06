#ifndef _COMMAND_
#define _COMMAND_

struct Cmd{
	char** cmd;
	char* output_file;
	int len;
};


struct Cmd cmd_make(char*** cmd, char** file);

int cmd_exec(struct Cmd cmd);

void cmd_destroy(struct Cmd* cmd, int len);


#endif
