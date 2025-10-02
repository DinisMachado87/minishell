/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:40:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/02 20:23:27 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	set_and_save_signal(int signal, void (*handler)(int),
		struct sigaction *old_signal)
{
	struct sigaction	sa_struct;

	sa_struct.sa_handler = handler;
	sigemptyset(&sa_struct.sa_mask);
	sa_struct.sa_flags = 0;
	sigaction(signal, &sa_struct, old_signal);
}

void	if_not_line(char *line, char *eof, int fd, char *temp_file)
{
	if (!line)
	{
		write(STDERR_FILENO, "Warning: heredoc delimited by eof: ", 36);
		write(STDERR_FILENO, eof, ms_strlen(eof));
		write(STDERR_FILENO, "\n", 1);
		close(fd);
		unlink(temp_file);
		free_and_null((void **)&temp_file);
		exit(1);
	}
}

int	create_heredoc_file(char *eof, char *temp_file)
{
	int		fd;
	char	*line;

	set_and_save_signal(SIGINT, &sigint_handler, NULL);
	set_and_save_signal(SIGQUIT, SIG_IGN, NULL);
	line = NULL;
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
		return (perror("ERROR: heredoc file :"), ERROR);
	while (1)
	{
		line = readline("> ");
		if_not_line(line, eof, fd, temp_file);
		if (!ms_strcmp(eof, line) && free_and_null((void **)&line))
			break ;
		else
		{
			write(fd, line, ms_strlen(line));
			write(fd, "\n", 1);
			free_and_null((void **)&line);
		}
	}
	close(fd);
	free_and_null((void **)&temp_file);
	exit(0);
}

int	wait_to_store_file(pid_t pid, char **temp_file, char **arg_tkn)
{
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;
	int					status;

	status = 0;
	set_and_save_signal(SIGINT, SIG_IGN, &old_sigint);
	set_and_save_signal(SIGQUIT, SIG_IGN, &old_sigquit);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		free(*temp_file);
		sigaction(SIGINT, &old_sigint, NULL);
		sigaction(SIGQUIT, &old_sigquit, NULL);
		return (0);
	}
	free_and_reassign(arg_tkn, temp_file);
	sigaction(SIGINT, &old_sigint, NULL);
	sigaction(SIGQUIT, &old_sigquit, NULL);
	return (1);
}
