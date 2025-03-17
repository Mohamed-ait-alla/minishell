/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@stduent.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:32:45 by mait-all          #+#    #+#             */
/*   Updated: 2025/03/17 19:37:15 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    redirect_output_to_file(char *file, char mode)
{
    int fd;

    if (mode == 'o') // overwriting mode
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == 'a') // appending mode
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("permission denied: ");
        exit (EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close (fd);
}