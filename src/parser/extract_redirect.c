/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:56:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/13 01:26:47 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	convert_append_heredoc_to_in_out(t_ast *ast, t_s_token *r)
{
	ast->append = 0;
	if (r->red_subtype == APPEND)
	{
		ast->append = 1;
		r->red_subtype = OUT;
	}
	else if (r->red_subtype == HEREDOC)
	{
		ast->heredoc = 1;
		r->heredoc = 1;
		r->red_subtype = IN;
	}
	else if (r->red_subtype == OUT)
		ast->append = 0;
	else if (r->red_subtype == IN)
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

int	skip_red_sign_and_spaces(char *str, t_s_token *r)
{
	if (r->red_subtype == APPEND || r->red_subtype == HEREDOC)
		r->spaces += 2;
	else
		r->spaces++;
	while (str[r->spaces] && str[r->spaces] == ' ')
		r->spaces++;
	if (!str[r->spaces] || type(str + r->spaces) != CMD)
		return (0);
	return (1);
}

int	extract_redirect(t_ast *ast, char *str, t_s_parser *s)
{
	t_s_token	r;

	r.i_ltr = 0;
	r.spaces = 0;
	r.heredoc = 0;
	r.red_subtype = subtype(str);
	r.quotes = NO_QUOTES;
	r.space_after = SPACE_AFTER;
	r.expand = POTENCIALLY_EXPAND;
	if (!skip_red_sign_and_spaces(str, &r))
		return (perror("Error: No file after redirect"), 0);
	r.i_ltr = skip_count_word((str + r.spaces), &r);
	r.word = ms_strcpy((str + r.spaces), r.i_ltr);
	if (!r.word)
		return (perror("Error malloc redirection argument"), 0);
	convert_append_heredoc_to_in_out(ast, &r);
	free_red_args(ast, r.red_subtype);
	ast->red_args[r.red_subtype] = r.word;
	ast->red_exp_args[r.red_subtype] = r.expand;
	if (r.heredoc)
		ms_heredoc(ast, s);
	if (str[r.spaces + r.i_ltr])
		s->n_cmd_ltrs += extract_cmd_recursive(ast,
							str + r.spaces + r.i_ltr, s);
	return(r.spaces + r.i_ltr);
}
