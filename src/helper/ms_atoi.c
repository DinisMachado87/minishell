/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:59:22 by jlind             #+#    #+#             */
/*   Updated: 2025/09/11 19:33:37 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ms_atoi(char *nbr)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	if (*nbr == '-')
	{
		sign = -1;
		nbr++;
	}
	while (*nbr == '+')
		nbr++;
	while (*nbr && (*nbr >= '0' && *nbr <= '9'))
	{
		if (sign == 1 && (res > (INT_MAX - (*nbr - '0')) / 10))
			return (0);
		else if (sign == -1 && (res > (-(long)INT_MIN - (*nbr - '0')) / 10))
			return (0);
		res *= 10;
		res += *nbr - '0';
		nbr++;
	}
	return (res * sign);
}
