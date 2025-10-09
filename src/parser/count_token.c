/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:52:37 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/09 23:24:53 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env_tkn(char *str, t_token *cur)
{
	int	i_ltr;

	i_ltr = 1;
	if (!str[i_ltr] || is_space(str[i_ltr])
		|| str[i_ltr] == '\"' || str[i_ltr] == '\'' || str[i_ltr] == '\(')
		return (i_ltr);
	cur->expand = EXPAND;
	if (str[i_ltr] == '?')
		return (++i_ltr);
	while (str[i_ltr] && is_alphanum_or_underscore(str[i_ltr]))
		i_ltr++;
	return (i_ltr);
}

int	count_single_quote_tkn(char *str)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr] && str[i_ltr] != '\'')
		i_ltr++;
	return (i_ltr);
}

int	count_double_quote_tkn(char *str)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr] && str[i_ltr] != '\"')
	{
		if (str[i_ltr] == '$' && !(!str[i_ltr + 1] || is_space(str[i_ltr + 1])
				|| str[i_ltr + 1] == '\"'))
			break ;
		i_ltr++;
	}
	return (i_ltr);
}

int	count_base_tkn(char *str)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr] && type(str + i_ltr) == CMD && !is_space(str[i_ltr]))
	{
		if (str[i_ltr] == '\'' || str[i_ltr] == '\"'
			|| str[i_ltr] == '\(' || str[i_ltr] == '$')
			break ;
		i_ltr++;
	}
	return (i_ltr);
}

int	count_token(char *str, t_token *cur, t_token *nxt)
{
	int	i_ltr;

	i_ltr = 0;
	if (*str == '\'' || *str == '\"')
	{
		if (cur->limiter == ' ')
			cur->limiter = str[i_ltr++];
		else if (cur->limiter == *str)
		{
			cur->limiter = ' ';
			return (++i_ltr);
		}
	}
	if (cur->limiter == '\'')
		i_ltr += count_single_quote_tkn(str + i_ltr);
	else if (str[i_ltr] == '$' && cur->limiter != '\'')
		i_ltr += count_env_tkn(str + i_ltr, cur);
	else if (cur->limiter == '\"')
		i_ltr += count_double_quote_tkn(str + i_ltr);
	else
		i_ltr += count_base_tkn(str + i_ltr);
	handle_end_quote(str, cur, nxt, &i_ltr);
	return (i_ltr);
}
