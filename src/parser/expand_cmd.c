/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:29:36 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/24 11:38:00 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmd_expander(t_args	*args, t_shell *sh)
{
	if (args[0].n
		&& (!expand_tkn_arr(&args[0], sh)
			|| !tkns_to_words(&args[0], args[0].n)
			|| !split_cmd_flags(args[0].tkns[0], &args[0], args[0].n)))
		return (ERROR);
	if (args[IN].n
		&& (!expand_tkn_arr(&args[IN], sh)
			|| !tkns_to_words(&args[IN], args[IN].n)))
		return (ERROR);
	if (args[OUT].n
		&& (!expand_tkn_arr(&args[OUT], sh)
			|| !tkns_to_words(&args[OUT], args[OUT].n)))
		return (ERROR);
	if (DEBUG)
		print_ast(sh->ast, "AFTER EXPANSION");
	return (1);
}
