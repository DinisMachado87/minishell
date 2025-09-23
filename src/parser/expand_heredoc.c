/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:45:30 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/24 01:36:49 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *get_n_env(char **expanded_value, t_env *head, char *key_str, int n)
{
	while (head)
	{
		if (ms_strncmp(head->key, key_str, n) == 0)
		{
			*expanded_value = head->value;
			return (*expanded_value);
		}
		head = head->next;
	}
	return (NULL);
}

int	expand_here_tkn(char *b, char **value, t_shell *sh)
{
	int	i;

	i = 0;
	if (b[i] == '?')
	{
		i++;
		if (!expand_exit_status(value, sh))
			return (ERROR);
	}
	else
	{
		while (b[i] && is_alphanum_or_underscore(b[i]))
			i++;
		get_n_env(value, sh->env, b, i);
	}
	return (i);
}

int expand_and_write(char **b, int new_fd, t_shell *sh)
{
	int	i;
	char	*value;

	i = 0;
	while ((*b)[i])
	{
		if ((*b)[i] == '$' && (*b)[i + 1] && (*b)[i + 1] != ' '
			&& (*b)[i + 1] != '\'' && (*b)[i + 1] != '\"')
		{
			write(new_fd, *b, i);
			*b += i + 1;
			value = NULL;
			i = expand_here_tkn(*b, &value, sh);
			if (i == ERROR)
				return (0);
			if (i == 0)
				continue;
			if (value)
				write(new_fd, value, ms_strlen(value));
			if ((*b)[0] == '?' && value)
				free(value);
			*b += i;
			i = 0;
		}
		else
			i++;
	}
	write(new_fd, *b, i);
	*b += i;
	return (1);
}

int	open_fds(int *old_fd, int *new_fd, char *filename, char *exp_file)
{
	*old_fd = open(filename, O_RDONLY);
	if (*old_fd < 0)
		return (perror("Error: cannot open heredoc file"), 0);
	*new_fd = open(exp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*new_fd < 0)
		return (close(*old_fd),
			perror("Error: cannot open new heredoc file"), 0);
	return (1);
}

int	expand_heredoc(char **filename, t_shell *sh)
{
	char	buf[1000 + 1] = "";
	int		read_char;
	int		fd[2];
	char	*file[2];
	int		err;
	char	*b;

	read_char = 1;
	file[OLD] = *filename;
	err = 0;
	b = buf;
	if (!unique_tmp(&file[NEW], *filename, "_exp")
		|| !open_fds(&fd[OLD], &fd[NEW], *filename, file[NEW]))
		return (0);
	while (read_char)
	{
		read_char = read(fd[OLD], b, 1000);
		if (read_char > 0)
			b[read_char] = '\0';
		if (read_char < 0
			|| !expand_and_write(&b, fd[NEW], sh))
			err = 1;
	}
	close(fd[NEW]);
	close(fd[OLD]);
	unlink(file[OLD]);
	free(*filename);
	if (err)
		return (perror("Error: expand heredoc:"), 0);
	*filename = file[NEW];
	return (1);
}
