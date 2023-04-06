#ifndef __PARSING__
#define __PARSING__

#define INIT_CMD 2

#define DELIM " "

#define END_LINE	"\\n" 
#define PIPE "|"
#define REDIRECT ">"

#include "command.h"
#include "list.h"


int parsing_input(char** imput_cmd, struct Cmd** cmds);


#endif


