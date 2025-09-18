/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:12:31 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/18 13:04:06 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
