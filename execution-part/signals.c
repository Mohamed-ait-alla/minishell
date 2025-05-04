/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:48:50 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/04 16:28:51 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	sig_handler_child(int signum)
{
	if (signum == SIGQUIT)
	{
		// printf("Quit (core dumped)\n");
		write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		exit (131);
	}
}

void	handle_parent_signals()
{
	signal(SIGQUIT, SIG_IGN); // when ctrt-\ is pressed in parent process does nothing
}

void	handle_child_signals()
{
	signal(SIGQUIT, sig_handler_child); // when ctrl-\ is pressed in a child process
}