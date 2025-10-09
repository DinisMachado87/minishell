/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:40:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/09 22:24:39 by dimachad         ###   ########.fr       */
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

void	free_heredoc(char *temp_file, t_cmd *c)
{
	if (!c->sh->list)
		c->sh->list = c->cur;
	free_and_null((void **)c->sh->input);
	free_ast_in_heredoc_fork(c->sh);
	free_and_null((void **)&temp_file);
}

void	if_not_line(char *line, int fd, char *temp_file, t_cmd *c)
{
	if (!line)
	{
		close(fd);
		unlink(temp_file);
		free_heredoc(temp_file, c);
		exit(130);
	}
}

int	create_heredoc_file(char *eof, char *temp_file, t_cmd *c)
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
		if_not_line(line, fd, temp_file, c);
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
	free_heredoc(temp_file, c);
	exit(0);
}
