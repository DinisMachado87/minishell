/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ext_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:33:50 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/25 17:53:05 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

const char *type_str[] = {
	"CMD",
	"PIPE",
	"REDIRECT_OUT",
	"REDIRECT_IN",
	"SUBSHELL",
	"OR",
	"AND",
	"APPEND",
	"HEREDOC"
};

void	print_ast(t_ast *ast, char *testname)
{
	int	i;

	i = 0;
	if (testname)
		printf("== %s ==\n\n", testname);
	printf("TYPE:\t%s\n", type_str[ast->type]);
	if (ast->args)
	{
		printf("ARGS:");
		while (ast && ast->args[i])
		{
			printf("\t%s\n", ast->args[i]);
			i++;
		}
	}
	if (ast->n_args)
		printf("N_ARGS:\t%d\n", ast->n_args);
	if (ast->next)
		printf("TYPE:\t%s\n", type_str[ast->next->type]);
	if (ast->left)
		printf("TYPE:\t%s\n", type_str[ast->left->type]);
	if (ast->right)
		printf("TYPE:\t%s\n", type_str[ast->right->type]);
	printf("\n");
}

int	main()
{
	t_ast	*ast;
	t_s_parser s;
	char	*str_cmd = "echo 'the city \"is wilder\" than you think'\"'and think' better\"    ";
	char	*str_par = "(echo \"I am them\")";
	// char	*str_input = "echo 'the city \"is wilder\" than you think' (ls) \"(ls)\" \"'and think' better\"";

	ast = extract_cmd(&str_cmd, &s);
	print_ast(ast, "Extract Cmd");
	free_all(ast);
	ast = extract_subshell(&str_par);
	print_ast(ast, "Extract Subshell");
	free_all(ast);
	// ast = extract_cmd(&str_cmd, &s);
	// print_ast(ast);
	// free_all(ast);
	return (0);
}
