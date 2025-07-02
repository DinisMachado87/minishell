/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:19:38 by jlind             #+#    #+#             */
/*   Updated: 2025/07/02 17:55:19 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



// @make_node creates a node for a new expression and set it as next of the previous or ast head if the first
int	make_node(t_ast **ast)
{
	t_ast	*ast_cpy;
	t_ast	*new_node;


	new_node = malloc(sizeof(t_ast));
	if (!new_node)
		return (0);
	if (!(*ast))
		*ast = new_node;
	else
	{
		ast_cpy = *ast;
		while (ast_cpy->next)
			ast_cpy = ast_cpy->next;
		ast_cpy->next = new_node;
	}
	return (1)
}

