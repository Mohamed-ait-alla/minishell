/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/18 11:30:55 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// paring the command
void	parsing_cmd(char *input)
{
	int			i;
	t_token		*tokens;
	t_commands	*cmd_list;
	t_token		*tmp;

	i = 0;
	// handle the exit cmd
	if ((input == NULL || !ft_strncmp(input, "exit", 4))
		&& (ft_strlen(input) == 4))
	{
		printf("exit\n");
		while (i != 99999999)
			i++;
		free(input);
		exit(0);
	}
	// split the cmd to tokens
	tokens = tokenize_input(input);
	// print tokens => value & type
	tmp = tokens;
	while (tmp)
	{
		printf("TOKEN: [%s] Type: %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
	// parse the tokens
	cmd_list = parse_tokens(tokens);
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
		if (ft_strlen(input) > 0)
			add_history(input);
		// paring the command
		parsing_cmd(input);
		free(input);
	}
}
