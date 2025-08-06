/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure_ast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:56 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:47:42 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	place_between_higher_and_lower_precedence(t_ast *cur_list, t_ast *cur_right)
{
	t_ast *last_bigger;

	last_bigger = NULL;
	while (cur_list->type < cur_right->type)
	{
		if (!cur_right->right)
		{
			cur_right->right = cur_list;
			return ;
		}
		last_bigger = cur_right;
		cur_right = cur_right->right;
	}
	cur_list->left = cur_right;
	last_bigger->right = cur_list;
}

t_ast	*structure_ast(t_ast *cur_list)
{
	t_ast *ast_head;
		
	ast_head = cur_list;
	cur_list = cur_list->next;
	while (cur_list)
	{
		if (cur_list->type < ast_head->type)
			place_between_higher_and_lower_precedence(cur_list, ast_head);
		else
		{
			cur_list->left = ast_head;
			ast_head = cur_list;
		}
		cur_list = cur_list->next;
	}
	return (ast_head);
}
