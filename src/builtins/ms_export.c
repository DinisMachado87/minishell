/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:30:38 by jlind             #+#    #+#             */
/*   Updated: 2025/10/09 11:34:53 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_export(t_shell *shell, t_ast *node)
{
	char	*equ;
	char	*key;
	char	*value;

	if (node->args[0].n == 1)
		return (print_env(shell, 1), 0);
	equ = ms_strchr(node->args[0].tkns[1], '=');
	if (!equ && !ms_isalpha(node->args[0].tkns[1]))
		return (print_sys_err("export", node->args[0].tkns[1],
				"invalid identifier", 1));
	if (!equ)
		return (0);
	key = ms_strndup(node->args[0].tkns[1],
			(ms_strlen(node->args[0].tkns[1]) - ms_strlen(equ)));
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
		return (free(key), print_err("export", "invalid identifier", 1));
	value = ms_strndup((equ + 1), (ms_strlen(equ) - 1));
	if (!value)
		return (free(key), 1);
	set_env_node(&shell->env, key, value);
	free(key);
	free(value);
	return (0);
}
