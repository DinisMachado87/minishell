/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:05 by jlind             #+#    #+#             */
/*   Updated: 2025/08/22 13:21:06 by jlind            ###   ########.fr       */
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
