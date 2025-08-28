/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/27 19:54:18 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	create_heredoc_file(char *eof, char *temp_file)
{
	int			fd;
	char		*line;

	line = NULL;
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
		return (perror("ERROR: Failed to create heredoc file"), ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (perror("ERROR: readline error in heredoc"), ERROR);
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

int ms_heredoc(t_ast *ast, t_s_parser *s)
{
	int		pid;
	char	*temp_file_name;
	int		status;
	
	status = 0;
	free_and_null((void **)ast->red_args[IN]);
	if (!cat_str_arr(&ast->red_args[IN], &ast->pre_red_args[IN])
		|| !unique_tmp(&temp_file_name, TEMP_PREFIX, itoa(++s->n_heredoc)))
		return (ERROR);
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
