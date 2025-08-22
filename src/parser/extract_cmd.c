/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/22 19:31:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	set_limiter(char *str, int *i_ltr, int *expand)
{
	if (*str == '\'')
		*expand = DONT_EXPAND;
	if (*str == '\'' || *str == '\"')
	{
		(*i_ltr)++;
		return (str[0]);
	}
	return (' ');
}

int	skip_count_word(char *str, char *limiter, int *space, int *expand)
{
	int		i_ltr;
	int		char_type;

	i_ltr = 0;
	char_type = type(str + i_ltr);
	if (char_type != CMD)
		return (i_ltr);
	*limiter = set_limiter(str, &i_ltr, expand);
	while (str[i_ltr] && char_type == CMD && str[i_ltr] != *limiter)
	{
		if (*limiter == ' '
			&& (str[i_ltr] == '\'' || str[i_ltr] == '"'))
		{
			*space = NO_SPACE_AFTER;
			return (i_ltr);
		}
		if (str[i_ltr] == '$' && is_alphanumeric_or_underscore(str[i_ltr + 1])
			&& *expand == POTENCIALLY_EXPAND)
			*expand = EXPAND;
		i_ltr++;
		char_type = type(str + i_ltr);
	}
	return (i_ltr);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	int		i_ltr;
	char	*word;
	char	quotes;
	int		space_args;
	int		expand;

	quotes = NO_QUOTES;
	space_args = SPACE_AFTER;
	expand = POTENCIALLY_EXPAND;
	s->i_word++;

	i_ltr = skip_count_word(str, &quotes, &space_args, &expand);
	if (str[i_ltr])
		s->n_cmd_ltrs += extract_cmd_recursive(ast, str + i_ltr, s);
	if (!ast->args && !allocate_ast_args(ast, s->i_word))
		return (0);
	word = ms_strcpy(str, i_ltr);
	if (!word)
		return (0);
	ast->args[--s->i_word] = word;
	ast->space_args[s->i_word] = space_args;
	ast->exp_args[s->i_word] = expand;
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

t_ast	*extract_cmd(t_ast **ast, char **str, t_s_parser *s)
{
	s->i_word = 0;
	s->n_cmd_ltrs = 0;
	if (!make_node(ast))
		return (NULL);
	s->n_cmd_ltrs += extract_cmd_recursive(*ast, *str, s);
	if (!s->n_cmd_ltrs)
		return (free_ast(ast), NULL);
	(*ast)->type = CMD;
	if ((*ast)->args[0])
		(*ast)->subtype = subtype((*ast)->args[0]);
	*str += s->n_cmd_ltrs;
	return (*ast);
}
