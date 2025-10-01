/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:18:49 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 16:01:27 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_external_fork(t_shell *shell, t_ast *node)
{
	char	*cmd;
	char	**list;
	int		status;

	status = 0;
	signal(SIGINT, SIG_DFL);
	cmd = get_cmd_path(shell, *node->args[0].tkns);
	if (!cmd)
	{
		free_shell(shell);
		exit(shell->exit_status);
	}
	list = convert_env_to_list(shell->env);
	if (execve(cmd, node->args[0].tkns, list) == -1)
	{
		perror("execve");
		status = 1;
	}
	free(cmd);
	free_shell(shell);
	free(list);
	return (status);
}

void	execute_external(t_shell *shell, t_ast *node)
{
	int		pid;
	int		status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		exit(execute_external_fork(shell, node));
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	set_handler(0);
}
