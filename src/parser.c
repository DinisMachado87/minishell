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

// @make_node creates a node for a new expression and set it as next of the previous or ast head if the first

// @tokenize stores str_compares extracted word into token and stores token
int	tokenize_and_save_vars(t_ast **ast)
{
	return(0);
}
// @compose_node calls the right callback function to extract and store remaining arguments according to tokenized cmd
int	compose_node(t_ast **ast, char **str)
{
	return(0);
}

int expand_vars(t_ast **ast, char **str)
{
	return(0);
}


// @Returns the ast still as a list and a pointer by reference on the current letter not split yet
int	parser_core(char **str, t_ast **ast)
{
	make_node(ast);
	extract_cmd(*ast, *str);
	tokenize_and_save_vars(ast);
	compose_node(ast, str);
	expand_vars(ast, str);
	return (0);
}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char *cur_ltr)
{
	t_ast *ast_list;
	t_ast *ast_tree;

	ast_list = 0;
	ast_tree = 0;
	while (*cur_ltr)
		parser_core(&cur_ltr, &ast_list);
	//make_tree(&ast_list, &ast_tree);
	return (ast_tree);
}
