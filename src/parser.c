/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/03 13:37:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//[NOTE:]
// @make_node creates a node for a new expression and set it as next of the previous or ast head if the first
// @extract extracts the first word of the input string and stores it as the first args in the s_ast node
// @tokenize stores str_compares extracted word into token and stores token
// @compose_node calls the right callback function to extract and store remaining arguments according to tokenized cmd
// @Returns the ast still as a list and a pointer by reference on the current letter not split yet
int	parser(char **str, t_ast *ast)
{
	make_node(ast, str);
	extract(ast);
	tokenize_and_save_vars(ast);
	compose_node(ast, str);
	expand_vars();
	return (0);
}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char **str)
{
	(void)str;
	t_ast *ast;

	cur_ltr = str;
	ast_list = 0;
	ast_tree = 0;
	while (*cur_ltr)
		parser(&ast_list, &cur_ltr);
	make_tree(&ast_list, &ast_tree);
	return (0);
}
