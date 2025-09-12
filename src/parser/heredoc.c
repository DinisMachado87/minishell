/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/02 13:21:59 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

char	*unique_tmp(char **dest, char *str1, char *str2)
{
	int		i;
	int		len;

	i = 0;
	len = ms_strlen(str1) + ms_strlen(str2);
	*dest = malloc(len + 1);
	if (!*dest)
		return (perror("ERROR: Failed concatenating heredoc filename"), NULL);
	ms_bzero((void *)*dest, len + 1);
	while (*str1)
	{
		(*dest)[i] = *str1;
		i++;
		str1++;
	}
	while (*str2)
	{
		(*dest)[i] = *str2;
		i++;
		str2++;
	}
	return (*dest);
}

void	sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	set_and_save_signal(int signal, void (*handler)(int), struct sigaction *old_signal)
{
	struct sigaction sa_struct;

	sa_struct.sa_handler = handler;
	sigemptyset(&sa_struct.sa_mask);
	sa_struct.sa_flags = 0;
	sigaction(signal, &sa_struct, old_signal);
}

int	create_heredoc_file(char *eof, char *temp_file)
{
	int			fd;
	char		*line;

	set_and_save_signal(SIGINT, &sigint_handler, NULL);
	set_and_save_signal(SIGQUIT, SIG_IGN, NULL);
	line = NULL;
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
		return (perror("ERROR: Failed to create heredoc file"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDERR_FILENO, "Warning: heredoc delimited by eof: ", 36);
			write(STDERR_FILENO, eof, ms_strlen(eof));
			break;
		}
		if (!ms_strcmp(eof, line)
			&& free_and_null((void **)&line))
			break;
		else
		{
			write(fd, line, ms_strlen(line));
			write(fd, "\n", 1);
			free_and_null((void **)&line);
		}
	}
	close(fd);
	exit(0);
}

int	count_args(char **arr)
{
	int	n_args;

	n_args = 0;
	while (arr[n_args])
		n_args++;
	return (n_args);
}

int	wait_to_store_file(pid_t pid, int status, char *temp_file, t_ast *ast)
{
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;

	set_and_save_signal(SIGINT, SIG_IGN, &old_sigint);
	set_and_save_signal(SIGQUIT, SIG_IGN, &old_sigquit);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		free(temp_file);
		return (ERROR);
	}
	free_red_args(ast, IN);
	ast->red_args[IN] = temp_file;
	sigaction(SIGINT, &old_sigint, NULL);
	sigaction(SIGQUIT, &old_sigquit, NULL);
	return (0);
}

int ms_heredoc(t_ast *ast, t_parser *s)
{
	int		n_args;
	int		status;
	pid_t	pid;
	char	*temp_file_name;
	
	n_args = count_args(ast->pre_r_args[IN]);
	status = 0;
	free_and_null((void **)ast->red_args[IN]);
	if (!cat_str_arr(&ast->red_args[IN], ast->pre_r_args[IN], n_args)
		|| !unique_tmp(&temp_file_name, TEMP_PREFIX, itoa(++s->n_heredoc)))
		return (ERROR);

	pid = fork();
	if ((pid == 0
			&& create_heredoc_file(ast->red_args[IN], temp_file_name) == ERROR)
		|| (pid > 0
			&& wait_to_store_file(pid, status, temp_file_name, ast) == ERROR)
		|| (pid < 0))
		return (ERROR);
	return (0);
}
