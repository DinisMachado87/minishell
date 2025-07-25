#include "../include/minishell.h"
#include <stdio.h>

t_ast	*extract_subshell(char **str)
{
	int		i_ltr;
	char	*substr;
	t_ast	*ast_nd;

	i_ltr = 0;
	(*str)++;
	while ((*str)[i_ltr] && (*str)[i_ltr] != ')')
		i_ltr++;
	if ((*str)[i_ltr] && (*str)[i_ltr] != ')')
		return (perror("ERROR: Missing ')'"), NULL);
	substr = ms_strcpy(*str, i_ltr);
	if (!substr)
		return (NULL);
	if (!make_node(&ast_nd))
		return (perror("ERROR allocating node "
				 "while extracting subshell"), NULL);
	if (!allocate_ast_args(ast_nd, 1))
		return (0);
	ast_nd->args[0] = substr;
	ast_nd->type = SUBSHELL;
	*str += i_ltr;
	return (ast_nd);
}

