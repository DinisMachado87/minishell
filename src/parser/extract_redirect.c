/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:56:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/28 01:50:54 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

static void	convert_append_heredoc_to_in_out(t_ast *ast, int *r_subtype)
{
	ast->append = 0;
	if (*r_subtype == APPEND)
	{
		ast->append = 1;
		*r_subtype = OUT;
	}
	else if (*r_subtype == HEREDOC)
	{
		ast->heredoc = 1;
		*r_subtype = IN;
	}
	else if (*r_subtype == OUT)
		ast->append = 0;
	else if (*r_subtype == IN)
		ast->heredoc = 0;
}

void	free_red_args(t_ast *ast, int subtype)
{
	if (subtype == IN && ast->red_args[IN]
		&& !ms_strcmp((char *)TEMP_PREFIX, ast->red_args[IN]))
	{
		unlink(ast->red_args[IN]);
		ast->red_args[IN] = NULL;
	}
	else if (ast->red_args[subtype])
	{
		free(ast->red_args[subtype]);
		ast->red_args[subtype] = NULL;
	}
}

int	skip_red_sign_and_spaces(char *str, int *subtype)
{
	int i_ltr;

	i_ltr = 0;
	if (*subtype == APPEND || *subtype == HEREDOC)
		i_ltr += 2;
	else
		i_ltr++;
	while (str[i_ltr] && str[i_ltr] == ' ')
		i_ltr++;
	if (!str[i_ltr] || type(str + i_ltr) != CMD)
		return (ERROR);
	return (i_ltr);
}

int	free_and_null_red_args(t_ast *ast, int subtype)
{
	int i;

	i = 0;
	if (ast->heredoc)
		unlink(ast->red_args[IN]);
	if (ast->red_args[subtype])
		free(ast->red_args[subtype]);
	ast->red_args[subtype] = NULL;

	while (ast->pre_red_args[subtype] && ast->pre_red_args[subtype][i])
	{
		free(ast->pre_red_args[subtype][i]);
		ast->pre_red_args[subtype][i++] = NULL;
	}
	if (ast->red_exp_args[subtype])
		free(ast->red_exp_args[subtype]);
	ast->red_exp_args[subtype] = NULL;
	return (1);
}

int	extract_red_args_rec(t_s_token cur, t_s_parser *s, int r_subtype, int i_tkn)
{
	t_s_token	nxt;
	int			i_ltr;

	nxt.limiter = cur.limiter;
	cur.space_after = SPACE_AFTER;
	i_tkn++;
	i_ltr = count_token(cur.str, &cur, &nxt);
	nxt.str = cur.str + i_ltr;
	if (cur.space_after == NO_SPACE_AFTER && nxt.str)
		s->n_cmd_ltrs += extract_red_args_rec(nxt, s, r_subtype, i_tkn);
	else if (!free_and_null_red_args(s->ast, r_subtype)
		|| !allocate_red_args(s->ast, i_tkn, r_subtype))
		return (0);
	s->ast->pre_red_args[r_subtype][i_tkn] = ms_strcpy((cur.str), i_ltr);
	if (!s->ast->pre_red_args[r_subtype][i_tkn])
		return (perror("Error malloc redirection argument"), 0);
	if (cur.limiter != '\'' && *cur.str == '$')
		s->ast->red_exp_args[r_subtype][i_tkn] = EXPAND;
	return(i_ltr);
}

int	extract_redirect(int *i_ltr, t_s_token cur, t_s_parser *s)
{
	int	i_tkn;
	int	r_subtype;

	i_tkn = 0;
	r_subtype = subtype(cur.str);
	*i_ltr = skip_red_sign_and_spaces(cur.str, &r_subtype);
	if (*i_ltr == ERROR)
		return (perror("Error: No file after redirect"), ERROR);
	convert_append_heredoc_to_in_out(s->ast, &r_subtype);
	cur.str += *i_ltr;
	s->n_cmd_ltrs += extract_red_args_rec(cur, s, r_subtype, i_tkn);
	if (s->ast->heredoc)
		ms_heredoc(s->ast, s);
	return(*i_ltr);
}
