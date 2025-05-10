/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:48:50 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/10 19:52:07 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	sig_handler_parent(int signum)
{
	(void)	signum;
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
		// write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		printf("Quit (core dumped)\n");
		g_exit_status = 131;
	}
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_status = 130;
	}
}

void	sig_handler_here_doc(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		exit (130);
	}
}

void	handle_here_doc_signals()
{
	signal (SIGQUIT, SIG_IGN);
	signal (SIGINT, sig_handler_here_doc);
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