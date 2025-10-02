/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:31 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/02 20:27:14 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	strict_cmp(char *ref, char *str)
{
	if (!ref || !str)
		return (1);
	while (*ref && *str && *ref == *str)
	{
		ref++;
		str++;
	}
	return (*ref || *str);
}

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

int	strict_subtype(char *str)
{
	int	i;

	i = 0;
	while (g_types[i].str)
	{
		if (!strict_cmp(g_types[i].str, str))
			return (g_types[i].subtype);
		i++;
	}
	return (CMD);
}

char	*ms_strcpy(char *str, int len)
{
	char	*substring;

	if (*str && (*str == '\'' || *str == '\"'))
	{
		str++;
		len--;
	}
	if (str[len - 1] == '\'' || str[len - 1] == '\"')
		len--;
	substring = malloc((len + 1) * sizeof(char));
	if (!substring)
		return (perror("Err extracting substring"), NULL);
	substring[len] = '\0';
	while (0 <= --len)
		substring[len] = str[len];
	return (substring);
}
