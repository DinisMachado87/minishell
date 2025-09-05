/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:19:50 by jlind             #+#    #+#             */
/*   Updated: 2025/09/05 15:23:03 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*split_at_sep(char **str, char sep)
{
	int		len;
	char	*start;
	char	*dir;

	start = *str;
	len = 0;
	while (*(*str))
	{
		if (*(*str) == sep)
		{
			(*str)++;
			break ;
		}
		len++;
		(*str)++;
	}
	dir = malloc(sizeof(char) * (len + 1));
	if (!dir)
		return (NULL);
	ms_strncpy(dir, start, len);
	return (dir);
}

static char	*append_cmd(char *dir, char *cmd)
{
	char	*path;
	int		total_len;

	total_len = ms_strlen(dir) + ms_strlen(cmd) + 1;
	path = malloc(sizeof(char) * (total_len + 1));
	ms_bzero((void *)path, (total_len + 1));
	if (!path)
		return (NULL);
	ms_strncpy(path, dir, ms_strlen(dir));
	ms_strncat(path, "/", 1);
	ms_strncat(path, cmd, ms_strlen(cmd));
	return (path);
}

static int	cmd_valid(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0)
		return (1);
	return (0);
}

char	*get_cmd_path(char *cmd, char *env)
{
	char	sep;
	char	*dir;
	char	*path;

	sep = ':';
	if (cmd_valid(cmd))
		return (cmd);
	while (*env)
	{
		dir = split_at_sep(&env, sep);
		if (!dir)
			return (NULL);
		path = append_cmd(dir, cmd);
		if (!path)
			return (NULL);
		free(dir);
		if (cmd_valid(path))
			return (path);
		free(path);
	}
	return (NULL);
}
