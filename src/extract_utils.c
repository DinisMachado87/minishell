#include "../include/minishell.h"

int	allocate_ast_args(t_ast *ast, int n_strs)
{
	ast->args = malloc((n_strs + 1) * sizeof(char *));
	if (!ast->args)
		return (perror("Err allocating args array"), 0);
	ast->args[n_strs] = 0;
	ast->n_args = n_strs;
	return (1);
}
