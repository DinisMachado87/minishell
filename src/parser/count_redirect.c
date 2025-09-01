/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:21:37 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/29 14:38:22 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	count_redirect(char **str)
{
	t_s_token	tk;

	tk.limiter = ' ';
	tk.space_after = SPACE_AFTER;
	tk.str = str;

	if (!(tk.str && REDIRECT == c->type)
		return (0);
	if (skip_red_sign_and_spaces(&tk, c->subtype) == ERROR)
		return (0);
	c->n_red_tk[c->subtype] = 0;
	while (*tk.str && *tk.str != ' ' && CMD == type(tk.str + c->ltr))
	{
		tk.str += count_token(tk.str, &tk, &tk);
		c->n_red_tk[c->subtype]++;
	}
}
