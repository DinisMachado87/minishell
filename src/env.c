#include "../include/minishell.h"

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

void	free_env_node(t_env **head, char *key)
{
	t_env *node_cpy;

	if (!*head || !head)
		return ;
	node_cpy = *head;
	while (node_cpy)
	{
		if (strcmp(node_cpy->key, key) == 0)
		{
			free(node_cpy->key);
			free(node_cpy->value);
			free(node_cpy);
			break ;
		}
		node_cpy = node_cpy->next;
	}
}

void	set_env_node(t_env **head, char *key, char *value)
{
	t_env	*new_node;

	new_node = gen_env_node(key, value);
	if (!(*head))
		*head = new_node;
	else
	{
		free_env_node(head, key);
		while ((*head)->next)
			*head = (*head)->next;
		(*head)->next = new_node;
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
	total_len += strlen(value) + 1;
	str = malloc(sizeof(char) * total_len);
	strncpy(str, key, strlen(key));
	strncat(str, "=", 1);
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

void	free_env(t_env **head)
{
	t_env	*tmp;
	t_env	*next;

	tmp = *head;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next;
	}
}

int	main(void)
{
	t_env	*head;
	t_env	*tmp;

	head = NULL;
	set_env_node(&head, "USER", "jlind");
	tmp = get_env_node(head, "USER");
	printf("%s\n", tmp->value);
	return (0);
}
