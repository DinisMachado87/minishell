/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:22:53 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 09:23:05 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_env_node(t_env **head, char *key, char *value)
{
	t_env	*new_node;
	t_env	*node;

	new_node = gen_env_node(key, value);
	if (!(*head))
		*head = new_node;
	else
	{
		node = *head;
		free_env_node_by_key(head, key);
		if (!(*head))
			*head = new_node;
		else
		{
			while (node->next)
				node = node->next;
			node->next = new_node;
		}
	}
}

void	split_env_var(char *var, char **key, char **val)
{
	char	*match;

	if (!*var)
		return ;
	match = ms_strchr(var, '=');
	if (!match)
		return ;
	*key = ms_strndup(var, (ms_strlen(var) - ms_strlen(match)));
	*val = ms_strndup((match + 1), (ms_strlen(match) - 1));
}

void	init_env(t_shell *shell, char *envp[])
{
	char	*key;
	char	*val;

	while (*envp)
	{
		split_env_var(*envp, &key, &val);
		set_env_node(&shell->env, key, val);
		free(key);
		free(val);
		envp++;
	}
}
