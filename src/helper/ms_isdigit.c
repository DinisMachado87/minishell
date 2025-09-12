/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 08:28:42 by jlind             #+#    #+#             */
/*   Updated: 2025/09/12 09:51:39 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ms_char_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ms_isdigit(char *str)
{
	if (!str || !*str)
		return (1);
	while (*str && (ms_char_isdigit(*str) || (*str == '-') || (*str == '+')))
		str++;
	if (!*str)
		return (1);
	return (0);
}
