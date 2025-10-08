/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:29:33 by jlind             #+#    #+#             */
/*   Updated: 2025/10/08 10:42:21 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_echo(t_ast *node)
{
	int	i;
	int	j;
	int	newline;

	if (!node->args[0].tkns[1])
		return (write(1, "\n", 1), 0);
	newline = 1;
	i = 1;
	if (!ms_strncmp(node->args[0].tkns[1], "-n", 2))
	{
		newline = 0;
		i = 2;
	}
	while (node->args[0].tkns[i])
	{
		j = 0;
		while (node->args[0].tkns[i][j])
			write(1, &node->args[0].tkns[i][j++], 1);
		i++;
		if (node->args[0].tkns[i])
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
