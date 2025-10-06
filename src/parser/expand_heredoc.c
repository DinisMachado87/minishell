/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:45:30 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/06 23:50:09 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_n_env(char **expanded_value, t_env *head, char *key_str, int n)
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

int	expand_and_write(char **b, int new_fd, t_shell *sh, int i)
{
	char	*value;

	while ((*b)[i])
	{
		if ((*b)[i] == '$' && (*b)[i + 1] && (*b)[i + 1] != ' '
			&& (*b)[i + 1] != '\'' && (*b)[i + 1] != '\"')
		{
			write(new_fd, *b, i);
			*b += i + 1;
			value = NULL;
			i = expand_here_tkn(*b, &value, sh);
			if (i <= 0)
				return (ERROR);
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
	return (i);
}

int	expand_into_new_file(int *fd, char *b, t_shell *sh)
{
	int		read_char;
	int		i;

	read_char = 1;
	i = 0;
	while (read_char)
	{
		read_char = read(fd[OLD], b, 1000);
		if (read_char <= 0)
			return (read_char);
		b[read_char] = '\0';
		i = expand_and_write(&b, fd[NEW], sh, i);
		if (i == ERROR)
			return (1);
		write(fd[NEW], b, i);
	}
	return (0);
}

int	expand_heredoc(char **filename, t_shell *sh)
{
	char	buf[1000 + 1];
	int		fd[2];
	char	*file[2];
	int		err;

	buf[0] = '\0';
	file[OLD] = *filename;
	if (!unique_tmp(&file[NEW], *filename, "_exp")
		|| !open_fds(&fd[OLD], &fd[NEW], *filename, file[NEW]))
		return (0);
	err = expand_into_new_file(fd, buf, sh);
	close(fd[NEW]);
	close(fd[OLD]);
	unlink(file[OLD]);
	free(*filename);
	if (err)
		return (perror("Error: expand heredoc:"), 0);
	*filename = file[NEW];
	return (1);
}
