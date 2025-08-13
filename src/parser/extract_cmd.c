/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/13 01:14:17 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_count_word(char *str, char limiter)
{
	int	i_ltr;
	int	char_type;

	i_ltr = 0;
	char_type = type(str + i_ltr);
	while (str[i_ltr] && str[i_ltr] != limiter && (char_type == CMD || limiter != ' '))
	{
		if (limiter == ' '
			&& (str[i_ltr] == '\'' || str[i_ltr] == '"'))
				limiter = str[i_ltr];
		i_ltr++;
		char_type = type(str + i_ltr);
	}
	if (str[i_ltr] && str[i_ltr] == limiter && limiter != ' ')
		i_ltr++;
	return (i_ltr);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	int		i_ltr;
	char	*word;

	i_ltr = skip_count_word(str, ' ');
	s->i_word++;
	if (str[i_ltr])
		s->n_cmd_ltrs += extract_cmd_recursive(ast, str + i_ltr, s);
	if (!ast->args && !allocate_ast_args(ast, s->i_word))
		return (0);
	word = ms_strcpy(str, i_ltr);
	if (!word)
		return (0);
	ast->args[--s->i_word] = word;
	return(i_ltr);
}

int	extract_cmd_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	int		spaces;

	spaces = 0;
	while (str[spaces] && str[spaces] == ' ')
		spaces++;
	if (!str[spaces] || str[spaces] == '(')
		return (spaces);
	if (str[spaces] && REDIRECT == type(str + spaces))
		s->n_cmd_ltrs += extract_redirect(ast, str + spaces, s);
	if (str[spaces] && CMD == type(str + spaces))
		s->n_cmd_ltrs += extract_word_recursive(ast, str + spaces, s);
	return (spaces);
}

t_ast	*extract_cmd(t_ast **ast_nd, char **str, t_s_parser *s)
{
	s->i_word = 0;
	s->n_cmd_ltrs = 0;
	if (!make_node(ast_nd))
		return (NULL);
	s->n_cmd_ltrs += extract_cmd_recursive(*ast_nd, *str, s);
	if (!s->n_cmd_ltrs)
		return (free_all(ast_nd), NULL);
	(*ast_nd)->type = CMD;
	if ((*ast_nd)->args[0])
		(*ast_nd)->subtype = subtype((*ast_nd)->args[0]);
	*str += s->n_cmd_ltrs;
	return (*ast_nd);
}
