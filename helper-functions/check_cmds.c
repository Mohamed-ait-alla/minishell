/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:46:47 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/12 12:20:03 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_cmds(t_commands *cmds)
{
	t_commands	*tmp_cmds;

	tmp_cmds = cmds;
	while (tmp_cmds)
	{
		if (!tmp_cmds->args && !tmp_cmds->input_file && !tmp_cmds->output_file)
			return (0);
		tmp_cmds = tmp_cmds->next;
	}
	return (1);
}
