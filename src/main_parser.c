/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:33:50 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/30 01:39:47 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	t_ast	*ast;
	t_s_parser s;
	char	*str_cmd = "echo <file.txt 'the city<file.txt\"is wilder\" than you think'\"'and think' better\"    ";
	char	*str_par = "(echo \"I am them\")";
	char	*str_pipe = "|";
	char	*str_input = "1echo 'the city \"is wilder\" than you think' (ls) \"(ls)\" \"'and think' better\"   ";

	ast = NULL;
	extract_cmd(&ast, &str_cmd, &s);
	print_nd_list(ast, "Extract Cmd");
	ast = free_all(ast);
	extract_subshell(&ast, &str_par);
	print_nd_list(ast, "Extract Subshell");
	ast = free_all(ast);
	extract_operator(&ast, &str_pipe, PIPE);
	print_nd_list(ast, "Extract operator");
	ast = free_all(ast);
	ast = parser(str_input);
	print_ast(ast, "Parser");
	ast = free_all(ast);
	return (0);
}
