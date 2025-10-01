/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:29:05 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 10:50:38 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_env_node(t_env *env, char *key, char *value)
{
	char	*val;

	val = ms_strndup(value, ms_strlen(value));
	if (!get_env_node(env, key))
		return (set_env_node(&env, key, val));
	free(get_env_node(env, key)->value);
	get_env_node(env, key)->value = val;
	free(value);
}
