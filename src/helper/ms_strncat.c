/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:01 by jlind             #+#    #+#             */
/*   Updated: 2025/08/22 13:21:02 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ms_strncat(char *dst, char *src, int ssize)
{
	char	*ret;

	ret = dst;
	while (*dst)
		dst++;
	while (*src && ssize--)
		*dst++ = *src++;
	*dst = '\0';
	return (ret);
}
