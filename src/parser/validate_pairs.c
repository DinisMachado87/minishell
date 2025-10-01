/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_pairs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 10:58:55 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/01 19:40:32 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	str_pairs_even(char *str)
{
	int	single_quotes;
	int	double_quotes;
	int	parens;

	single_quotes = 0;
	double_quotes = 0;
	parens = 0;
	while (*str)
	{
		if (*str == '\'' && !double_quotes)
			single_quotes = !single_quotes;
		else if (*str == '\"' && !single_quotes)
			double_quotes = !double_quotes;
		else if (*str == '(' && !single_quotes && !double_quotes)
			parens++;
		else if (*str == ')' && !single_quotes && !double_quotes)
			parens--;
		if (parens < 0)
			return (printf("syntax error near unexpected token ')'\n"), ERROR);
		str++;
	}
	return (!single_quotes && !double_quotes && !parens);
}
