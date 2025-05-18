/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:09:01 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/18 12:19:43 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_handler_here_doc(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_status = 130;
		exit (g_exit_status);
	}
}

void	handle_here_doc_signals(void)
{
	signal (SIGQUIT, SIG_IGN);
	signal (SIGINT, sig_handler_here_doc);
}
