/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:56:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 02:38:42 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skip_red_sign_and_spaces(t_token *cur, int r_subtype)
{
	if (r_subtype == APPEND || r_subtype == HEREDOC)
		cur->str += 2;
	else
		cur->str++;
	while (*cur->str && *cur->str == ' ')
		cur->str++;
	if (!*cur->str || type(cur->str) != CMD)
		return (perror("Error: No file after redirect"), 0);
	return (1);
}

static int	convert_append_heredoc_to_in_out(t_ast *ast, int *r_subtype)
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
	return (1);
}

int	free_and_null_red_args(t_ast *ast, int subtype)
{
	int i;

	if (subtype == IN && ast->heredoc)
		unlink(ast->red_args[IN]);
	if (ast->red_args[subtype])
		free(ast->red_args[subtype]);
	ast->red_args[subtype] = NULL;
	i = 0;
	while (ast->pre_r_args[subtype]
		&& ast->pre_r_args[subtype][i])
	{
		free(ast->pre_r_args[subtype][i]);
		ast->pre_r_args[subtype][i++] = NULL;
	}
	if (ast->r_exp_args[subtype])
		free(ast->r_exp_args[subtype]);
	ast->r_exp_args[subtype] = NULL;
	return (1);
}

static int	extract_red_args(t_token *cur, t_ast *ast, int subtype)
{
	char	*err_str = "Error malloc redirection argument";
	t_token	nxt;
	int		len;
	int		i_tkn;

	i_tkn = 0;
	cur->space_after = NO_SPACE_AFTER;
	while (i_tkn < ast->n_red_tk[subtype])
	{
		nxt.limiter = cur->limiter;
		cur->space_after = SPACE_AFTER;
		len = count_token(cur->str, cur, &nxt);
		nxt.str = cur->str + len;
		ast->pre_r_args[subtype][i_tkn] = ms_strcpy(cur->str, len);
		if (!ast->pre_r_args[subtype][i_tkn])
			return (perror(err_str), 0);
		if (cur->limiter != '\'' && *cur->str == '$')
			ast->r_exp_args[subtype][i_tkn] = EXPAND;
		i_tkn++;
		*cur = nxt;
	}
	return(len);
}

int	extract_redirect(t_token *cur, t_cmd *c, t_parser *s)
{
	c->subtype = subtype(cur->str);
	if (!count_redirect(*cur, c, s->ast)
		|| !skip_red_sign_and_spaces(cur, c->subtype)
		|| !free_and_null_red_args(s->ast, c->subtype)
		|| !allocate_red_args(s->ast,
				s->ast->n_red_tk[c->subtype], c->subtype)
		|| !convert_append_heredoc_to_in_out(s->ast, &c->subtype)
		|| !extract_red_args(cur, s->ast, c->subtype))
		return (0);
	if (s->ast->heredoc)
		ms_heredoc(s->ast, s);
	return(1);
}
