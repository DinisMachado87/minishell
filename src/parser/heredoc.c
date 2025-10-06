/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:35:02 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/06 18:59:37 by dimachad         ###   ########.fr       */
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

int	ms_heredoc(t_args *args, int offset, int expand)
{
	pid_t		pid;
	char		*temp_file;
	int			i;
	static int	uni = 0;

	if (!cat_str_arr(&args->tkns[offset], args->tkns + offset, args->n
			- offset))
		return (ERROR);
	i = 0;
	uni++;
	while (args->tkns[offset + ++i])
		free_and_null((void **)&args->tkns[offset + i]);
	pid = fork();
	if (pid == 0 && (!unique_tmp(&temp_file, TEMP_PREFIX, itoa(uni))
			|| !create_heredoc_file(args->tkns[offset], temp_file)))
		return (ERROR);
	else if (pid > 0 && (!unique_tmp(&temp_file, TEMP_PREFIX, itoa(uni))
			|| !wait_to_store_file(pid, &temp_file, &args->tkns[offset])))
		return (ERROR);
	else if (pid < 0)
		return (ERROR);
	args->exp[offset] = !expand;
	return (1);
}
