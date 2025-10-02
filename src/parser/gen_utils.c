/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:59 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/02 18:52:39 by dimachad         ###   ########.fr       */
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

int	is_alphanum_or_underscore(char chr)
{
	if ((chr >= 'a' && chr <= 'z')
		|| (chr >= 'A' && chr <= 'Z')
		|| (chr >= '0' && chr <= '9')
		|| (chr >= '_'))
		return (1);
	return (0);
}

void	*safe_malloc(void **ptr, size_t size)
{
	*ptr = malloc(size);
	if (!*ptr)
		return (NULL);
	return (*ptr);
}
