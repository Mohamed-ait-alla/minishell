/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/15 18:21:12 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int ft_strlen(char *str)
// {
//     int i = 0;
//     while (str[i])
//         i++;
//     return (i);
// }

void    parsing_cmd(char *cmd)
{
    int i;
    
    i = 0;
    if (cmd == NULL || !ft_strncmp(cmd, "exit\n", 5))
    {
        printf("exit\n");
        while (i != 99999999)
            i++;
        free(cmd);
        exit(0);
    }
    
}

int main(int ac, char **av)
{
    // get all commands
    // t_commands  *cmds;
    char *input;

    (void)av;
    // check if we have any args
    if (ac != 1)
        custom_error("Error: run only the programme" ,"", 1);
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break ;
        // paring the command
        parsing_cmd(input);
        // cmds->args = split_the_cmd(input);
        free(input);
    }
}
