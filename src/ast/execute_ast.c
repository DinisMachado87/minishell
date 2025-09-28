/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:10:22 by jlind             #+#    #+#             */
/*   Updated: 2025/09/28 09:52:38 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_external(t_shell *shell, t_ast *node)
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
			free(cmd);
			free_shell(shell);
			exit(1);
		}
		free(cmd);
		free_shell(shell);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	set_handler(0);
}

int	execute_built_in(t_shell *shell, t_ast *node)
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
		ft_exit(shell, node);
	return (0);
}

int	setup_red_in(char *file, int type)
{
	struct stat	statbuf;
	int			fd;

	ms_bzero((void *)&statbuf, sizeof(struct stat));
	if (stat(file, &statbuf) == -1)
		return (print_sys_err("stat", file, strerror(errno), ERROR));
	if (!statbuf.st_mode)
		return (print_err(file, "No such file or directory"), ERROR);
	fd = open(file, O_RDONLY);
	if (fd == ERROR)
		return (print_sys_err("open", file, strerror(errno), ERROR));
	if (dup2(fd, STDIN_FILENO) == ERROR)
		return (print_sys_err("dup2", file, strerror(errno), ERROR));
	if (type == HEREDOC)
	{
		if (unlink(file) == ERROR)
			return (print_sys_err("unlink", file, strerror(errno), ERROR));
	}
	return (SUCCESS);
}

int	setup_red_out(char *dir, struct stat *statbuf, char *file, int type)
{
	int	fd;

	if ((stat(dir, statbuf) == ERROR) && (errno != ENOENT))
	{
		if (errno == ENOENT)
			return (print_err(file, "No such file or directory"), ERROR);
		else
			return (perror("minishell: stat"), ERROR);
	}
	if ((access(file, F_OK) == 0) && (access(file, W_OK) != 0))
		return (print_err(file, "Permission denied"), ERROR);
	if (type == APPEND)
		fd = open(file,	O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == ERROR )
		return (print_err(file, "Permission denied"), ERROR);
	if (dup2(fd, STDOUT_FILENO) == ERROR)
		return (perror("minishell: dup2"), ERROR);
	return (SUCCESS);
}

int	setup_red_out_wrapper(char *file, int type)
{
	char		*dir;
	char		*last_slash;
	struct stat	statbuf;
	int			exit_status;

	ms_bzero((void *)&statbuf, sizeof(struct stat));
	dir = ".";
	last_slash = ms_strrchr(file, '/');
	if (last_slash)
		dir = ms_strndup(file, (ms_strlen(file) - ms_strlen(last_slash)));
	exit_status = setup_red_out(dir, &statbuf, file, type);
	free(dir);
	return (exit_status);
}

void	execute_ast(t_shell *shell, t_ast *node)
{
	int			save_stdin;
	int			save_stdout;
	t_shell		subshell;
	int			err_code;
	int			arg_n;

	arg_n = 0;
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (!shell->ast || !node)
		shell->exit_status = ERROR;
	if (node->type == OPERATOR && node->subtype == AND)
		execute_and(shell, node);
	else if (node->type == OPERATOR && node->subtype == OR)
		execute_or(shell, node);
	else if (node->type == PIPE)
		execute_pipe(shell, node);
	else if (node->type == CMD)
	{
		cmd_expander(node->args, shell);
		if (!node->args[0].tkns)
			node->subtype = EXTERNAL;
		else
			node->subtype = strict_subtype(node->args[0].tkns[0]);
		while (node && 
				((node->args[IN].tkns && node->args[IN].tkns[arg_n]) || 
				(node->args[OUT].tkns && node->args[OUT].tkns[arg_n])))
		{
			if (node && node->args[IN].tkns && node->args[IN].tkns[arg_n])
				err_code = setup_red_in(node->args[IN].tkns[arg_n], node->args[IN].type[arg_n]);
			else if (node && node->args[OUT].tkns && node->args[OUT].tkns[arg_n])
				err_code = setup_red_out_wrapper(node->args[OUT].tkns[arg_n], node->args[OUT].type[arg_n]);
			if (err_code < 0)
			{
				shell->exit_status = err_code * -1;
				dup2(save_stdin, STDIN_FILENO);
				dup2(save_stdout, STDOUT_FILENO);
				return ;
			}
			arg_n++;
		}
		if (node->subtype == EXTERNAL && *node->args[0].tkns)
			execute_external(shell, node);
		else if (node->subtype != EXTERNAL && *node->args[0].tkns)
			shell->exit_status = execute_built_in(shell, node);
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
	}
	else if (node->type == SUBSHELL)
	{
		ms_bzero((void *)&subshell, sizeof(t_shell));
		init_env(&subshell, convert_env_to_list(shell->env));
		//[WARNING:] I noticed you are not checking for errors in a lot ofsystem calls,
		//and in the ones being checked I don't think you are exiting the function
		//which will lead to crash.
		//I sent you on slack a print screen
		if (parser(node->args[0].tkns[0], &subshell) < 0)
			return ;
		execute_ast(&subshell, subshell.ast);
		free_shell(&subshell);
	}
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

void	execute_pipe(t_shell *shell, t_ast *node)
{
	int	left_pid;
	int	right_pid;
	int	fd[2];
	int	lstatus;
	int	rstatus;
	int	exit_status;

	exit_status = 0;
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
		exit_status = shell->exit_status;
		free_shell(shell);
		exit(exit_status);
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
		exit_status = shell->exit_status;
		free_shell(shell);
		exit(exit_status);
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
