/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:24 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:45:36 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_and_null(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

t_ast	*free_all(t_ast **ast_head)
{
	t_ast *next_head;
	
	next_head = NULL;
	while ((*ast_head))
	{
		next_head = (*ast_head)->next;
		while ((*ast_head)->args && --(*ast_head)->n_args >= 0)
			free_and_null((void *)&(*ast_head)->args[(*ast_head)->n_args]);
		if ((*ast_head)->args)
			free_and_null((void *)&(*ast_head)->args);
		if ((*ast_head)->red_args[IN])
			free_and_null((void *)&(*ast_head)->red_args[IN]);
		if ((*ast_head)->red_args[OUT])
			free_and_null((void *)&(*ast_head)->red_args[OUT]);
		free_and_null((void *)&(*ast_head));
		*ast_head = next_head;
	}
	return (NULL);
}

t_ast	*make_node(t_ast **ast)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
	{
		*ast = free_all(ast);
		return (NULL);
	}
	bzero(new_node, sizeof(t_ast));
	if (!(*ast))
		*ast = new_node;
	else
	{
		while ((*ast)->next)
			*ast = (*ast)->next;
		(*ast)->next = new_node;
		*ast = (*ast)->next;
	}
	return (new_node);
}
