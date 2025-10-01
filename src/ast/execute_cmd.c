/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:35:13 by jlind             #+#    #+#             */
/*   Updated: 2025/10/01 16:24:52 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	setup_red_in(char *file, int type)
{
	struct stat	statbuf;
	int			fd;

	ms_bzero((void *)&statbuf, sizeof(struct stat));
	if (stat(file, &statbuf) == -1)
		return (print_sys_err("stat", file, strerror(errno), ERROR));
	if (!statbuf.st_mode)
		return (print_err(file, "No such file or directory", ERROR));
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
			return (print_err(file, "No such file or directory", ERROR));
		else
			return (perror("minishell: stat"), ERROR);
	}
	if ((access(file, F_OK) == 0) && (access(file, W_OK) != 0))
		return (print_err(file, "Permission denied", ERROR));
	if (type == APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == ERROR)
		return (print_err(file, "Permission denied", ERROR));
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
	if (last_slash)
		free(dir);
	return (exit_status);
}

int	setup_redirects(t_shell *shell, t_ast *node)
{
	int	arg_n;
	int	err_code;

	arg_n = 0;
	err_code = 0;
	while (node
		&& ((node->args[IN].tkns && node->args[IN].tkns[arg_n])
			|| (node->args[OUT].tkns && node->args[OUT].tkns[arg_n])))
	{
		if (node && node->args[IN].tkns && node->args[IN].tkns[arg_n])
			err_code = setup_red_in(node->args[IN].tkns[arg_n],
					node->args[IN].type[arg_n]);
		if (err_code < 0)
			break ;
		if (node && node->args[OUT].tkns && node->args[OUT].tkns[arg_n])
			err_code = setup_red_out_wrapper(node->args[OUT].tkns[arg_n],
					node->args[OUT].type[arg_n]);
		if (err_code < 0)
			break ;
		arg_n++;
	}
	if (err_code < 0)
		shell->exit_status = err_code * -1;
	return (err_code);
}

void	execute_cmd(t_shell *shell, t_ast *node)
{
	int	save_stdin;
	int	save_stdout;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	cmd_expander(node->args, shell);
	if (!node->args[0].tkns)
		node->subtype = EXTERNAL;
	else
		node->subtype = strict_subtype(node->args[0].tkns[0]);
	if (setup_redirects(shell, node) < 0)
	{
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
		return ;
	}
	if (node->subtype == EXTERNAL && *node->args[0].tkns)
		execute_external(shell, node);
	else if (node->subtype != EXTERNAL && *node->args[0].tkns)
		shell->exit_status = execute_built_in(shell, node);
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
}
