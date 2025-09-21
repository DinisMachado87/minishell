/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:35:58 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/18 17:18:32 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int extract_token(t_token *cur, t_cmd *c, t_args *args)
{
	int		len;
	t_token	nxt;

	cur->space_after = NO_SPACE_AFTER;
	cur->expand = DONT_EXPAND;
	len = count_token(cur->str, cur, &nxt);
	nxt.str = cur->str + len;
	args->tkns[c->i_tkn] = ms_strcpy(cur->str, len);
	if (!args->tkns[c->i_tkn])
		return (0);
	args->space[c->i_tkn] = cur->space_after;
	args->exp[c->i_tkn] = cur->expand;
	c->i_tkn++;
	*cur = nxt;
	return(len);
}

int	extract_cmd_core(t_token *cur, t_cmd *c, t_ast *ast)
{
	while(cur->limiter != ' ' || chr_after_spaces(cur))
	{
		c->type = type(cur->str);
		if (*cur->str && REDIRECT == c->type)
		{
			int	sub_i;

			c->subtype = subtype(cur->str);
			sub_i = c->subtype;
			if (sub_i > OUT)
				sub_i -= 2;
			if (!extract_redirect(cur, c, &ast->args[sub_i], ast->args[sub_i]))
				return (0);
		}
		else if (cur->str && CMD == type(cur->str))
		{
			if (!extract_token(cur, c, &ast->args[0]))
				return (0);
		}
		else
			break;
	}
	return (1);
}

int	extract_cmd(char **str, t_ast **lst_nd)
{
	t_token	cur;
	t_cmd	c;

	bzero((void *)&c, sizeof(t_cmd));
	cur.limiter = ' ';
	cur.str = *str;
	count_cmd_tokens(cur, &c);
	if (!make_node(lst_nd)
		|| !allocate_ast_args(&(*lst_nd)->args[0], c.n_cmd_tk))
		return (0);
	(*lst_nd)->type = CMD;
	if (!extract_cmd_core(&cur, &c, *lst_nd))
		return (0);
	if ((*lst_nd)->args[0].n)
		(*lst_nd)->args[0].space[(*lst_nd)->args[0].n - 1] = SPACE_AFTER;
	*str = cur.str;
	return (1);
}
