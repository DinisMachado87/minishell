#include "../include/minishell.h"

// buggy? in combindation with wc i get weird results..
void  ft_echo(t_ast *node)
{
	int	i;
	int	j;
	int	newline;

	if (strcmp(node->args[1], "-n") == 0)
	{
		newline = 0;
		i = 1;
	}
	else
	{
		newline = 1;
		i = 2;
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
}

void  ft_cd(t_ast *node)
{
	(void)node;
	return ;
}

void  ft_pwd(t_ast *node)
{
	(void)node;
	return ;
}

void  ft_export(t_ast *node)
{
	(void)node;
	return ;
}

void  ft_unset(t_ast *node)
{
	(void)node;
	return ;
}

void  ft_env(t_ast *node)
{
	(void)node;
	return ;
}

void  ft_exit(t_ast *node)
{
	(void)node;
	return ;
}
