/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:11 by jlind             #+#    #+#             */
/*   Updated: 2025/09/17 10:21:41 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ms_strchr(char *s, int c)
{
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			return (s);
		s++;
	}
	if ((unsigned char)*s == (unsigned char)c)
		return (s);
	return (NULL);
}

char	*ms_strrchr(char *s, int c)
{
	int	len;

	len = ms_strlen(s);
	while (len--)
	{
		if (s[len] == (unsigned char)c)
			return (s + len);
	}
	return (NULL);
}
