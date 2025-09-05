/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:29:36 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 02:00:13 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stddef.h>

char	*get_env(char **expanded_value, t_env *head, char *key)
{
	while (head)
	{
		if (ms_strncmp(head->key, key, ms_strlen(key)) == 0)
		{
			*expanded_value = head->value;
			return (*expanded_value);
		}
		head = head->next;
	}
	return (NULL);
}

int	free_and_reassign(char **dest, char **src) 
{
	free_and_null((void **)dest);
	*dest = *src;
	*src = NULL;
	return (1);
}

int	word_len(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

int	split_len(char *str)
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

int	move_arr(char **dst, char **src, int len)
{
	if (dst < src)
		while (len--)
			*dst++ = *src++;
	else if (dst > src)
		while (len--)
			dst[len] = src[len];
	return (1);
}

int extract_skip_section(char **str, char **dst, int len)
{
	*dst = ms_strndup(*str, len);
	if (!*dst)
		return (0);
	*str += len;
	while (**str == ' ')
		(*str)++;
	return (1);
}

int	split_cmd_flags(char *str, t_ast *ast, int alloc_len)
{
	int	total_len;
	int	splt_len;
	int	i;
	char **new_arr;

	splt_len = split_len(str);
	if (splt_len == 1)
		return (0);
	total_len = ast->n_args + splt_len - 1;
	if (total_len > alloc_len
		&& !safe_malloc((void **)&new_arr, (total_len + 1) * sizeof(char *)))
			return (0);
	else
		new_arr = ast->args;
	new_arr[total_len] = NULL;
	move_arr(new_arr + splt_len, ast->args + 1, ast->n_args - 1);
	i = 0;
	while (*str)
		if (!extract_skip_section(&str, &new_arr[i++], word_len(str)))
			return (0);
	if (new_arr != ast->args)
		free_and_null((void **)&ast->args);
	ast->args = new_arr;
	ast->n_args = total_len;
	return (1);
}

int	tkns_to_words(char **tkn_arr, int *space_arr, int arr_len)
{
	int	i;
	int	new_i;

	i = 0;
	new_i = 0;
	while (i < arr_len)
	{
		int		interval;

		interval = 0;
		while (i + interval + 1 < arr_len
			&& (!space_arr || space_arr[i + interval] == NO_SPACE_AFTER))
			interval++;
		if (++interval == 1 && i != new_i)
			free_and_reassign(&tkn_arr[new_i], &tkn_arr[i]);
		else if (interval > 1)
			if (!cat_str_arr(&tkn_arr[new_i], tkn_arr + i, interval))
				return (0);
		new_i++;
		i += interval;
	}
	arr_len = new_i;
	while (new_i < arr_len)
		free_and_null((void **)&tkn_arr[new_i++]);
	return (1);
}

int	expand_tkn_arr(char **tkn_arr, int *exp_arr, int n_tks, t_env *env_head)
{
	char	*env_value;
	int		i;

	i = 0;
	while (i < n_tks)
	{
		if (exp_arr[i] != EXPAND)
		{
			i++;
			continue;
		}
		if (!get_env(&env_value, env_head, tkn_arr[i])
			|| !free_and_null((void **)&tkn_arr[i])
			|| !safe_malloc((void **)&tkn_arr[i], ms_strlen(env_value) + 1)
			|| !ms_strncpy(tkn_arr[i], env_value, ms_strlen(env_value) + 1))
			return (0);
		i++;
	}
	return (1);
}

static int	expand_cat_red_tkns(
		int subtype, t_ast *ast, int alloc_len, t_env *env_head)
{
	if (!expand_tkn_arr(ast->pre_r_args[subtype],
					 ast->r_exp_args[subtype],
					 ast->n_red_tk[subtype],
					 env_head)
		|| !tkns_to_words(ast->pre_r_args[subtype], NULL, alloc_len))
		return (0);
	ast->red_args[subtype] = ast->pre_r_args[subtype][0];
	return (1);
}

int	cmd_expander(t_ast *ast, t_env *env_head)
{
	int	alloc_len;
	
	alloc_len = ast->n_args;
	if (!expand_tkn_arr(ast->args, ast->exp_args, ast->n_args, env_head)
		|| !tkns_to_words(ast->args, ast->space_args, alloc_len)
		|| !split_cmd_flags(ast->args[0], ast, alloc_len))
		return (0);
	ast->subtype = subtype(ast->args[0]);
	if (ast->pre_r_args[IN])
		expand_cat_red_tkns(IN, ast, alloc_len, env_head);
	if (ast->pre_r_args[OUT])
		expand_cat_red_tkns(OUT, ast, alloc_len, env_head);
	if (DEBUG)
		print_ast(ast, "AFTER EXPANSION");
	return (1);
}
