/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/08 13:09:13 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*unique_tmp(char **dest, char *str1, char *str2)
{
	int	i;
	int	len;

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

int	wait_to_store_file(pid_t pid, char **temp_file, char **arg_tkn)
{
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;
	int					status;

	status = 0;
	set_and_save_signal(SIGINT, SIG_IGN, &old_sigint);
	set_and_save_signal(SIGQUIT, SIG_IGN, &old_sigquit);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		free(*temp_file);
		sigaction(SIGINT, &old_sigint, NULL);
		sigaction(SIGQUIT, &old_sigquit, NULL);
		return (0);
	}
	free_and_reassign(arg_tkn, temp_file);
	sigaction(SIGINT, &old_sigint, NULL);
	sigaction(SIGQUIT, &old_sigquit, NULL);
	return (1);
}

int	ms_heredoc(t_args *args, int offset, int expand, t_cmd *c)
{
	pid_t		pid;
	char		*temp_file;
	int			i;
	static char	uni = 'A';

	if (!cat_str_arr(&args->tkns[offset], args->tkns + offset, args->n
			- offset))
		return (ERROR);
	i = 0;
	uni++;
	while (args->tkns[offset + ++i])
		free_and_null((void **)&args->tkns[offset + i]);
	pid = fork();
	if (pid == 0 && (!unique_tmp(&temp_file, TEMP_PREFIX, &uni)
			|| !create_heredoc_file(args->tkns[offset], temp_file, c)))
		return (ERROR);
	else if (pid > 0 && (!unique_tmp(&temp_file, TEMP_PREFIX, &uni)
			|| !wait_to_store_file(pid, &temp_file, &args->tkns[offset])))
		return (ERROR);
	else if (pid < 0)
		return (ERROR);
	args->exp[offset] = !expand;
	return (0);
}
