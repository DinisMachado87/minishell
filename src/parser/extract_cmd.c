/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 16:07:37 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int extract_token(t_token *cur, t_cmd *c, t_ast *ast)
{
	int		len;
	t_token	nxt;

	nxt.limiter = cur->limiter;
	cur->space_after = SPACE_AFTER;
	len = count_token(cur->str, cur, &nxt);
	nxt.str = cur->str + len;
	ast->args[c->i_tkn] = ms_strcpy(cur->str, len);
	if (!ast->args[c->i_tkn])
		return (0);
	ast->space_args[c->i_tkn] = cur->space_after;
	if (*cur->str == '$'
		|| (*cur->str == '\"' && cur->str[1] == '$'))
		ast->exp_args[c->i_tkn] = EXPAND;
	c->i_tkn++;
	*cur = nxt;
	return(len);
}

static int	extract_cmd_core(t_token *cur, t_cmd *c, t_parser *s)
{
	while(chr_after_spaces(cur))
	{
		while (*cur->str && *cur->str == ' ')
			cur->str++;
		c->type = type(cur->str);
		if (*cur->str && REDIRECT == c->type)
		{
			if (!extract_redirect(cur, c, s))
				return (0);
		}
		else if (cur->str && CMD == type(cur->str))
		{
			if (!extract_token(cur, c, s->ast))
				return (0);
		}
		else
			break;
	}
	return (1);
}

int	extract_cmd(char **str, t_parser *s)
{
	t_token	cur;
	t_cmd	c;

	bzero((void *)&c, sizeof(t_cmd));

	cur.limiter = ' ';
	cur.space_after = SPACE_AFTER;
	cur.str = *str;

	count_cmd_tokens(cur, &c);

	if (!make_node(&s->ast)
		|| !allocate_ast_args(s->ast, c.n_cmd_tk))
		return (free_ast(&s->ast), 0);
	
	s->ast->type = CMD;
	
	if (!extract_cmd_core(&cur, &c, s))
		return (free_ast(&s->ast), 0);

	if (s->ast->n_args)
		s->ast->space_args[s->ast->n_args - 1] = SPACE_AFTER;

	*str = cur.str;

	return (1);
}
