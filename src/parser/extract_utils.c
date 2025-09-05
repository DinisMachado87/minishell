/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:48 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 15:46:49 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	*safe_alloc_zero(void **ptr, size_t size)
{
	*ptr = malloc(size);
	if (!*ptr)
		return(NULL);
	ms_bzero(*ptr, size);
	return (*ptr);
}

int	allocate_ast_args(t_ast *ast, int n_strs)
{
	if (!safe_alloc_zero((void **)&ast->args,
					  (n_strs + 1) * sizeof(char *))
		|| !safe_alloc_zero((void **)&ast->exp_args,
					  (n_strs + 1) * sizeof(int))
		|| !safe_alloc_zero((void **)&ast->space_args,
					  (n_strs + 1) * sizeof(int)))
		return (perror("Err allocating args array"), 0);
	ast->n_args = n_strs;
	return (1);
}

int	allocate_red_args(t_ast *ast, int n_strs, int subtype)
{
	safe_alloc_zero((void **)&ast->pre_r_args[subtype],
			(n_strs + 1) * sizeof(char *));
	safe_alloc_zero((void **)&ast->r_exp_args[subtype],
			(n_strs + 1) * sizeof(int));
		return (perror("Err allocating redirect args array"), 0);
	return (1);
}
