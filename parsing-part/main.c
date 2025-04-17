/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/17 11:11:12 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// paring the command
void	parsing_cmd(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	// handle the exit cmd
	if (input == NULL || !ft_strncmp(input, "exit", 4))
	{
		printf("exit\n");
		while (i != 99999999)
			i++;
		free(input);
		exit(0);
	}
	// split the cmd to tokens
	tokens = tokenize_input(input);
	while (tokens)
	{
		printf("TOKEN: [%s] Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

int	main(int ac, char **av)
{
	char	*input;

	(void)av;
	// check if we have any args
	if (ac != 1)
		custom_error("Error: run only the programme", "", 1);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		// paring the command
		parsing_cmd(input);
		free(input);
	}
}
