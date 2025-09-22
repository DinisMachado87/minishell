/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:05 by jlind             #+#    #+#             */
/*   Updated: 2025/09/22 09:55:44 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_err(char *cmd_name, char *err_msg)
{
	while (*cmd_name)
	{
		write(2, cmd_name, 1);
		cmd_name++;
	}
	write(2, ": ", 2);
	while (*err_msg)
	{
		write(2, err_msg, 1);
		err_msg++;
	}
	write(2, "\n", 1);
}

int	print_sys_err(char *syscall, char *component, char *err_msg, int err)
{
	while (*syscall)
		write(2, syscall++, 1);
	write(2, ": ", 2);
	while (*component)
		write(2, component++, 1);
	write(2, ": ", 2);
	while (*err_msg)
		write(2, err_msg++, 1);
	write(2, "\n", 1);
	return (err);
}
