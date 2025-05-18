/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:35:09 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/16 20:12:31 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static t_env	*create_env_node(char *env)
{
	t_env	*node;
	char	*equal;
	int		end;

	node = ft_malloc(sizeof(t_env), 1);
	if (!node)
		return (NULL);
	equal = ft_strchr(env, '=');
	if (equal)
	{
		end = 0;
		while (env[end] && env[end] != '=')
			end++;
		node->key = ft_substr(env, 0, end);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(env);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **env)
{
	t_env	*env_list;
	t_env	*new;
	t_env	*tmp;
	int		i;

	env_list = NULL;
	i = 0;
	while (env && env[i])
	{
		new = create_env_node(env[i]);
		if (!new)
			return (NULL);
		if (!env_list)
			env_list = new;
		else
		{
			tmp = env_list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		i++;
	}
	return (env_list);
}
