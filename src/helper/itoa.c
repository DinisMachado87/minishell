/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:25:19 by jlind             #+#    #+#             */
/*   Updated: 2025/08/22 13:25:20 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_digits(int num)
{
	int	len;

	len = 0;
	while (num)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*itoa(int num)
{
	int		digits;
	char	*str;

	if (num < INT_MIN)
		return (NULL);
	else if (num > INT_MAX)
		return (NULL);
	else if (num == 0)
		return ("0");
	digits = get_digits(num);
	str = malloc(sizeof(char) * digits + 1);
	str[digits] = '\0';
	while (num)
	{
		str[--digits] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}
