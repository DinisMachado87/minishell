/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:59 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/14 13:15:13 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
	{
		*ptr = 0;
		ptr++;
	}
}

int	ms_strcmp(char *ref, char *str)
{
	while (*ref && *str && *ref == *str)
	{
		ref++;
		str++;
	}
	if (*ref)
		return (1);
	return (0);
}

int	ms_strlen(char *str)
{
	int	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

int	is_alphanumeric_or_underscore(char chr)
{
	if ((chr >= 'a' && chr <= 'z')
		|| (chr >= 'A' && chr <= 'Z')
		|| (chr >= 0 && chr <= 9)
		|| (chr >= '_'))
		return (1);
	return (0);
}
