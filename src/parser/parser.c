/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/24 16:15:03 by dimachad         ###   ########.fr       */
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
int	parser(char *str, t_shell *sh)
{
	t_ast	*cur;

	cur = NULL;
	new_line_to_null(str);
	while (*str && *str == ' ')
		str++;
	while (*str)
	{
		if (type(str) > REDIRECT)
			return (psynterr(&str[0], sh));
		if (!extract_subshell(&cur, &str)
			&& !extract_cmd(&str, &cur, sh))
		{
			if (sh->exit_status == 2)
				return (SYNTAX);
			else
				return (ERROR);
		}
		if (!sh->list)
			sh->list = cur;
		if (is_end_of_string(&str))
			break;
		if (!extract_operator(&cur, &str, type(str)))
			return (ERROR);
		if (is_end_of_string(&str))
			break;
	}
	sh->ast = structure_ast(sh->list);
	return (1);
}
