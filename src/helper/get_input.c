/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:13:46 by jlind             #+#    #+#             */
/*   Updated: 2025/10/06 16:26:29 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	reassign_and_null(char **dst, char **src)
{
	*dst = *src;
	*src = NULL;
	return (1);
}

int	extend_str(char **dst, char **src)
{
	char	*str;
	char	*ext;
	char	*catstr;
	int		i;

	if (!dst || !src || !*dst || !*src)
		return (0);
	str = *dst;
	ext = *src;
	catstr = NULL;
	i = 0;
	if (!safe_malloc((void **)&catstr, ms_strlen(str) + ms_strlen(ext) + 1))
		return (perror("Error: Malloc input:"), 0);
	while (*str)
		catstr[i++] = *str++;
	while (*ext)
		catstr[i++] = *ext++;
	catstr[i] = '\0';
	free_and_null((void **)dst);
	free_and_null((void **)src);
	*dst = catstr;
	return (1);
}

int	store_or_cat_input(char **new_read, char **input)
{
	if (new_read
		&& ((!*input && reassign_and_null(input, new_read))
			|| (*input && extend_str(input, new_read))))
		return (1);
	free_and_null((void **)input);
	free_and_null((void **)&new_read);
	return (0);
}

void	skip_spaces(char *str)
{
	int	i;
	int	j;

	i = 0;
	while (is_space(str[i]))
		i++;
	if (!i)
		return ;
	j = 0;
	while (str[i])
	{
		str[j] = str[i];
		i++;
		j++;
	}
	str[j] = '\0';
}

int	get_input(char *prompt, char **input, int *eof)
{
	char	*new_read;
	int		even;

	even = 0;
	set_handler(1);
	while (!even || !*input)
	{
		if (!*input)
			new_read = readline(prompt);
		else
			new_read = readline("> ");
		if (!new_read)
		{
			*eof = 1;
			break ;
		}
		if (!store_or_cat_input(&new_read, input))
			return (set_handler(0), ERROR);
		even = str_pairs_even(*input);
	}
	skip_spaces(*input);
	set_handler(0);
	if (even != ERROR && *input && **input != '\0')
		add_history(*input);
	return (even);
}
