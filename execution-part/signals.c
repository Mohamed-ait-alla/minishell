/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:48:50 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/05 13:43:04 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	sig_handler_parent(int signum)
{
	(void)	signum;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_handler_child(int signum)
{
	if (signum == SIGQUIT)
	{
		// printf("Quit (core dumped)\n");
		write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		exit (131);
	}
	if (signum == SIGINT)
	{
		printf("\n");
		exit (130);
	}
}

void	handle_parent_signals()
{
	signal (SIGQUIT, SIG_IGN); // when ctrl-\ is pressed in parent process
	signal (SIGINT, sig_handler_parent); // when ctrl-c is pressed in parent process 
}

void	handle_child_signals()
{
	signal(SIGQUIT, sig_handler_child); // when ctrl-\ is pressed in a child process
	signal(SIGINT, sig_handler_child); // when ctrl-c is pressed in a child process
}