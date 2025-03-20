/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@stduent.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:22 by mait-all          #+#    #+#             */
/*   Updated: 2025/03/20 18:08:45 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"

char    *get_path_name(char *cmd, char **env);
void    redirect_output_to_file(char *file, char mode);
void    redirect_input_to_file(char *file);
void    redirect_input_to_file_here_doc(char *limitter);

#endif
