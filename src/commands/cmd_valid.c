/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_valid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:12:52 by jlind             #+#    #+#             */
/*   Updated: 2025/10/09 11:34:00 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*split_at_sep(char **path, char sep)
{
	char	*match;
	char	*dir;

	match = ms_strchr(*path, sep);
	dir = ms_strndup(*path, (ms_strlen(*path) - ms_strlen(match)));
	if (match)
		*path = match + 1;
	else
		*path = NULL;
	return (dir);
}

char	*append_cmd(char *dir, char *cmd)
{
	char	*path;
	int		total_len;

	total_len = ms_strlen(dir) + ms_strlen(cmd) + 1;
	path = malloc(sizeof(char) * (total_len + 1));
	if (!path)
		return (NULL);
	ms_bzero((void *)path, (total_len + 1));
	ms_strncpy(path, dir, ms_strlen(dir));
	ms_strncat(path, "/", 1);
	ms_strncat(path, cmd, ms_strlen(cmd));
	return (path);
}

void	cmd_err(t_shell *shell, char *cmd, char *err_msg, int err_code)
{
	shell->exit_status = print_err(cmd, err_msg, err_code);
}

char	*local_cmd_valid(t_shell *shell, char *cmd)
{
	struct stat	statbuf;
	int			x_perm;

	ms_bzero(&statbuf, sizeof(struct stat));
	stat(cmd, &statbuf);
	if (!statbuf.st_mode)
		return (cmd_err(shell, cmd, "No such file or directory", 127), NULL);
	if (S_ISDIR(statbuf.st_mode))
		return (cmd_err(shell, cmd, "Is a directory", 126), NULL);
	x_perm = access(cmd, X_OK);
	if (x_perm)
		return (cmd_err(shell, cmd, "Permission denied", 126), NULL);
	cmd = ms_strndup(cmd, ms_strlen(cmd));
	return (cmd);
}

char	*global_cmd_valid(t_shell *shell, char *cmd)
{
	char	*path;
	char	*cmd_path;
	char	*dir;

	if (get_env_node(shell->env, "PATH"))
		path = get_env_node(shell->env, "PATH")->value;
	while (path && *path)
	{
		shell->exit_status = 0;
		dir = split_at_sep(&path, ':');
		cmd_path = append_cmd(dir, cmd);
		free(dir);
		if (access(cmd_path, X_OK) != 0)
			shell->exit_status = 126;
		if (access(cmd_path, F_OK) != 0)
			shell->exit_status = 127;
		if (shell->exit_status == 0)
			return (cmd_path);
		free(cmd_path);
	}
	if (shell->exit_status == 126)
		cmd_err(shell, cmd, "Permission denied", 126);
	else if (shell->exit_status == 127)
		cmd_err(shell, cmd, "command not found", 127);
	return (NULL);
}
