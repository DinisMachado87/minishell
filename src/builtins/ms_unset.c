/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:30:57 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 09:31:04 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_unset(t_shell *shell, t_ast *node)
{
	int	i;

	i = 1;
	if (!node->args[0].tkns[i])
		return (0);
	while (node->args[0].tkns[i])
	{
		free_env_node_by_key(&shell->env, node->args[0].tkns[i]);
		i++;
	}
	return (0);
}
