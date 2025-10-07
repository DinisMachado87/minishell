/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:24 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/17 20:57:47 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

t_ast	*make_node(t_ast **ast)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
	{
		perror("ERROR: allocating subshell node");
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
