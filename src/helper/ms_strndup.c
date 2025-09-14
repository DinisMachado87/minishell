/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:19 by jlind             #+#    #+#             */
/*   Updated: 2025/09/14 11:31:33 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ms_strndup(char *str, int n)
{
	int		len;
	char	*dup;
	int		i;

	i = 0;
	if (n < 0)
		n = 0;
	len = ms_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	dup[len] = '\0';
	while (i < len)
	{
		dup[i] = *(str + i);
		i++;
	}
	return (dup);
}
