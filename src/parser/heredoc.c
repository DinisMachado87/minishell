/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/13 02:06:52 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>

char	*unique_tmp(char *str1, char *str2)
{
	char	*cat_str;
	int		i;
	int		len;

	i = 0;
	len = ms_strlen(str1) + ms_strlen(str2);
	cat_str = malloc(len + 1);
	if (!cat_str)
		return (0);
	ms_bzero((void *)cat_str, len + 1);
	while (*str1)
	{
		cat_str[i] = *str1;
		i++;
		str1++;
	}
	while (*str2)
	{
		cat_str[i] = *str2;
		i++;
		str2++;
	}
	return (cat_str);
}

int	create_heredoc_file(char *eof, char *temp_file)
{
	int			fd;
	char		*line;

	fd = 0;
	line = NULL;
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
		return (perror("ERROR: Failed to create heredoc file"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (perror("ERROR: readline error in heredoc"), ERROR);
		if (ms_strcmp(eof, line))
		{
			free(line);
			line = NULL;
			break;
		}
		else
		{
			write(fd, line, ms_strlen(line));
			write(fd, "\n", 1);
		}
	}
	close(fd);
	exit(0);
}

int ms_heredoc(t_ast *ast, t_s_parser *s)
{
	int		pid;
	char	*temp_file_name;
	int		status;
	
	status = 0;
	s->n_heredoc++;
	temp_file_name = unique_tmp(TEMP_PREFIX, itoa(s->n_heredoc));
	if (!temp_file_name)
		return (perror("ERROR: Failed concatenating heredoc filename"), ERROR);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
		create_heredoc_file(ast->red_args[IN], temp_file_name);
	else
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0)
		{
			free(temp_file_name);
			return (ERROR);
		}
		free_red_args(ast, IN);
		ast->red_args[IN] = temp_file_name;
	}
	return (1);
}
