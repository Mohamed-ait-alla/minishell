/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@stduent.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/03/20 19:55:16 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"             

char    *get_env_path(char **env)
{                                            
        char    *env_path;        
        int     i;                         
        
        i = 0;        
        while (env && env[i])
        {                            
                if (ft_strncmp("PATH=", env[i], 5) == 0)
                        return (env[i]);
                i++;                     
        }                        
        return (NULL);                                 
}  

char    *get_path_name(char *cmd, char **env)
{        
        char    *env_path;                   
        char    *holder;          
        char    *binary_path;              
        char    **exec_dirs;
        int     i;    

        env_path = get_env_path(env);
        exec_dirs = ft_split(env_path, ':');
        i = 0;
          
        while (exec_dirs && exec_dirs[i])
        {                        
                holder = ft_strjoin(exec_dirs[i], "/");
                binary_path = ft_strjoin(holder, cmd);
                free(holder);                      
                if (access(binary_path, F_OK) == 0 && access(binary_path, X_OK) == 0)                                   
                        return (binary_path);                             
                free(binary_path);                 
                i++;                        
        }                                
        return (NULL);                                                    
}

void    check_for_redirections(char *redirection_symbol, char *file)
{
        if (!redirection_symbol || !file)
                return ;
        if (ft_strcmp(redirection_symbol, ">") == 0) // redirect output to the specified file, overwrite it if it exist
                redirect_output_to_file(file, 'o');
        else if (ft_strcmp(redirection_symbol, ">>") == 0) // redirect output to the specified file, append to it if it exist 
                redirect_output_to_file(file, 'a');
        else if (ft_strcmp(redirection_symbol, "<") == 0) // redirect input to the specified file, so the command can read from it
                redirect_input_to_file(file);
        else if (ft_strcmp(redirection_symbol, "<<") == 0) // here_document
                redirect_input_to_file_here_doc(file); // just for testing file here is a limitter  
}

int     main(int ac, char **av, char **env)
{
        int     pid;
        char    *path_name;
        char    **args;

        pid = fork();
        if(pid == -1)
                perror("fork: ");
        if (pid == 0)
        {
                // child process that will execute the command
                check_for_redirections(av[1], av[2]); // check for redirections if any
                args = ft_split(av[3], ' ');
                path_name = get_path_name(args[0], env);
                if (!path_name)
		{
                        printf("no binary path found for this command\n");
                        exit(EXIT_FAILURE);
                }
                execve(path_name, args, env);
                perror("execve: ");
                exit(EXIT_FAILURE);
        }
        waitpid(pid, NULL, 0);
}
