/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens_wip.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 22:20:56 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/29 00:14:22 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_allocations(char *str)
{
	t_s_token cur;
	t_s_token nxt;
	int	n_tkns;
	int	r_tkns[2];
	int	i_ltr;
	int	subtype;

	n_tkns = 0;
	r_tkns[IN] = 0;
	r_tkns[OUT] = 0;
	i_ltr = 0;
	subtype = 0;

	cur.str = str;
	cur.limiter = ' ';
	cur.space_after = SPACE_AFTER;

	while (*cur.str)
	{
		while (*cur.str && *cur.str == ' ')
			cur.str++;
		nxt.limiter = cur.limiter;
		nxt.space_after = SPACE_AFTER;
		if (cur.str[i_ltr] && CMD == type(cur.str + i_ltr))
		{
			i_ltr = count_token(cur.str, &cur, &nxt);
			cur.str += i_ltr;
			n_tkns++;
		}
		else if (cur.str[i_ltr] && REDIRECT == type(cur.str + i_ltr))
		{
			skip_red_sign_and_spaces(cur.str, &subtype);
			while (cur.str[i_ltr] != ' ')
			{
				i_ltr = count_token(cur.str, &cur, &nxt);
				cur.str += i_ltr;
				r_tkns[subtype]++;
			}
		}
		else
			break;
	}
}

// int extract_cmd_recursive(t_s_token cur, t_s_parser *s)
// {
// 	t_s_token	nxt;
// 	int			i_ltr;
//
// 	i_ltr = 0;
// 	skip_spaces(&cur, &s->n_cmd_ltrs);
// 	if (cur.str[i_ltr] && REDIRECT == type(cur.str + i_ltr))
// 		if (extract_redirect(&i_ltr, cur, s) == ERROR)
// 			return (ERROR);
// 	nxt.limiter = cur.limiter;
// 	cur.space_after = SPACE_AFTER;
// 	s->i_word++;
// 	if (cur.str[i_ltr] && CMD == type(cur.str + i_ltr))
// 		i_ltr += count_token(cur.str, &cur, &nxt);
// 	nxt.str = cur.str + i_ltr;
// 	s->n_cmd_ltrs += i_ltr;
// 	if (cur.str[i_ltr] && extract_cmd_recursive(nxt, s) == ERROR)
// 		return (ERROR);
// 	if (!s->ast->args && !allocate_ast_args(s->ast, s->i_word))
// 		return (ERROR);
// 	s->ast->args[--s->i_word] = ms_strcpy(cur.str, i_ltr);
// 	if (!s->ast->args[s->i_word])
// 		return (ERROR);
// 	s->ast->space_args[s->i_word] = cur.space_after;
// 	s->ast->exp_args[s->i_word] = cur.limiter != '\'' && *cur.str == '$';
// 	return(i_ltr);
// }
