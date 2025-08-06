#include "../include/minishell.h"

t_ast	*structure_ast(t_ast *cur_list)
{
	t_ast *ast_head;
		
	ast_head = cur_list;
	while (cur_list)
	{
		cur_list = cur_list->next;
		if (cur_list->type < ast_head->type)
			ast_head->right = cur_list;
		else
		{
			cur_list->left = ast_head;
			ast_head = cur_list;
		}
	}
	return (ast_head);
}
