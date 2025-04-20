
//
//				this file is just for testing!!!!!!
//


#include "parsing-testing-file.h"
# include "../includes/minishell.h"
// Helper to skip whitespace
char *skip_spaces(char *str) {
    while (*str == ' ' || *str == '\t')
        str++;
    return str;
}

// Helper to split by pipes (|) into an array
char **split_by_pipe(char *input, int *count) {
    char **commands = malloc(sizeof(char *) * (MAX_ARGS));
    *count = 0;
    char *token = strtok(input, "|");
    while (token && *count < MAX_ARGS - 1) {
        commands[(*count)++] = strdup(skip_spaces(token));
        token = strtok(NULL, "|");
    }
    commands[*count] = NULL;
    return commands;
}

// Helper to parse redirections and arguments from a command string
t_command *parse_command(char *cmd_str) {
    t_command *cmd = calloc(1, sizeof(t_command));
    cmd->args = malloc(sizeof(char *) * MAX_ARGS);
    int argc = 0;

    char *token = strtok(cmd_str, " \t");
    while (token) {
        if (strcmp(token, ">") == 0) {
            cmd->outfile = strdup(strtok(NULL, " \t"));
            cmd->append = 0;
        } else if (strcmp(token, ">>") == 0) {
            cmd->outfile = strdup(strtok(NULL, " \t"));
            cmd->append = 1;
        } else if (strcmp(token, "<") == 0) {
            cmd->infile = strdup(strtok(NULL, " \t"));
            cmd->heredoc = 0;
        } else if (strcmp(token, "<<") == 0) {
            cmd->infile = strdup(strtok(NULL, " \t"));
            cmd->heredoc = 1;
        } else {
            cmd->args[argc++] = strdup(token);
        }
        token = strtok(NULL, " \t");
    }
    cmd->args[argc] = NULL;
    return cmd;
}

// Free command array
void free_commandds(t_command **cmds, int count) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; cmds[i]->args[j]; j++)
            free(cmds[i]->args[j]);
        free(cmds[i]->args);
        free(cmds[i]->infile);
        free(cmds[i]->outfile);
        free(cmds[i]);
    }
    free(cmds);
}

// Dummy executor
void execute_commands(t_command **cmds, int count, char **env) {
	for (int i=0; i < count; i++)
		cmds[i]->env = env;
	// for (int i =0; cmds[0]->env[i]; i++)
	// 	printf("%s\n", cmds[0]->env[i]);
	
	tested_main_with_parsing(cmds, count);
    // for (int i = 0; i < count; i++) {
    //     printf("Command %d:\n", i + 1);
	// 	for (int j = 0; cmds[i]->args[j]; j++)
	// 		printf("  Arg[%d]: %s\n", j, cmds[i]->args[j]);
    //     if (cmds[i]->infile)
    //         printf("  Input: %s (%s)\n", cmds[i]->infile, cmds[i]->heredoc ? "heredoc" : "file");
    //     if (cmds[i]->outfile)
    //         printf("  Output: %s (%s)\n", cmds[i]->outfile, cmds[i]->append ? "append" : "overwrite");
    // }
}

// void	execute_commands(t_command *cmds, int cmd_count)
// {
// 	int	i;

// 	i = 0;
// 	while (cmds->args)
// 	{
// 		printf("args[%d] = %s\n", i + 1, cmds->args[i]);
// 		i++;
// 	}
// }

int main(int ac, char **av, char **env) {
    char *input;
	char **envp;
	envp = copy_env(env);

    while ((input = readline("minishell$ "))) {
        if (*input)
            add_history(input);

        int cmd_count = 0;
        char **cmd_strs = split_by_pipe(input, &cmd_count);

        t_command **cmds = malloc(sizeof(t_command *) * (cmd_count + 1));
        for (int i = 0; i < cmd_count; i++) {
            cmds[i] = parse_command(cmd_strs[i]);
            free(cmd_strs[i]);
        }
        cmds[cmd_count] = NULL;
        free(cmd_strs);

        execute_commands(cmds, cmd_count, envp); // Replace with your real execution logic

        free_commandds(cmds, cmd_count);
        free(input);
    }

    return 0;
}
