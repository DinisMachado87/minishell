/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:59 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/03 01:57:58 by dimachad         ###   ########.fr       */
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
	if (!ref || !str)
		return (1);
	while (*ref && *str && *ref == *str)
	{
		ref++;
		str++;
	}
	if (*ref)
		return (1);
	return (0);
}

int	ms_strncmp(char *s1, char *s2, int size)
{
	int i;
	
	i = 0;
	while (i < size && s1[i] && s2[i] && s1[i] == s2[i] )
		i++;	
	if (i < size)
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
		return(NULL);
	return (*ptr);
}

char	*cat_str_arr(char **dest, char **str_arr, int size)
{
	int		total_len;
	int		i_str;
	char	*new_str;

	total_len = 0;
	i_str = 0;
	while (str_arr[i_str] && i_str < size)
		total_len += ms_strlen(str_arr[i_str++]);
	if (!safe_malloc((void **)&new_str, total_len + 1))
		return (perror("Error concatenating tokens to words: "), NULL);
	total_len = 0;
	i_str = 0;
	while (str_arr[i_str] && i_str < size)
	{
		int	i_ltr;

		i_ltr = 0;
		while (str_arr[i_str][i_ltr])
			new_str[total_len++] = str_arr[i_str][i_ltr++];
		i_str++;
	}
	new_str[total_len] = '\0';
	free_and_null((void **)dest);
	*dest = new_str;
	return (*dest);
}
