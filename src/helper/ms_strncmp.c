/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:47:55 by jlind             #+#    #+#             */
/*   Updated: 2025/09/14 14:12:50 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ms_strncmp(char *s1, char *s2, int len)
{
	int	pos;

	pos = 0;
	while ((pos < len) && s1 && s2 && (s1[pos] == s2[pos]) && s1[pos + 1])
		pos++;
	return (s1[pos] - s2[pos]);
}
