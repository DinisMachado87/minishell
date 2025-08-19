/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_normalizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:22:10 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/18 22:11:51 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

int	main()
{
	char	*str;
	int		i_test;
	const	char	*test_strs[] = {
		"cat<file.txt|cat&&ls    -l&&export env=car&&echo hello'$env'\"$env\"&&echo hello '$env' \"$env\"||echo end",
		0
	};

	str = NULL;
	i_test = 0;
	printf("== Normalizer ==\n");
	while (test_strs[i_test])
	{
		str = ms_strcpy((char *)*test_strs, ms_strlen((char *)*test_strs));
		printf("before: %s\n", str);
		normalizer(&str);
		printf("after : %s\n\n", str);
		i_test++;
	}
	if (str)
		free(str);
	return (0);
}
