/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@stduent.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/06 14:58:58 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"             





void    check_for_redirections(char **av)
{
        int     i;
        
        i = 1;
        while (av && av[i])
        {
                
                if (ft_strcmp(av[i], ">") == 0) // redirect output to the specified file, overwrite it if it exist
                        redirect_output_to_file(av[i + 1], 'o');
                else if (ft_strcmp(av[i], ">>") == 0) // redirect output to the specified file, append to it if it exist 
                        redirect_output_to_file(av[i + 1], 'a');
                else if (ft_strcmp(av[i], "<") == 0) // redirect input to the specified file, so the command can read from it
                        redirect_input_to_file(av[i + 1]);
                else if (ft_strcmp(av[i], "<<") == 0) // here_document
                        redirect_input_to_file_here_doc(av[i + 1]);
                i++;  
        }
}

int     main(int ac, char **av, char **env)
{
        int pid;
        
        // check for pipes
        if (check_for_pipes(av))
                handle_pipes(av);
        // if no pipes are included execute other commands as normal
        pid = fork();
        if (pid == -1)
                perror("fork: ");
        if (pid == 0)
        {
                check_for_redirections(av);
                execute_command(av[1], env);
        }
        waitpid(pid, NULL, 0);
}
