/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:52:37 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/02 01:02:52 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env_tkn(char *str, t_token *cur, t_token *nxt)
{
	int		i_ltr;

	i_ltr = 1;
	if (*str == '"')
		i_ltr++;
	if (!str[i_ltr] || str[i_ltr] == ' ' || str[i_ltr] == '?')
		i_ltr++;
	else
		while (str[i_ltr]
			&& is_alphanum_or_underscore(str[i_ltr]))
			i_ltr++;
	if (str[i_ltr] && str[i_ltr] == cur->limiter && str[i_ltr] == '\"')
	{
		nxt->limiter = ' ';
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] != ' ')
		cur->space_after = NO_SPACE_AFTER;
	return (i_ltr);
}

int	count_single_quote_tkn(char *str, t_token *cur, t_token *nxt)
{
	int		i_ltr;

	i_ltr = 0;
	if (*str == '\'')
		i_ltr++;
	while (str[i_ltr] && str[i_ltr] != '\'')
		i_ltr++;
	if (str[i_ltr] == '\'')
	{
		nxt->limiter = ' ';
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] != ' ')
		cur->space_after = NO_SPACE_AFTER;
	return (i_ltr);
}

int	count_double_quote_tkn(char *str, t_token *cur, t_token *nxt)
{
	int		i_ltr;

	i_ltr = 0;
	if (*str == '\"')
		i_ltr++;
	while (str[i_ltr] && str[i_ltr] != '\"')
	{
		if (str[i_ltr] == '$')
		{
			if (i_ltr == 1)
				return (count_env_tkn(str, cur, nxt));
			else
				break;
		}
		i_ltr++;
	}
	if (str[i_ltr] == '\"')
	{
		nxt->limiter = ' ';
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] != ' ')
		cur->space_after = NO_SPACE_AFTER;
	return (i_ltr);
}

int	count_base_tkn(char *str, t_token *cur, t_token *nxt)
{
	int		i_ltr;

	i_ltr = 0;
	while (str[i_ltr] && type(str + i_ltr) == CMD
		&& str[i_ltr] != cur->limiter)
	{
		if (str[i_ltr] == '\'' || str[i_ltr] == '\"')
		{
			nxt->limiter = str[i_ltr];
			break;
		}
		if (str[i_ltr] == '$')
			break;
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] != ' ')
		cur->space_after = NO_SPACE_AFTER;
	return (i_ltr);
}

int	count_token(char *str, t_token *cur, t_token *nxt)
{
	if (*str == '$'
		|| (str[1] && str[1] == '$'
			&& *str == '\"'))
		return (count_env_tkn(str, cur, nxt));
	if (cur->limiter == '\''
		|| (cur->limiter == ' ' && *str == '\''))
		return (count_single_quote_tkn(str, cur, nxt));
	if (cur->limiter == '\"'
		|| (cur->limiter == ' ' && *str == '\"'))
		return (count_double_quote_tkn(str, cur, nxt));
	else
		return (count_base_tkn(str, cur, nxt));
}
