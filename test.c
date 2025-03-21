/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:04:47 by mdahani           #+#    #+#             */
/*   Updated: 2025/03/20 18:12:46 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main(int ac, char **av)
{
    char *input;
    while (1)
    {
        input = readline("minishell>");
        if (!input)
            break ;
        printf("%s\n", input);
        free(input);
    }
    
}
