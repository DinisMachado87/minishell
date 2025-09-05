/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:18:14 by jlind             #+#    #+#             */
/*   Updated: 2025/09/05 15:20:33 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_env(t_shell *shell, int _export)
{
	t_env	*var;

	var = shell->env;
	while (var)
	{
		if (_export)
			printf("declare -x %s=%s\n", var->key, var->value);
		else
			printf("%s=%s\n", var->key, var->value);
		var = var->next;
	}
}
