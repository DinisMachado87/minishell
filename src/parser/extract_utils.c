/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:48 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/17 20:50:40 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <strings.h>

void	*safe_alloc_zero(void **ptr, size_t size)
{
	*ptr = malloc(size);
	if (!*ptr)
		return(NULL);
	ms_bzero(*ptr, size);
	return (*ptr);
}

int	allocate_ast_args(t_args *args, int n_strs)
{
	if (!safe_alloc_zero((void **)&args->tkns,
					  (n_strs + 1) * sizeof(char *))
		|| !safe_alloc_zero((void **)&args->exp,
					  (n_strs + 1) * sizeof(int))
		|| !safe_alloc_zero((void **)&args->space,
					  (n_strs + 1) * sizeof(int))
		|| !safe_alloc_zero((void **)&args->type,
					  (n_strs + 1) * sizeof(int)))
		return (perror("Err allocating args array"), 0);
	args->n = n_strs;
	return (1);
}
