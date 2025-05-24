/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:34:29 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/22 17:02:16 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static t_commands	*init_new_command(t_commands **cmd_list)
{
	t_commands	*cmd;
	t_commands	*tmp;

	cmd = ft_malloc(sizeof(t_commands), 1);
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_commands));
	if (!(*cmd_list))
		(*cmd_list) = cmd;
	else
	{
		tmp = (*cmd_list);
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
	}
	return (cmd);
}

static void	handle_word_tokens(t_token *tokens, t_commands *current_cmd)
{
	int		i;
	char	**cmd_in_env_var;

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
		current_cmd->args = ft_realloc_array(current_cmd->args, tokens->value);
}

static void	add_redirections(t_commands *current_cmd, t_redirections *new_redir)
{
	t_redirections	*last;

	if (!current_cmd->redirections)
		current_cmd->redirections = new_redir;
	else
	{
		last = current_cmd->redirections;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

static int	handle_redirections(t_commands *current_cmd, t_token **tokens)
{
	t_redirections	*new_redir;

	new_redir = ft_malloc(sizeof(t_redirections), 1);
	if (!new_redir)
		return (0);
	ft_bzero(new_redir, sizeof(t_redirections));
	new_redir->type = (*tokens)->type;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type == TOKEN_REDIRECT_IN
		|| (*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_APPEND || (*tokens)->type == TOKEN_HEREDOC)
		return (0);
	new_redir->file = ft_strdup((*tokens)->value);
	new_redir->quote_type = (*tokens)->quote_type;
	add_redirections(current_cmd, new_redir);
	if (new_redir->type == TOKEN_APPEND)
		current_cmd->append = 1;
	else if (new_redir->type == TOKEN_HEREDOC)
		current_cmd->heredoc = 1;
	return (1);
}

t_commands	*parse_tokens(t_token *tokens)
{
	t_quote_type	quote_type;
	t_commands		*cmd_list;
	t_commands		*current_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	quote_type = NO_QUOTE;
	while (tokens)
	{
		if (!current_cmd || tokens->type == TOKEN_PIPE)
			current_cmd = init_new_command(&cmd_list);
		if (!current_cmd)
			return (NULL);
		if (tokens->type == TOKEN_WORD)
			handle_word_tokens(tokens, current_cmd);
		else if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
			if (!handle_redirections(current_cmd, &tokens))
				return (NULL);
		tokens = tokens->next;
	}
	return (cmd_list);
}
