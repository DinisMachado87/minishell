/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:29:36 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/25 02:51:33 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

typedef struct s_state_cmd_expander {
	char **new_word;
	t_env *env_head;
}	t_exp;

typedef struct s_expander_i {
	int	chr;
	int	env;
	int	copy;
}	t_exp_i;

char	*get_env(t_env *head, char *key, int size)
{
	while (head)
	{
		if (ms_strncmp(head->key, key, size) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

int	allocate_new_word(t_exp *e, int	total_chr)
{
	*e->new_word = malloc(total_chr + 1);
	if (!*e->new_word)
		return (perror("Error: Malloc expanding token."), ERROR);
	*e->new_word[0] = '\0';
	return (0);
}

static int exp_token_rec(char *token,  int total_chr, t_exp *e)
{
	char	*expanded_value;
	t_exp_i i = {0};

	if (!*token)
		return (allocate_new_word(e, total_chr));
	while (token[i.chr] && token[i.chr] != '$')
		i.chr++;
	while (token[i.chr + i.env]
		&& is_alphanum_or_underscore(token[i.chr + i.env]))
		i.env++;
	if (i.env)
	{
		expanded_value = get_env(e->env_head, token + i.chr + 1, i.env - 1);
		i.env = ms_strlen(expanded_value);
	}
	if (0 > exp_token_rec(token + i.chr + i.env, total_chr + i.chr + i.env, e))
		return (ERROR);
	if (i.chr--)
		while (i.copy <= i.chr)
		{
			*e->new_word[total_chr + i.copy] = token[i.copy];
			i.copy++;
		}
	if (i.env--)
		while (i.copy <= (i.env + i.chr))
			*e->new_word[total_chr + i.copy++] = expanded_value[i.env++];
	return (total_chr + i.copy);
}

int	cmd_expander(t_ast *ast, t_env *env_head)
{
	t_exp	e;
	int		i;

	e.env_head = env_head;
	i = 0;
	while (i < ast->n_args)
	{
		if (ast->exp_args[i] == EXPAND
			&& !exp_token_rec(ast->args[i], 0, &e))
			return (ERROR);
		i++;
	}
	return (1);
}
