/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:35 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/17 20:59:18 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

t_ast	*extract_subshell(t_ast **ast, char **str)
{
	int		i_ltr;
	int		paretheses;

	i_ltr = 0;
	paretheses = 1;
	if (**str != '(')
		return (NULL);
	*str += paretheses;
	while ((*str)[i_ltr] && (*str)[i_ltr] != ')')
		i_ltr++;
	if (!(*str)[i_ltr])
		return (perror("ERROR: Missing ')'"), NULL);
	(*ast)->args[0].tkns[0] = ms_strcpy(*str, i_ltr);
	if (!(*ast)->args[0].tkns[0])
		return (NULL);
	if (!make_node(ast)
		|| !allocate_ast_args(&(*ast)->args[0], 1))
		return (NULL);
	(*ast)->args[0].exp[0] = 0;
	(*ast)->args[0].space[0] = 0;
	(*ast)->args[0].n = 1;
	(*ast)->type = SUBSHELL;
	*str += i_ltr + paretheses;
	return (*ast);
}

