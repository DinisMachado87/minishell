/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tkn_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:30:09 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/01 19:54:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	expand_exit_status(char **tkn, t_shell *sh)
{
	free_and_null((void **)tkn);
	*tkn = itoa(sh->exit_status);
	if (!*tkn)
		return (0);
	return (1);
}

static char	*get_env(char **expanded_value, t_env *head, char *key)
{
	key++;
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

int	expand_tkn_arr(t_args *args, t_shell *sh)
{
	char	*env_value;
	int		i;

	i = 0;
	while (i < args->n)
	{
		if (args->exp[i] != EXPAND)
		{
			i++;
			continue ;
		}
		else if (args->type[i] == HEREDOC)
		{
			if (!expand_heredoc(&args->tkns[i], sh))
				return (0);
		}
		else if (args->tkns[i][1] == '?' && args->tkns[i][2] == '\0')
		{
			if (!expand_exit_status(&args->tkns[i], sh))
				return (0);
		}
		else if (!get_env(&env_value, sh->env, args->tkns[i]))
			ms_bzero((void *)args->tkns[i], ms_strlen(args->tkns[i]));
		else if (!free_and_null((void **)&args->tkns[i])
			|| !safe_malloc((void **)&args->tkns[i], ms_strlen(env_value) + 1)
			|| !ms_strncpy(args->tkns[i], env_value, ms_strlen(env_value) + 1))
			return (0);
		i++;
	}
	return (1);
}

