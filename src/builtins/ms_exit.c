/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:31:33 by jlind             #+#    #+#             */
/*   Updated: 2025/10/08 11:56:07 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_exit(t_shell *shell, t_ast *node)
{
	int	exit_status;

	write(2, "exit\n", 5);
	if (node->args[0].n > 2)
		exit_status = print_err("exit", "too many arguments", 1);
	else if (node->args[0].tkns[1])
	{
		if (ms_isdigit(node->args[0].tkns[1]))
			exit_status = ms_atoi(node->args[0].tkns[1]);
		else
			exit_status = print_sys_err("exit", node->args[0].tkns[1],
					"numeric argument required", 2);
	}
	else
		exit_status = shell->exit_status;
	free_shell(shell);
	exit(exit_status);
}
