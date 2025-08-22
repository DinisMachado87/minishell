/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:25:01 by jlind             #+#    #+#             */
/*   Updated: 2025/08/22 13:25:11 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ms_strncpy(char *dst, char *src, int ssize)
{
	char	*ret;

	ret = dst;
	while (ssize && *src)
	{
		*dst++ = *src++;
		ssize--;
	}
	while (ssize--)
		*dst++ = '\0';
	return (ret);
}
