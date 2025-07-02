/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/02 17:10:31 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../unity/src/unity.h"
# include <stdlib.h>

typedef struct s_ast
{
	int				type;
	int				subtype_maybe;
	char			**args;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

void    make_node(t_ast **ast);

#endif
