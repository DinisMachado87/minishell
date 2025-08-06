/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 15:30:31 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1

# include "../unity/src/unity.h"
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <stddef.h>

typedef struct s_ast
{
	int				type;
	int				subtype_maybe;
	char			**args;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct	s_state_ext_cmd
{
	char	*str;
	int 	i_word;
	int		error;
}				t_s_ext_cmd;

int	make_node(t_ast **ast);
int	extract_cmd(t_ast *ast, char *str);
void	prompt_loop(void);

#endif
