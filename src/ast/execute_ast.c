/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:10:22 by jlind             #+#    #+#             */
/*   Updated: 2025/09/05 15:17:10 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_external(t_shell *shell, t_ast *node)
{
	char	*cmd;
	char	**list;
	int		pid;
	int		status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		cmd = get_cmd_path(node->args[0],
				get_env_node(shell->env, "PATH")->value);
		if (!cmd)
		{
			print_err(node->args[0], "command not found");
			exit(127);
		}
		list = convert_env_to_list(shell->env);
		if (execve(cmd, node->args, list) == -1)
		{
			perror("execve");
			free_env_list(list);
			_exit(1);
		}
		free_env_list(list);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	set_handler(0);
}

static int	execute_built_in(t_shell *shell, t_ast *node)
{
	if (node->subtype == ECHO)
		return (ft_echo(node));
	else if (node->subtype == CD)
		return (ft_cd(shell, node));
	else if (node->subtype == PWD)
		return (ft_pwd());
	else if (node->subtype == EXPORT)
		return (ft_export(shell, node));
	else if (node->subtype == UNSET)
		return (ft_unset(shell, node));
	else if (node->subtype == ENV)
		return (ft_env(shell, node));
	else if (node->subtype == EXIT)
		ft_exit(shell);
	return (0);
}

static void	execute_and(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status == 0)
		execute_ast(shell, node->right);
}

static void	execute_or(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status != 0)
		execute_ast(shell, node->right);
}

static void	execute_pipe(t_shell *shell, t_ast *node)
{
	int			left_pid;
	int			right_pid;
	int			fd[2];
	int			lstatus;
	int			rstatus;

	if (pipe(fd) < 0)
		shell->exit_status = ERROR;
	left_pid = fork();
	if (left_pid < 0)
		shell->exit_status = ERROR;
	if (left_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute_ast(shell, node->left);
		exit(shell->exit_status);
	}
	right_pid = fork();
	if (right_pid < 0)
		shell->exit_status = ERROR;
	if (right_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execute_ast(shell, node->right);
		exit(shell->exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &lstatus, 0);
	if (WIFEXITED(lstatus))
		shell->exit_status = WEXITSTATUS(lstatus);
	waitpid(right_pid, &rstatus, 0);
	if (WIFEXITED(rstatus))
		shell->exit_status = WEXITSTATUS(rstatus);
}

void	execute_ast(t_shell *shell, t_ast *node)
{
	int		fd;
	int		save_stdin;
	int		save_stdout;
	t_shell	subshell;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (!shell->ast_tree || !node)
		shell->exit_status = ERROR;
	if (node->type == OPERATOR && node->subtype == AND)
		execute_and(shell, node);
	else if (node->type == OPERATOR && node->subtype == OR)
		execute_or(shell, node);
	else if (node->type == PIPE)
		execute_pipe(shell, node);
	else if (node->type == CMD)
	{
		cmd_expander(node, shell->env);
		if (node->red_args[IN] || node->red_args[OUT])
		{
			if (node->red_args[IN])
			{
				fd = open(node->red_args[IN], O_RDONLY);
				dup2(fd, STDIN_FILENO);
			}
			else
			{
				if (node->append)
					fd = open(node->red_args[OUT],
							O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd = open(node->red_args[OUT],
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(fd, STDOUT_FILENO);
			}
			if (fd == ERROR)
			{
				perror(node->red_args[OUT]);
				dup2(save_stdin, STDIN_FILENO);
				dup2(save_stdout, STDOUT_FILENO);
				shell->exit_status = ERROR;
			}
			close(fd);
		}
		if (node->subtype == EXTERNAL)
			execute_external(shell, node);
		else
			shell->exit_status = execute_built_in(shell, node);
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
	}
	else if (node->type == SUBSHELL)
	{
		ms_bzero((void *)&subshell, sizeof(t_shell));
		subshell.env = init_env();
		subshell.ast_tree = parser(node->args[0], &subshell.ast_head);
		execute_ast(&subshell, subshell.ast_tree);
		free_ast(&subshell.ast_head);
	}
}
