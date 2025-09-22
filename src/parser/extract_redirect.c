/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:56:17 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/22 19:23:38 by dimachad         ###   ########.fr       */
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

static int	find_array_subtype(t_cmd *c)
{
	if (c->subtype == APPEND)
		c->subtype = OUT;
	else if (c->subtype == HEREDOC)
		c->subtype = IN;
	return (1);
}

static int	extract_red_args(t_token *cur, t_args *args, int i_tkn)
{
	const char	*err_str = "Error malloc redirect args";
	t_token	nxt;
	int		len;

	cur->space_after = NO_SPACE_AFTER;
	while (i_tkn < args->n)
	{
		nxt.limiter = cur->limiter;
		cur->space_after = NO_SPACE_AFTER;
		cur->expand = DONT_EXPAND;
		len = count_token(cur->str, cur, &nxt);
		nxt.str = cur->str + len;
		args->tkns[i_tkn] = ms_strcpy(cur->str, len);
		if (!args->tkns[i_tkn])
			return (perror(err_str), 0);
		i_tkn++;
		*cur = nxt;
	}
	args->space[i_tkn - 1] = SPACE_AFTER;
	return(len);
}

int	copy_old_args(t_args *old_args, t_args *new_args)
{
	int	i;

	i = 0;
	while (i < old_args->n)
	{
		new_args->tkns[i] = old_args->tkns[i];
		old_args->tkns[i] = NULL;
		new_args->exp[i] = old_args->exp[i];
		new_args->space[i] = old_args->space[i];
		new_args->type[i] = old_args->type[i];
		i++;
	}
	free_and_null((void **)&old_args->tkns);
	free_and_null((void **)&old_args->exp);
	free_and_null((void **)&old_args->space);
	free_and_null((void **)&old_args->type);
	return (1);
}

/* New_args and old_args are the same instance of t_args,
 * but one passed by reference and the other literally.
 * Therefore the nested pointers are preserved in old_args in the stack
 * when loger arrays are realloc in new args by reference.
 * The strings in the old array are then moved,
 * therefore only the old array of pointers not it's strs are freed,
 * minimizing allocations.
 * Old_args is in the stack so no need to free;
 */
int	extract_redirect(t_token *cur, t_cmd *c, t_args *new_args, t_args old_args)
{
	int		orig_subtype;
	int		exp_heredoc;

	orig_subtype = c->subtype;
	if (!skip_red_sign_and_spaces(cur, c->subtype))
		return (0);
	exp_heredoc = c->subtype == HEREDOC
		&& (*cur->str == '\'' || *cur->str == '\"');
	if (!find_array_subtype(c)
		|| !count_redirect(*cur, new_args)
		|| !allocate_ast_args(new_args, old_args.n + new_args->n))
		return (0);
	if (old_args.tkns && !copy_old_args(&old_args, new_args))
		return (0);
	if (!extract_red_args(cur, new_args, old_args.n))
		return (0);
	new_args->type[old_args.n] = orig_subtype;
	if (orig_subtype == HEREDOC)
		ms_heredoc(new_args, old_args.n, exp_heredoc);
	return(1);
}
