/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/04 22:36:31 by dimachad         ###   ########.fr       */
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
	t_ast	*cur_list;
	t_s_parser	s;

	while (*str && *str == ' ')
		str++;
	if (type(str) > REDIRECT)
		return (perror("Error: Input must start with a command or redirection"), NULL);
	if (!extract_subshell(&head_ast, &str)
		&& !extract_cmd(&head_ast, &str, &s))
			return (NULL);
	cur_list = head_ast;
	while (*str)
	{
		if (is_end_of_string(&str))
			return (head_ast);
		if (!extract_subshell(&cur_list->next, &str))
			if (!extract_operator(&cur_list->next, &str, type(str)))
				return (NULL);
		if (is_end_of_string(&str))
			return (head_ast);
		if (type(str) > REDIRECT)
			return (perror("ERROR: input includes two consecutive operators"), NULL);
		cur_list = cur_list->next;
		if (!extract_subshell(&cur_list->next, &str)
			&& !extract_cmd(&cur_list->next, &str, &s))
				return (NULL);
		if (is_end_of_string(&str))
			return (head_ast);
		cur_list = cur_list->next;
	}
	// expand_vars();
	head_ast = structure_ast(head_ast);
	return (head_ast);
}
