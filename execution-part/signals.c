/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:48:50 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/21 13:06:25 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_handler_parent(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	sig_handler_child(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_exit_status = 131;
	}
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_status = 130;
	}
}

void	handle_parent_signals(void)
{
	signal (SIGQUIT, SIG_IGN);
	signal (SIGINT, sig_handler_parent);
}

void	handle_child_signals(void)
{
	signal(SIGQUIT, sig_handler_child);
	signal(SIGINT, sig_handler_child);
}
