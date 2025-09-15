/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 22:59:53 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/29 02:06:58 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_red_args(t_s_token cur, int r_subtype)
{
	t_s_token	nxt;
	int			i_ltr;

	nxt.limiter = cur.limiter;
	cur.space_after = SPACE_AFTER;
	i_ltr = count_token(cur.str, &cur, &nxt);
	nxt.str = cur.str + i_ltr;
	if (cur.space_after == NO_SPACE_AFTER && nxt.str)
		return (1 + count_red_args(nxt, r_subtype));
	return(1);
}

int	count_redirect(t_s_token cur, t_count *c)
{
	int i_ltr;
	int	r_subtype;

	r_subtype = subtype(cur.str);
	i_ltr = skip_red_sign_and_spaces(cur.str, &r_subtype);
	if (i_ltr == ERROR)
		return (perror("Error: No file after redirect"), ERROR);
	if (r_subtype == APPEND)
		r_subtype = OUT;
	else if (r_subtype == HEREDOC)
		r_subtype = IN;
	cur.str += i_ltr;
	c->n_red_tk[r_subtype] = 0;
	c->n_red_tk[r_subtype] = count_red_args(cur, r_subtype);
	return(c->n_red_tk[r_subtype]);
}
