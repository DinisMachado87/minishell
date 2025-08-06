/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_operator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:24 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:36:28 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator)
{

	if (type(*str) < PIPE)
		return (perror("Error: Input needs pipe or operator bettween comands"), NULL);
	if (!make_node(ast_nd))
		return (perror("ERROR allocating node "
				 "while extracting subshell"), NULL);
	(*ast_nd)->type = operator;
	(*ast_nd)->subtype = subtype(*str);
	if (operator == PIPE)
		(*str)++;
	else
		*str += 2;
	return (*ast_nd);
}
