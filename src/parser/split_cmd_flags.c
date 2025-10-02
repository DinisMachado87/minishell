/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_flags.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:08:43 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/02 19:56:56 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	split_len(char *str)
{
	int	i_words;

	i_words = 0;
	while (*str)
	{
		while (*str && *str != ' ')
			str++;
		i_words++;
		while (*str && *str == ' ')
			str++;
	}
	return (i_words);
}

static int	extract_skip_section(char **str, char **dst, int len)
{
	*dst = ms_strndup(*str, len);
	if (!*dst)
		return (0);
	*str += len;
	while (**str == ' ')
		(*str)++;
	return (1);
}

static int	word_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

static int	move_arr(char **dst, char **src, int len)
{
	if (dst < src)
		while (len--)
			*dst++ = *src++;
	else if (dst > src)
		while (len--)
			dst[len] = src[len];
	return (1);
}

int	split_cmd_flags(char *str, t_args *args, int alloc_len)
{
	int		total_len;
	int		splt_len;
	int		i;
	char	**new_arr;

	splt_len = split_len(str);
	if (splt_len == 1)
		return (1);
	total_len = args->n + splt_len - 1;
	if (total_len > alloc_len
		&& !safe_malloc((void **)&new_arr, (total_len + 1) * sizeof(char *)))
		return (0);
	else
		new_arr = args->tkns;
	new_arr[total_len] = NULL;
	move_arr(new_arr + splt_len, args->tkns + 1, args->n - 1);
	i = 0;
	while (*str)
		if (!extract_skip_section(&str, &new_arr[i++], word_len(str)))
			return (0);
	if (new_arr != args->tkns)
		free_and_null((void **)&args->tkns);
	args->tkns = new_arr;
	args->n = total_len;
	return (1);
}
