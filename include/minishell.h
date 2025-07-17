/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/07 15:25:01 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1

# include "../unity/src/unity.h"
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>

typedef enum {
	CMD,
	PIPE,
	REDIRECT_OUT,
	REDIRECT_IN,
	SUBSHELL,
	OR,
	AND,
	APPEND,
	HEREDOC,
} t_type;

typedef struct s_ast
{
	int				type;
	int				subtype_maybe;
	char			**args;
	int				n_args;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct	s_state_parser
{
	char	*str;
	int		needle;
	int 	i_word;
	int		error;
}				t_s_parser;

// ast_utils
t_ast	*make_node(t_ast **ast);
t_ast	*free_all(t_ast *ast);
// parser_utils
int		is_operator(char *str);
char	*ms_strcpy(char *str, int len);
// gen_utils
void	ms_bzero(void *s, size_t n);
// extract cmd
t_ast	*extract_cmd(char **str, t_s_parser *s);
// int		split_parser(t_ast *ast, char *str, int *error);

#endif
