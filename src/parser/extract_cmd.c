/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/18 18:24:23 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	set_limiter(char *str, int *i_ltr, int *expand)
{
	if (*str == '\'')
		*expand = DONT_EXPAND;
	if (*str == '\'' || *str == '\"')
	{
		*i_ltr += 1;
		return (str[0]);
	}
	return (' ');
}

int	skip_count_word(char *str, t_s_token *t)
{
	int		i_ltr;
	int		char_type;
	char	*limiter;

	i_ltr = 0;
	char_type = type(str + i_ltr);
	limiter = &t->quotes;
	if (char_type != CMD)
		return (i_ltr);
	*limiter = set_limiter(str, &i_ltr, &t->expand);
	while (str[i_ltr] && char_type == CMD && str[i_ltr] != *limiter)
	{
		if (*limiter == ' ' && (str[i_ltr] == '\'' || str[i_ltr] == '\"'))
			break;
		if (str[i_ltr] == '$'
			&& t->expand == POTENCIALLY_EXPAND)
			t->expand = EXPAND;
		i_ltr++;
		char_type = type(str + i_ltr);
	}
	if (*limiter == '\'' || *limiter == '\"')
		i_ltr++;
	if (str[i_ltr] && str[i_ltr] != ' ')
		t->space_after = NO_SPACE_AFTER;
	return (i_ltr);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	t_s_token t;
	int		i_ltr;

	t.quotes = NO_QUOTES;
	t.space_after = SPACE_AFTER;
	t.expand = POTENCIALLY_EXPAND;
	t.word = NULL;
	s->i_word++;
	i_ltr = skip_count_word(str, &t);
	if (str[i_ltr])
		s->n_cmd_ltrs += extract_cmd_recursive(ast, str + i_ltr, s);
	if (!ast->args && !allocate_ast_args(ast, s->i_word))
		return (0);
	t.word = ms_strcpy(str, i_ltr);
	if (!t.word)
		return (0);
	ast->args[--s->i_word] = t.word;
	ast->space_args[s->i_word] = t.space_after;
	ast->exp_args[s->i_word] = t.expand;
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
	*str += s->n_cmd_ltrs;
	return (*ast);
}
