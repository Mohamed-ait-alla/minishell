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

char    *get_path_name(char **av, char **env)
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
                binary_path = ft_strjoin(holder, av[1]);      
                free(holder);                      
                if (access(binary_path, F_OK) == 0 && access(binary_path, X_OK) == 0)                                   
                        return (binary_path);                             
                free(binary_path);                 
                i++;                        
        }                                
        return (NULL);                                                    
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
                path_name = get_path_name(av, env);
                args = ft_split(av[1], ' ');
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

