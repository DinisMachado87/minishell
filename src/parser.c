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
	cur_ast = head_ast;
	while (*str)
	{
		cur_ast->next = extract_cmd(&str, &s);
		cur_ast = cur_ast->next;
	// tokenize_and_save_vars();
	// compose_node();
	// expand_vars();
	//make_tree(&ast_list, &ast_tree);
	}
	return (head_ast);
}
