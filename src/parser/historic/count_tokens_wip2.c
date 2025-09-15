/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens_wip2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 23:19:15 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/29 02:29:49 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	chr_after_spaces(t_s_token *tk)
{
	while (*tk->str && *tk->str == ' ')
		tk->str++;
	if (!*tk->str)
		return (0);
	return (1);
}

int	count_cmd_rec(t_s_token tk, t_count *c)
{
	while (chr_after_spaces(&tk))
	{
		tk.space_after = SPACE_AFTER;
		c->type = type(tk.str);
		if (tk.str && CMD == c->type)
		{
			tk.str += count_token(tk.str, &tk, &tk);
			c->n_cmd_tk++;
		}
		else if (tk.str && REDIRECT == c->type)
		{
			tk.str += skip_red_sign_and_spaces(tk.str, &c->subtype);
			c->n_red_tk[c->subtype] = 0;
			while (*tk.str && *tk.str != ' ' && CMD == type(tk.str + c->ltr))
			{
				tk.str += count_token(tk.str, &tk, &tk);
				c->n_red_tk[c->subtype]++;
			}
			if (!c->n_red_tk[c->subtype])
				return (perror("Error: no file after redirection"), 0);
		}
		else
			break;
	}
	return(1);
}

t_count count_cmd_tokens(char *str)
{
	t_s_token	init;
	t_count		c;

	c.n_cmd_tk = 0;
	c.n_red_tk[IN] = 0;
	c.n_red_tk[OUT] = 0;
	c.type = CMD;
	c.subtype = 0;

	init.limiter = ' ';
	init.space_after = SPACE_AFTER;
	init.str = str;
	if (!count_cmd_rec(init, &c))
	{
		c.n_cmd_tk = 0;
		c.n_red_tk[IN] = 0;
		c.n_red_tk[OUT] = 0;
		return (c);
	}
	return (c);
}
