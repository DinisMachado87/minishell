/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/28 01:45:15 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	skip_spaces(t_s_token *cur, t_s_parser *s)
{
	int i_ltr;

	i_ltr = 0;
	while (cur->str[i_ltr] && cur->str[i_ltr] == ' ')
		(i_ltr)++;
	cur->str += i_ltr;
	s->n_cmd_ltrs += i_ltr;
}

int extract_cmd_recursive(t_s_token cur, t_s_parser *s)
{
	t_s_token	nxt;
	int			i_ltr;

	i_ltr = 0;
	skip_spaces(&cur, s);
	if (cur.str[i_ltr] && REDIRECT == type(cur.str + i_ltr))
		if (extract_redirect(&i_ltr, cur, s) == ERROR)
			return (ERROR);
	nxt.limiter = cur.limiter;
	cur.space_after = SPACE_AFTER;
	s->i_word++;
	if (cur.str[i_ltr] && CMD == type(cur.str + i_ltr))
		i_ltr += count_token(cur.str, &cur, &nxt);
	nxt.str = cur.str + i_ltr;
	s->n_cmd_ltrs += i_ltr;
	if (cur.str[i_ltr] && extract_cmd_recursive(nxt, s) == ERROR)
		return (ERROR);
	if (!s->ast->args && !allocate_ast_args(s->ast, s->i_word))
		return (ERROR);
	s->ast->args[--s->i_word] = ms_strcpy(cur.str, i_ltr);
	if (!s->ast->args[s->i_word])
		return (ERROR);
	s->ast->space_args[s->i_word] = cur.space_after;
	s->ast->exp_args[s->i_word] = cur.limiter != '\'' && *cur.str == '$';
	return(i_ltr);
}

// int extract_token(int *i_ltr, t_s_token cur, t_s_parser *s)
// {
// 	t_s_token	nxt;
//
// 	nxt.limiter = cur.limiter;
// 	cur.space_after = SPACE_AFTER;
// 	s->i_word++;
// 	*i_ltr = count_token(cur.str, &cur, &nxt);
// 	nxt.str = cur.str + *i_ltr;
// 	if (cur.str[*i_ltr])
// 		s->n_cmd_ltrs += extract_cmd_recursive(nxt, s);
// 	if (!s->ast->args && !allocate_ast_args(s->ast, s->i_word))
// 		return (ERROR);
// 	s->ast->args[--s->i_word] = ms_strcpy(cur.str, *i_ltr);
// 	if (!s->ast->args[s->i_word])
// 		return (ERROR);
// 	s->ast->space_args[s->i_word] = cur.space_after;
// 	s->ast->exp_args[s->i_word] = cur.limiter != '\'' && *cur.str == '$';
// 	return(*i_ltr);
// }
//
// int	extract_cmd_recursive(t_s_token cur, t_s_parser *s)
// {
// 	int		spaces;
// 	int		n_ltrs;
//
// 	spaces = 0;
// 	n_ltrs = 0;
// 	while (cur.str[spaces] && cur.str[spaces] == ' ')
// 		spaces++;
// 	cur.str += spaces;
// 	if (*cur.str && REDIRECT == type(cur.str))
// 	{
// 		if (extract_redirect(&n_ltrs, cur, s) == ERROR)
// 			return (ERROR);
// 		s->n_cmd_ltrs += n_ltrs;
// 		cur.str += n_ltrs;
// 		n_ltrs = extract_cmd_recursive(cur, s);
// 		if (n_ltrs == ERROR)
// 			return (ERROR);
// 	}
// 	else if (cur.str && CMD == type(cur.str))
// 		if (!extract_token(&n_ltrs, cur, s))
// 			return (ERROR);
// 	s->n_cmd_ltrs += n_ltrs;
// 	return (spaces);
// }

t_ast	*extract_cmd(t_ast **ast, char **str, t_s_parser *s)
{
	t_s_token init;

	s->i_word = 0;
	s->n_cmd_ltrs = 0;
	if (!make_node(ast))
		return (NULL);
	s->ast = *ast;
	init.limiter = ' ';
	init.space_after = SPACE_AFTER;
	init.str = *str;
	s->n_cmd_ltrs += extract_cmd_recursive(init, s);
	if (!s->n_cmd_ltrs)
		return (free_ast(ast), NULL);
	s->ast->type = CMD;
	if (s->ast->n_args)
		s->ast->space_args[s->ast->n_args - 1] = NO_SPACE_AFTER;
	*str += s->n_cmd_ltrs;
	return (s->ast);
}
