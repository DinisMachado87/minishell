/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:59 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/27 17:36:54 by dimachad         ###   ########.fr       */
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
		|| (chr >= 0 && chr <= 9)
		|| (chr >= '_'))
		return (1);
	return (0);
}

char	*cat_str_arr(char **dest, char ***address_str_arr)
{
	char	**str_arr;
	int		total_len;
	int		i_str;
	int		i_ltr;

	str_arr = *address_str_arr;
	total_len = 0;
	i_str = 0;
	i_ltr = 0;
	while (str_arr[i_str])
		total_len += ms_strlen(str_arr[i_str++]);
	*dest = malloc(total_len + 1);
	if (!*dest)
		return (perror("Error concatenating heredoc eof: "), NULL);
	total_len = 0;
	i_str = 0;
	while (str_arr[i_str])
	{
		while (str_arr[i_str][i_ltr])
			*dest[total_len++] = str_arr[i_str][i_ltr++];
		i_str++;
	}
	*dest[total_len] = '\0';
	free_and_null_str_arr(address_str_arr);
	return (*dest);
}
