/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_cmd_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 23:19:15 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 02:22:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	chr_after_spaces(t_token *tk)
{
		while (*tk->str && *tk->str == ' ')
			tk->str++;
		if (!*tk->str)
			return (0);
		return (1);
}

int	skip_redirect(t_token *cur, t_cmd *c)
{
	c->subtype = subtype(cur->str);
	if (!skip_red_sign_and_spaces(cur, c->subtype))
		return (0);
	while (*cur->str && *cur->str != ' ' && CMD == type(cur->str))
		cur->str += count_token(cur->str, cur, cur);
	return (1);
}

int	count_redirect(t_token cur, t_cmd *c, t_args *args)
{
	args->n = 0;
	while (*cur.str && *cur.str != ' ' && CMD == type(cur.str))
	{
		cur.str += count_token(cur.str, &cur, &cur);
		args->n += 1;
	}
	return (1);
}

int	count_cmd_tokens(t_token cur, t_cmd *c)
{
	while (chr_after_spaces(&cur))
	{
		cur.space_after = SPACE_AFTER;
		c->type = type(cur.str);
		if (cur.str && CMD == c->type)
		{
			cur.str += count_token(cur.str, &cur, &cur);
			c->n_cmd_tk++;
		}
		else if (cur.str && REDIRECT == c->type)
			skip_redirect(&cur, c);
		else
			break;
	}
	return(1);
}
