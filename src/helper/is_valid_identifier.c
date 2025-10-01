/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_identifier.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:23:14 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 09:07:24 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(char *str)
{
	if (!str || !*str)
		return (0);
	if (ms_char_isalpha(*str) || *str == '_')
		str++;
	else
		return (0);
	while (*str && (ms_char_isalpha(*str)
			|| ms_char_isdigit(*str) || (*str == '_')))
		str++;
	if (!*str)
		return (1);
	return (0);
}
