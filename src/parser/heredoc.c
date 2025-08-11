/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/11 13:56:06 by dimachad         ###   ########.fr       */
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
	cat_str[i] = 0;
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
		if (strcmp(eof, line))
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

int ms_heredoc(char *eof, int heredoc_count, t_ast *ast)
{
	int			pid;
	const char	*temp_prefix = "temp_heredoc";
	char		*temp_file;

	temp_file = unique_tmp("temp_heredoc", itoa(heredoc_count));
	if (!temp_file)
		return (perror("ERROR: Failed concatenating heredoc filename"), ERROR);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
		create_heredoc_file(eof, temp_file);
	else
	{
		waitpid(pid, NULL, 0);
		if (ast->red_args[IN] && ms_strcmp((char *)temp_prefix, ast->red_args[IN]))
			unlink(ast->red_args[IN]);
		if (ast->red_args[IN])
			ast->red_args[IN] = temp_file;
	}
	return (1);
}
