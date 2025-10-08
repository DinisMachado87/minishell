/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:29:54 by jlind             #+#    #+#             */
/*   Updated: 2025/10/08 11:20:57 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_cd(t_shell *shell, t_ast *node)
{
	char	*pwd;
	int		err_code;

	pwd = NULL;
	err_code = 0;
	if (node->args[0].n > 2)
		return (print_err("cd", "too many arguments", 1));
	else if ((node->args[0].n < 2) || (!*node->args[0].tkns[1]))
		return (0);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	if (ms_strncmp(node->args[0].tkns[1], "-", 1) == 0)
		err_code = chdir(get_env_node(shell->env, "OLDPWD")->value);
	else
		err_code = chdir(node->args[0].tkns[1]);
	if (err_code != 0)
	{
		free(pwd);
		return (print_sys_err("cd", node->args[0].tkns[1], strerror(errno), 1));
	}
	update_env_node(shell->env, "OLDPWD", pwd);
	pwd = getcwd(NULL, 0);
	update_env_node(shell->env, "PWD", pwd);
	return (0);
}
