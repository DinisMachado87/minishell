/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:47:12 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

int	is_end_of_string(char **str)
{
	while (**str == ' ')
		(*str)++;
	if (!**str)
		return (1);
	return (0);
}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char *str, t_ast **head_list)
{
	t_ast	*cur_list;
	t_s_parser	s;

	ms_bzero((void *)&s, sizeof(t_s_parser));
	cur_list = NULL;
	if (!str || *str == '\0')
		return (perror("ERROR: No str or empty str"), NULL);
	while (*str && *str == ' ')
		str++;
	while (*str)
	{
		if (type(str) > REDIRECT)
			return (perror("Error: Input must start with a command or redirection"), free_ast(head_list));
		if (!extract_subshell(&cur_list, &str)
			&& !extract_cmd(&cur_list, &str, &s))
				return (free_ast(head_list));
		if (!*head_list)
			*head_list = cur_list;
		if (is_end_of_string(&str))
			break;
		if (!extract_operator(&cur_list, &str, type(str)))
			return (free_ast(head_list));
		if (is_end_of_string(&str))
			break;
	}
	return (structure_ast(*head_list));
}
