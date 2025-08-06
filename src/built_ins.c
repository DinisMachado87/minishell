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

	if (node->n_args > 2)
		return (1);
	pwd = getcwd(NULL, 0);
	if (strncmp(node->args[0], "-", 1) == 0)
	{
		if (chdir(get_env_node(shell->env, "OLDPWD")->value) == -1)
		{
			perror("chdir");
			return (1);
		}
	}
	if (chdir(node->args[1]) == -1)
	{
		perror("chdir");
		return (1);
	}
	set_env_node(&shell->env, strndup("OLDPWD", 6), pwd);
	free(pwd);
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

int	ft_export(t_shell *shell)
{
	(void)shell;
	return (0);
}

int  ft_unset(t_shell *shell)
{
	(void)shell;
	return (0);
}

int  ft_env(t_shell *shell)
{
	(void)shell;
	return (0);
}

int	ft_exit(t_shell *shell)
{
	(void)shell;
	return (0);
}
