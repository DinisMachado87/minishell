/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:18:51 by jlind             #+#    #+#             */
/*   Updated: 2025/09/12 08:39:15 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(t_ast *node)
{
	int	i;
	int	j;
	int	newline;

	if (!ms_strcmp(node->args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	else
	{
		newline = 1;
		i = 1;
	}
	while (node->args[i])
	{
		j = 0;
		while (node->args[i][j])
		{
			write(1, &node->args[i][j], 1);
			j++;
		}
		i++;
		if (i < node->n_args)
			write(1, " ", 1);
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(t_shell *shell, t_ast *node)
{
	char	*pwd;
	char	*oldpwd;
	char	*pwd_env;

	if (node->n_args > 2)
	{
		print_err("cd", "too many arguments");
		return (ERROR);
	}
	pwd = getcwd(NULL, 0);
	if (strncmp(node->args[1], "-", 1) == 0)
	{
		if (chdir(get_env_node(shell->env, "OLDPWD")->value) == -1)
		{
			perror("chdir");
			return (ERROR);
		}
	}
	else if (chdir(node->args[1]) == -1)
	{
		perror("chdir");
		return (ERROR);
	}
	oldpwd = ms_strndup("OLDPWD", 6);
	set_env_node(&shell->env, oldpwd, pwd);
	pwd_env = ms_strndup ("PWD", 3);
	pwd = getcwd(NULL, 0);
	set_env_node(&shell->env, pwd_env, pwd);
	free(pwd);
	free(oldpwd);
	return (0);
}

int	ft_pwd(void)
{
	char		*path;

	path = getcwd(NULL, 0);
	printf("%s\n", path);
	free(path);
	return (0);
}

int	ft_export(t_shell *shell, t_ast *node)
{
	char	*equ;
	char	*key;
	char	*value;

	equ = NULL;
	key = NULL;
	value = NULL;
	if (node->n_args == 1)
		print_env(shell, 1);
	else
	{
		equ = ms_strchr(node->args[1], '=');
		if (equ)
		{
			key = ms_strndup(node->args[1],
					(ms_strlen(node->args[1]) - ms_strlen(equ)));
			value = ms_strndup((equ + 1), (ms_strlen(equ) - 1));
			if (!*key || !ms_isalpha(key))
			{
				print_err("export", "not a valid identifier");
				return (ERROR);
			}
		}
		else if (!ms_isalpha(node->args[1]))
		{
			print_err("export", "not a valid identifier");
			return (ERROR);
		}
		set_env_node(&shell->env, key, value);
		free(key);
		free(value);
	}
	return (0);
}

int	ft_unset(t_shell *shell, t_ast *node)
{
	int	i;

	i = 1;
	if (!node->args[i])
		return (0);
	while (node->args[i])
	{
		free_env_node_by_key(&shell->env, node->args[i]);
		i++;
	}
	return (0);
}

int	ft_env(t_shell *shell, t_ast *node)
{
	if (node->n_args > 1)
		return (ERROR);
	print_env(shell, 0);
	return (0);
}

void	ft_exit(t_shell *shell, t_ast *node)
{
	int	exit_status;

	if (node->n_args > 2)
	{
		print_err("exit", "too many arguments");
		exit_status = 1;
	}
	else if (node->args[1])
	{
		if (ms_isdigit(node->args[1]))
			exit_status = ms_atoi(node->args[1]);
		else
		{
			print_err("exit", "numeric argument required");
			exit_status = 2;
		}

	}
	else
		exit_status = shell->exit_status;
	free_ast(&shell->ast_head);
	exit(exit_status);
}
