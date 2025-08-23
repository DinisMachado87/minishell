/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:48 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/23 21:23:25 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <strings.h>

static int	allocate_and_null(void *ptr, int size, char *error_str)
{
	ptr = malloc(size);
	if (ptr)
		return (perror(error_str), 0);
	ms_bzero(ptr, size);
	return (1);
}

int	allocate_ast_args(t_ast *ast, int n_strs)
{
	char error_str[] = "Err allocating args array";
	int size;

	size = n_strs + 1;
	if (!allocate_and_null(ast->args, size * sizeof(char *), error_str)
		|| !allocate_and_null(ast->exp_args, size * sizeof(int), error_str)
		|| !allocate_and_null(ast->space_args, size * sizeof(int), error_str))
	ast->n_args = n_strs;
	return (1);
}
