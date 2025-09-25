/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:25:19 by jlind             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/24 11:50:52 by dimachad         ###   ########.fr       */
=======
/*   Updated: 2025/09/24 22:20:55 by jlind            ###   ########.fr       */
>>>>>>> 91141d7 (MOD fixed itoa leak)
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_digits(int num)
{
	int digits;

	digits = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		num *= -1;
		digits++;
	}
	while (num)
	{
		num /= 10;
		digits++;
	}
	return (digits);
}

char	*itoa(int num)
{
	char	*arg;
	int		digits;

	if (!num)
	{
		arg = ms_strndup("0", 2);
		if (!arg)
			return (NULL);
		return (arg);
	}
	digits = get_digits(num);
	arg = (char *)malloc(digits + 1);
	if (!arg)
		return (NULL);
	arg[digits] = '\0';
	digits--;
	if (num < 0)
	{
		arg[0] = '-';
		num *= -1;
	}
	while (num)
	{
		arg[digits--] = (num % 10) + '0';
		num /= 10;
	}
	return (arg);
}
