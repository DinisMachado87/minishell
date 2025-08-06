#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

char	*split_at_sep(char **str, char sep)
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
	strncpy(dir, start, len);
	dir[len] = '\0';
	return (dir);
}

char	*append_cmd(char *dir, char *cmd)
{
	char	*path;
	int		total_len;

	total_len = strlen(dir) + strlen(cmd);
	path = malloc(sizeof(char) * (total_len + 2));
	if (!path)
		return (NULL);
	strncpy(path, dir, strlen(dir));
	strncat(path, "/", 1);
	strncat(path, cmd, strlen(cmd));
	path[total_len + 1] = '\0';
	return (path);
}

int	cmd_valid(char *path)
{
	struct stat statbuf;

	if (stat(path, &statbuf) == 0)
		return (1);
	return (0);
}

char	*get_cmd_path(char *cmd, char *env)
{
	char	sep = ':';
	char	*dir;
	char	*path;

	while (*env)
	{
		dir = split_at_sep(&env, sep);
		if (!dir)
			return (NULL);
		path = append_cmd(dir, cmd);
		free(dir);
		if (cmd_valid(path))
			return (path);
		free(path);
	}
	return (NULL);
}
