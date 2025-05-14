/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:34:29 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/14 16:33:10 by mait-all         ###   ########.fr       */
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
	t_redirections *new_redir;
	t_redirections	*last;

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
		else if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
		{
			new_redir = ft_malloc(sizeof(t_redirections), 1);
			if (!new_redir)
				return (NULL);
			ft_bzero(new_redir, sizeof(t_redirections));
			// move to the next token
			new_redir->type = tokens->type;
			tokens = tokens->next;
			if (!tokens)
				return (NULL);
			new_redir->file = ft_strdup(tokens->value);
			if (!current_cmd->redirections)
				current_cmd->redirections = new_redir;
			else
			{
				last = current_cmd->redirections;
				while (last->next)
					last = last->next;
				last->next = new_redir;
			}
			if (new_redir->type == TOKEN_APPEND)
				current_cmd->append = 1;
			else if (new_redir->type == TOKEN_HEREDOC)
				current_cmd->heredoc = 1;
		}
		// else if (tokens->type == TOKEN_REDIRECT_OUT)
		// {
		// 	tokens = tokens->next;
		// 	if (!tokens)
		// 		return (NULL);
		// 	current_cmd->redirections->file = ft_strdup(tokens->value);
		// 	current_cmd->redirections->type = tokens->type;
		// 	// apped = 0 because when we use redirect out (>) we using over write
		// 	current_cmd->append = 0;
		// }
		// else if (tokens->type == TOKEN_APPEND)
		// {
		// 	tokens = tokens->next;
		// 	if (!tokens)
		// 		return (NULL);
		// 	current_cmd->redirections->file = ft_strdup(tokens->value);
		// 	current_cmd->redirections->type = tokens->type;
		// 	// apped = 1 because when we use append (>>) we using append
		// 	current_cmd->append = 1;
		// }
		// // condition of heredoc
		// else if (tokens->type == TOKEN_HEREDOC)
		// {
		// 	tokens = tokens->next;
		// 	if (!tokens)
		// 		return (NULL);
		// 	current_cmd->redirections->file = ft_strdup(tokens->value);
		// 	current_cmd->redirections->type = tokens->type;
		// 	// heredoc = 1 because when we use heredoc (<<) we using heredoc
		// 	current_cmd->heredoc = 1;
		// }
		current_cmd->quote_type = tokens->quote_type;
		tokens = tokens->next;
	}
	return (cmd_list);
}
