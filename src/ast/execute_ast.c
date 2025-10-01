/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:10:22 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 10:42:12 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_built_in(t_shell *shell, t_ast *node)
{
	if (node->subtype == ECHO)
		return (ms_echo(node));
	else if (node->subtype == CD)
		return (ms_cd(shell, node));
	else if (node->subtype == PWD)
		return (ms_pwd());
	else if (node->subtype == EXPORT)
		return (ms_export(shell, node));
	else if (node->subtype == UNSET)
		return (ms_unset(shell, node));
	else if (node->subtype == ENV)
		return (ms_env(shell, node));
	else if (node->subtype == EXIT)
		ms_exit(shell, node);
	return (0);
}

void	execute_subshell(t_shell *shell, t_ast *node)
{
	t_shell		subshell;

	ms_bzero((void *)&subshell, sizeof(t_shell));
	init_env(&subshell, convert_env_to_list(shell->env));
	if (parser(node->args[0].tkns[0], &subshell) < 0)
		return ;
	execute_ast(&subshell, subshell.ast);
	free_shell(&subshell);
}

void	execute_ast(t_shell *shell, t_ast *node)
{
	if (!shell->ast || !node)
		shell->exit_status = ERROR;
	if (node->type == OPERATOR && node->subtype == AND)
		execute_and(shell, node);
	else if (node->type == OPERATOR && node->subtype == OR)
		execute_or(shell, node);
	else if (node->type == PIPE)
		execute_pipe(shell, node);
	else if (node->type == CMD)
		execute_cmd(shell, node);
	else if (node->type == SUBSHELL)
		execute_subshell(shell, node);
}

void	execute_and(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status == 0)
		execute_ast(shell, node->right);
}

void	execute_or(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status != 0)
		execute_ast(shell, node->right);
}
