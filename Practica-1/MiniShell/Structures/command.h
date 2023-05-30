#ifndef _COMMAND_
#define _COMMAND_

struct Cmd{
	char** cmd;
	int output_fd;
};


struct Cmd cmd_make(char*** cmd, int fd);

int cmd_exec(struct Cmd cmd);

void cmds_destroy(struct Cmd* cmd, int len);

void cmd_destroy(struct Cmd cmd);

void cmd_pipe_exec(struct Cmd* cmds, int cmd_len);
#endif
