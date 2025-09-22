/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkns_to_words.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:37:13 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/18 17:53:04 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_and_reassign(char **dest, char **src) 
{
	free_and_null((void **)dest);
	*dest = *src;
	*src = NULL;
	return (1);
}

int	tkns_to_words(t_args *args, int alloc_len)
{
	int	i;
	int	new_i;

	i = 0;
	new_i = 0;
	while (i < args->n)
	{
		int		interval;

		interval = 0;
		while (i + interval < args->n && (args->tkns[i + interval][0] == '\0'
			|| args->space[i + interval] == NO_SPACE_AFTER))
			interval++;
		if (++interval == 1 && i != new_i)
			free_and_reassign(&args->tkns[new_i], &args->tkns[i]);
		else if (interval > 1)
			if (!cat_str_arr(&args->tkns[new_i], args->tkns + i, interval))
				return (0);
		new_i++;
		i += interval;
	}
	args->n = new_i;
	while (new_i < alloc_len)
		free_and_null((void **)&args->tkns[new_i++]);
	return (1);
}
