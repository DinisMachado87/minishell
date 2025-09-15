/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   experiment_restructure_red_tkns.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 01:44:23 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 01:53:13 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


typedef struct	s_tokens_arr
{
	char	**tkns;
	int		*exp;
	int		*space;
	int		n;
}	t_tkns_arr;

typedef struct s_ast
{
    int             type;
    int             subtype;
    t_tkns_arr		args;
    int             append;
    int             heredoc;
    t_tkns_arr		*red[2];
    struct s_ast    *next;
    struct s_ast    *left;
    struct s_ast    *right;
}					t_ast;

static int	expand_cat_red_tkns(t_tkns_arr *red, int alloc_len, t_env *env_head)
{
	if (!expand_tkn_arr(red->tkns, red->exp, red->n, env_head)
		|| !tkns_to_words(red->tkns, NULL, alloc_len))
		return (0);
	return (1);
}

