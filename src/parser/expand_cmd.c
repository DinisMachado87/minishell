/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:29:36 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/03 01:48:55 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	expand_tkn(t_ast *ast, int i, t_env *env_head)
{
	char	*env_value;

	if (!get_env(&env_value, env_head, ast->args[i])
		|| !free_and_null((void **)&ast->args[i])
		|| !safe_malloc((void **)&ast->args[i], ms_strlen(env_value))
		|| !ms_strncpy(ast->args[i], env_value, ms_strlen(env_value)))
		return (0);
	return (1);
}

void	free_and_reassign(char **dest, char **src) 
{
	free_and_null((void **)dest);
	*dest = *src;
	*src = NULL;
}

int	tkns_to_words(t_ast *ast, int *new_i)
{
	int	i;

	i = 0;
	while (i < ast->n_args)
	{
		int		interval;
		char	**arr_needle;
		char	*new_arg;

		interval = 0;
		while (ast->space_args[i + interval] == NO_SPACE_AFTER)
			interval++;
		arr_needle = ast->args + i;
		if (interval == 0 && i != *new_i)
			free_and_reassign(&new_arg, &ast->args[i]);
		else if (interval > 0
			&& !cat_str_arr(&new_arg, &arr_needle, interval + 1))
			return (0);
		if (i != *new_i || interval > 0)
			free_and_reassign(&ast->args[*new_i], &new_arg);
		(*new_i)++;
		i += interval + 1;
	}
	ast->n_args = *new_i;
	return (1);
}

int	cmd_expander(t_ast *ast, t_env *env_head)
{
	int	old_len;
	int	i;
	int	new_i;
	
	old_len = ast->n_args;
	i = 0;
	while (i < ast->n_args)
	{
		if (ast->exp_args[i] == EXPAND
			&& !expand_tkn(ast, i, env_head))
			return (0);
		i++;
	}
	new_i = 0;
	if (!tkns_to_words(ast, &new_i))
		return (0);
	while (new_i <= old_len)
		free_and_null((void **)&ast->args[new_i++]);
	ast->subtype = subtype(ast->args[0]);
	if (DEBUG)
		print_ast(ast, "AFTER EXPANSION");
	return (1);
}
