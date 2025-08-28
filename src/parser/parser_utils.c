/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:31 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/27 22:37:15 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	type(char *str)
{
	int	i;

	i = 0;
	while (g_types[i].str)
	{
		if (!ms_strcmp(g_types[i].str, str))
			return (g_types[i].type);
		i++;
	}
	return (CMD);
}

int	subtype(char *str)
{
	int	i;

	i = 0;
	while (g_types[i].str)
	{
		if (!ms_strcmp(g_types[i].str, str))
			return (g_types[i].subtype);
		i++;
	}
	return (CMD);
}


void	*handle_error(char *err_msg, int *error)
{
	*error = -1;
	perror(err_msg);
	return (0);
}

char	*ms_strcpy(char *str, int len)
{
	char	*substring;

	if (*str == '$')
	{
		str++;
		len--;
	}
	if (*str == '\'' || *str == '\"')
	{
		str++;
		len -= 2;
	}
	substring = malloc((len + 1) * sizeof(char));
	if (!substring)
		return (perror("Err extracting substring"), NULL);
	substring[len] = '\0';
	while (0 <= --len)
		substring[len] = str[len];
	return (substring);
}
