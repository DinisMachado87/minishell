/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:31:14 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 09:31:22 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_env(t_shell *shell, t_ast *node)
{
	if (node->args[0].n > 1)
		return (ERROR);
	print_env(shell, 0);
	return (0);
}
