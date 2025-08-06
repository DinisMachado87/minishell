/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:35 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:36:38 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

t_ast	*extract_subshell(t_ast **ast_nd, char **str)
{
	int		i_ltr;
	int		paretheses;
	char	*substr;

	i_ltr = 0;
	paretheses = 1;
	if (**str != '(')
		return (NULL);
	*str += paretheses;
	while ((*str)[i_ltr] && (*str)[i_ltr] != ')')
		i_ltr++;
	if (!(*str)[i_ltr])
		return (perror("ERROR: Missing ')'"), NULL);
	substr = ms_strcpy(*str, i_ltr);
	if (!substr)
		return (NULL);
	if (!make_node(ast_nd))
		return (perror("ERROR allocating node "
				 "while extracting subshell"), NULL);
	if (!allocate_ast_args(*ast_nd, 1))
		return (NULL);
	(*ast_nd)->args[0] = substr;
	(*ast_nd)->n_args = 1;
	(*ast_nd)->type = SUBSHELL;
	*str += i_ltr + paretheses;
	return (*ast_nd);
}

