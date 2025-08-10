#include "../../include/minishell.h"

t_env	*gen_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = strdup(key);
	node->value = strdup(value);
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
	t_env *curr = *head;
	t_env *prev = NULL;

	while (curr)
	{
			if (strcmp(curr->key, key) == 0)
			{
					if (prev)
							prev->next = curr->next;
					else
							*head = curr->next;

					free_env_node(&curr);
					return;
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
		if (strcmp(head->key, key) == 0)
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

	total_len = strlen(key);
	total_len += strlen(value) + 2;
	str = malloc(sizeof(char) * total_len);
	if (!str)
		return (NULL);
	bzero(str, total_len);
	strncpy(str, key, strlen(key));
	strncat(str, "=", 2);
	strncat(str, value, strlen(value));
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
