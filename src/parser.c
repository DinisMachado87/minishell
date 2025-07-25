/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/03 13:37:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char *str)
{
	t_ast	*head_ast;
	t_ast	*cur_ast;
	t_s_parser	s;

	while (*str && *str == ' ')
		str++;
	if (is_operator(str))
		return (0);
	head_ast = extract_cmd(&str, &s);
	if (head_ast == NULL)
		return (NULL);
	cur_ast = head_ast;
	while (*str)
	{
		if (str && is_operator(str) == SUBSHELL)
			cur_ast->next = extract_subshell(&str);
		else
			return (head_ast);
		if (cur_ast->next == NULL)
			return (head_ast);
		cur_ast = cur_ast->next;
		if (str && is_operator(str))
			return (perror("ERROR: input includes two consecutive operators"), NULL);
		if (str)
			cur_ast->next = extract_cmd(&str, &s);
		else
			return (head_ast);
		if (cur_ast->next == NULL)
			return (head_ast);
		cur_ast = cur_ast->next;
	// tokenize_and_save_vars();
	// compose_node();
	// expand_vars();
	//make_tree(&ast_list, &ast_tree);
	}
	return (head_ast);
}
