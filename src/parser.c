/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/29 15:27:00 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	is_end_of_string(char **str)
{
	while (**str == ' ')
		(*str)++;
	if (!**str)
		return (1);
	return (0);
}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char *str)
{
	t_ast	*head_ast;
	t_ast	*cur_ast;
	t_s_parser	s;

	while (*str && *str == ' ')
		str++;
	if (is_operator(str))
		return (perror("Error: Input must start with a command not an operator"), NULL);
	if (!extract_subshell(&head_ast, &str)
		&& !extract_cmd(&head_ast, &str, &s))
			return (NULL);
	cur_ast = head_ast;
	while (*str)
	{
		if (is_end_of_string(&str))
			return (head_ast);
		if (!extract_subshell(&cur_ast->next, &str))
			if (!extract_operator(&cur_ast->next, &str, is_operator(str)))
				return (NULL);
		if (is_end_of_string(&str))
			return (head_ast);
		if (is_operator(str))
			return (perror("ERROR: input includes two consecutive operators"), NULL);
		cur_ast = cur_ast->next;
		if (!extract_subshell(&cur_ast->next, &str)
			&& !extract_cmd(&cur_ast->next, &str, &s))
				return (NULL);
		if (is_end_of_string(&str))
			return (head_ast);
		cur_ast = cur_ast->next;
	}
	// tokenize_and_save_vars();
	// compose_node();
	// expand_vars();
	//make_tree(&ast_list, &ast_tree);
	return (head_ast);
}
