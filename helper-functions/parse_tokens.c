/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:34:29 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/11 11:47:51 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

t_commands	*parse_tokens(t_token *tokens)
{
	t_commands		*cmd_list;
	t_commands		*current_cmd;
	t_quote_type	quote_type;
	t_commands		*tmp;
	char			**cmd_in_env_var;
	int				i;

	cmd_list = NULL;
	current_cmd = NULL;
	quote_type = NO_QUOTE;
	while (tokens)
	{
		// if we found a new command or | pipe
		if (!current_cmd || tokens->type == TOKEN_PIPE)
		{
			current_cmd = ft_malloc(sizeof(t_commands), 1);
			if (!current_cmd)
				return (NULL);
			ft_bzero(current_cmd, sizeof(t_commands));
			// add the new element to list
			if (!cmd_list)
				cmd_list = current_cmd;
			else
			{
				tmp = cmd_list;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = current_cmd;
			}
		}
		// add token to args (struct commands)
		if (tokens->type == TOKEN_WORD)
		{
			// this condition handle if have a command in var
			if (tokens->quote_type == NO_QUOTE)
			{
				cmd_in_env_var = ft_split(tokens->value, ' ');
				i = 0;
				while (cmd_in_env_var && cmd_in_env_var[i])
				{
					current_cmd->args = ft_realloc_array(current_cmd->args,
							cmd_in_env_var[i]);
					i++;
				}
			}
			else
				current_cmd->args = ft_realloc_array(current_cmd->args,
						tokens->value);
		}
		else if (tokens->type == TOKEN_REDIRECT_IN)
		{
			// move to the next token
			tokens = tokens->next;
			if (!tokens)
				return (NULL);
			current_cmd->input_file = ft_realloc_array(current_cmd->input_file,
					tokens->value);
		}
		else if (tokens->type == TOKEN_REDIRECT_OUT)
		{
			tokens = tokens->next;
			if (!tokens)
				return (NULL);
			current_cmd->output_file = ft_realloc_array(current_cmd->output_file,
					tokens->value);
			// apped = 0 because when we use redirect out (>) we using over write
			current_cmd->append = 0;
		}
		else if (tokens->type == TOKEN_APPEND)
		{
			tokens = tokens->next;
			if (!tokens)
				return (NULL);
			current_cmd->output_file = ft_realloc_array(current_cmd->output_file,
					tokens->value);
			// apped = 1 because when we use append (>>) we using append
			current_cmd->append = 1;
		}
		// condition of heredoc
		else if (tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (!tokens)
				return (NULL);
			current_cmd->input_file = ft_realloc_array(current_cmd->input_file,
					tokens->value);
			// heredoc = 1 because when we use heredoc (<<) we using heredoc
			current_cmd->heredoc = 1;
		}
		current_cmd->quote_type = tokens->quote_type;
		tokens = tokens->next;
	}
	return (cmd_list);
}
