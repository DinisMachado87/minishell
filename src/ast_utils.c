#include "../include/minishell.h"
#include <strings.h>

t_ast	*free_all(t_ast *ast_head)
{
	t_ast *next_head;
	
	if (ast_head)
		next_head = ast_head->next;
	while (ast_head)
	{
		while (ast_head && ast_head->args && --ast_head->n_args >= 0)
			free(ast_head->args[ast_head->n_args]);
		if (ast_head && ast_head->args)
			free(ast_head->args);
		if (ast_head)
			free(ast_head);
		ast_head = next_head;
		if (ast_head)
			next_head = ast_head->next;
	}
	return (NULL);
}

t_ast	*make_node(t_ast **ast)
{
	t_ast	*ast_cpy;
	t_ast	*new_node;

	ast_cpy = NULL;
	new_node = malloc(sizeof(t_ast));
	if (!new_node)
	{
		*ast = free_all(*ast);
		return (NULL);
	}
	bzero(new_node, sizeof(t_ast));
	if (!(*ast))
		*ast = new_node;
	else
	{
		ast_cpy = *ast;
		while (ast_cpy->next)
			ast_cpy = ast_cpy->next;
		ast_cpy->next = new_node;
	}
	return (new_node);
}
