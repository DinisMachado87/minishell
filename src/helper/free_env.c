/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:22:33 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 09:25:02 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env_list(char	**list)
{
	int	i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

void	free_env(t_env **head)
{
	t_env	*next;

	while (*head)
	{
		next = (*head)->next;
		free_env_node(head);
		*head = next;
	}
	free(*head);
	head = NULL;
}

void	free_env_node(t_env **node)
{
	if (!node || !(*node))
		return ;
	if ((*node)->key)
		free((*node)->key);
	if ((*node)->value)
		free((*node)->value);
	free(*node);
	*node = NULL;
}

void	free_env_node_by_key(t_env **head, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (ms_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			free_env_node(&curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
