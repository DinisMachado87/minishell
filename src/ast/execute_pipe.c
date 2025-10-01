/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:34:41 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 16:29:40 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_pipe_fork(t_shell *shell, t_ast *node, int fd[2], int out)
{
	int	fd_to_close;
	int	fd_to_keep;
	int	redirection;
	int	exit_status;

	fd_to_close = 0;
	fd_to_keep = 1;
	redirection = STDOUT_FILENO;
	if (out)
	{
		fd_to_close = 1;
		fd_to_keep = 0;
		redirection = STDIN_FILENO;
	}
	close(fd[fd_to_close]);
	dup2(fd[fd_to_keep], redirection);
	close(fd[fd_to_keep]);
	if (!out)
		execute_ast(shell, node->left);
	else
		execute_ast(shell, node->right);
	exit_status = shell->exit_status;
	free_shell(shell);
	return (exit_status);
}

void	execute_pipe(t_shell *shell, t_ast *node)
{
	int	left_pid;
	int	right_pid;
	int	fd[2];
	int	status;

	if (pipe(fd) < 0)
		shell->exit_status = ERROR;
	left_pid = fork();
	if (left_pid < 0)
		shell->exit_status = ERROR;
	if (left_pid == 0)
		exit(execute_pipe_fork(shell, node, fd, 0));
	right_pid = fork();
	if (right_pid < 0)
		shell->exit_status = ERROR;
	if (right_pid == 0)
		exit(execute_pipe_fork(shell, node, fd, 1));
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}
