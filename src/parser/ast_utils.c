/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:24 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/16 15:32:43 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_and_null(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (1);
}

void free_and_null_str_arr(char	***address_str_arr)
{
	int		i_str;
	char	**str_arr;

	if (!address_str_arr || !*address_str_arr)
		return ;
	i_str = 0;
	str_arr = *address_str_arr;
	while (str_arr[i_str])
		free_and_null((void **)&str_arr[i_str++]);
	free_and_null((void **)address_str_arr);
}

void	free_red_args(t_ast *ast, int r_subtype)
{
	if (ast->red_args[r_subtype])
		free_and_null((void **)&ast->red_args[r_subtype]);
	if (ast->pre_r_args[r_subtype])
		free_and_null_str_arr(&ast->pre_r_args[r_subtype]);
	if (ast->r_exp_args[r_subtype])
		free_and_null((void **)&ast->r_exp_args[r_subtype]);
}

t_ast	*free_ast(t_ast **ast_head)
{
	t_ast *next_head;
	
	next_head = NULL;
	while ((*ast_head))
	{
		next_head = (*ast_head)->next;
		while ((*ast_head)->args && --(*ast_head)->n_args >= 0)
			free_and_null((void **)&(*ast_head)->args[(*ast_head)->n_args]);
		if ((*ast_head)->args)
			free_and_null((void **)&(*ast_head)->args);
		if ((*ast_head)->exp_args)
			free_and_null((void **)&(*ast_head)->exp_args);
		if ((*ast_head)->space_args)
			free_and_null((void **)&(*ast_head)->space_args);
		free_red_args(*ast_head, IN);
		free_red_args(*ast_head, OUT);
		free_and_null((void **)&(*ast_head));
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
		*ast = free_ast(ast);
		return (NULL);
	}
	bzero(new_node, sizeof(t_ast));
	if (!*ast)
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
