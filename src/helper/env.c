/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:55 by jlind             #+#    #+#             */
/*   Updated: 2025/09/14 19:11:33 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*gen_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ms_strndup(key, ms_strlen(key));
	node->value = ms_strndup(value, ms_strlen(value));
	node->next = NULL;
	return (node);
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

t_env	*get_env_node(t_env *head, char *key)
{
	while (head)
	{
		if (ms_strcmp(head->key, key) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

int	get_env_length(t_env *head)
{
	int	len;

	len = 0;
	while (head)
	{
		len++;
		head = head->next;
	}
	return (len);
}

char	*get_env_str(char *key, char *value)
{
	int		total_len;
	char	*str;

	total_len = ms_strlen(key);
	total_len += ms_strlen(value) + 2;
	str = malloc(sizeof(char) * total_len);
	if (!str)
		return (NULL);
	ms_bzero(str, total_len);
	ms_strncpy(str, key, ms_strlen(key));
	ms_strncat(str, "=", 2);
	ms_strncat(str, value, ms_strlen(value));
	return (str);
}

char	**convert_env_to_list(t_env *head)
{
	char	**list;
	int		env_len;
	int		i;

	i = 0;
	env_len = get_env_length(head);
	list = malloc(sizeof(char *) * (env_len + 1));
	while (i < env_len)
	{
		list[i] = get_env_str(head->key, head->value);
		head = head->next;
		i++;
	}
	list[i] = NULL;
	return (list);
}

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
