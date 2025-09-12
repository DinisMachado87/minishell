/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:57:40 by jlind             #+#    #+#             */
/*   Updated: 2025/09/12 09:55:46 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ms_char_isalpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	ms_isalpha(char *str)
{
	if (!str || !*str)
		return (1);
	while (*str && ms_char_isalpha(*str))
		str++;
	if (!*str)
		return (1);
	return (0);
}
