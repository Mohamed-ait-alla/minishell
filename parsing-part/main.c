/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/03/20 17:16:27 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

int main(int ac, char **av)
{
    // check if we have any args
    if (ac > 1)
    {
        // get all commands
        char **cmds;
        int (i), (j), (k);
        
        cmds = malloc(sizeof(char *) * ac);
        if (!cmds)
            return (1);
        i = 1;
        j = 0;
        while (i < ac)
        {
            cmds[j] = malloc(sizeof(char) * (ft_strlen(av[i]) + 1));
            if (!cmds[j])
                return (1);
            k = 0;
            while (av[i][k])
            {
                cmds[j][k] = av[i][k];
                k++;
            }
            cmds[j][k] = '\0';
            j++;
            i++;
        }
        cmds[j] = NULL;
        for (int i = 0; cmds[i]; i++)
        {
            printf("%s\n", cmds[i]);
        }
    }
    return (0);
}
