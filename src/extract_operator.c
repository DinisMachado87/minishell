#include "../include/minishell.h"

t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator)
{
	if (!make_node(ast_nd))
		return (perror("ERROR allocating node "
				 "while extracting subshell"), NULL);
	(*ast_nd)->type = operator;
	if (operator == PIPE
		|| operator == REDIRECT_IN
		|| operator == REDIRECT_OUT)
		(*str)++;
	*str += 2;
	return (*ast_nd);
}
