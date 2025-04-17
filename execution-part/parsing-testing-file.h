//
//      this file is just for testing!!!!
//

#ifndef PARSING_TESTING_FILE_H
#define PARSING_TESTING_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 100

typedef struct s_command {
	char **env;
    char **args;
    char *infile;
    char *outfile;
    int append;
    int heredoc;
} t_command;

#endif