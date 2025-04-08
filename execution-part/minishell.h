/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:22 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/08 11:57:10 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"

// char    *get_path_name(char *cmd, char **env);
void	redirect_output_to_file(char *file, char mode);
void	redirect_output_to_pipe(int write_pipe_end);
void	redirect_input_to_file(char *file);
void	redirect_input_to_file_here_doc(char *limitter);
void	redirect_input_to_pipe(int read_pipe_end);
void	check_for_redirections(char **av);
bool	check_for_pipes(char **av);
void    handle_pipes(char **av, char **env);
void	execute_command(char *cmd, char **env);

#endif
