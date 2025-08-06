#include "../include/minishell.h"

int	ft_echo(t_ast *node)
{
	int	i;
	int	j;
	int	newline;

	if (!strcmp(node->args[1], "-n"))
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
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(t_shell *shell, t_ast *node)
{
	char	*pwd;
	char	*oldpwd;

	if (node->n_args > 2)
		return (1);
	pwd = getcwd(NULL, 0);
	if (strncmp(node->args[1], "-", 1) == 0)
	{
		if (chdir(get_env_node(shell->env, "OLDPWD")->value) == -1)
		{
			perror("chdir");
			return (1);
		}
	}
	else if (chdir(node->args[1]) == -1)
	{
		perror("chdir");
		return (1);
	}
	oldpwd = strndup("OLDPWD", 6);
	set_env_node(&shell->env, oldpwd, pwd);
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
		// print env
		print_env(shell, 1);
	else
		// set a new env
	{
		equ = strchr(node->args[1], '=');
		if (!equ)
			return (0);
		key = strndup(node->args[1], (strlen(node->args[1]) - strlen(equ)));
		value = strndup((equ + 1), (strlen(equ) - 1));
		set_env_node(&shell->env, key, value);
		free(key);
		free(value);
	}
	return (0);
}

int  ft_unset(t_shell *shell, t_ast *node)
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

int  ft_env(t_shell *shell, t_ast *node)
{
	if (node->n_args > 1)
		return (1);
	print_env(shell, 1);
	return (0);
}

void	ft_exit(t_shell *shell)
{
	free_all(&shell->ast_head);
	exit(0);
}
