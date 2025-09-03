/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/02 19:33:03 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_end_of_string(char **str)
{
	while (**str == ' ')
		(*str)++;
	if (!**str || **str == '\n')
		return (1);
	return (0);
}

void	new_line_to_null(char *str)
{
	int	len;

	len = ms_strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = '\0';
}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char *str, t_ast **list_head)
{
	t_parser	s;

	if (!str || *str == '\0')
		return (perror("ERROR: No str or empty str"), NULL);
	ms_bzero((void *)&s, sizeof(t_parser));
	new_line_to_null(str);
	while (*str && *str == ' ')
		str++;
	while (*str)
	{
		if (type(str) > REDIRECT)
			return (perror("Error: Input must start with command"),
				free_ast(&s.ast));
		if (!extract_subshell(&s.ast, &str)
			&& !extract_cmd(&str, &s))
				return (free_ast(&s.ast));
		if (!*list_head)
			*list_head = s.ast;
		if (is_end_of_string(&str))
			break;
		if (!extract_operator(&s.ast, &str, type(str)))
			return (free_ast(&s.ast));
		if (is_end_of_string(&str))
			break;
	}
	return (structure_ast(*list_head));
}
