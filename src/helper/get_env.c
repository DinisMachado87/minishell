/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:21:55 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 09:21:50 by jlind            ###   ########.fr       */
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
