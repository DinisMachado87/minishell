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
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
	{
		free(temp_file);
		return (ERROR);
	}
	free_red_args(ast, IN);
	ast->red_args[IN] = temp_file;
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
	if (pid == 0
		&& create_heredoc_file(ast->red_args[IN], temp_file_name) == ERROR)
		return (ERROR);
	else if (pid > 0
		&& wait_to_store_file(pid, status, temp_file_name, ast) == ERROR)
		return (ERROR);
	else if (pid < 0)
		return (ERROR);
	return (0);
}
