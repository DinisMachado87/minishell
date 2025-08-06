#include "../include/minishell.h"

int	execute_external(t_ast *node)
{
	char	*cmd;
	char	*env[2];

	env[0] = getenv("PATH");
	env[1] = NULL;

	cmd = get_cmd_path(node->args[0], env[0]);
	if (execve(cmd, node->args, env) == -1)
	{
		perror("execve");
		return (1);
	}
	return (0);
}

int	execute_built_in(t_ast *node)
{
	if (node->subtype == ECHO)
		return (ft_echo(node));
	else if (node->subtype == CD)
		return (ft_cd(node));
	else if (node->subtype == PWD)
		return (ft_pwd());
	else if (node->subtype == EXPORT)
		return (ft_export(node));
	else if (node->subtype == UNSET)
		return (ft_unset(node));
	else if (node->subtype == ENV)
		return (ft_env(node));
	else if (node->subtype == EXIT)
		return (ft_exit(node));
	return (0);
}

int	execute_ast(t_ast *node)
{
	if (!node)
		return (1);
	if (node->type == AND)
		return (execute_and(node));
	if (node->type == PIPE)
		return (execute_pipe(node));
	else if (node->type == CMD)
	{
		if (node->subtype == EXTERNAL)
			return (execute_external(node));
		else
			return (execute_built_in(node));
	}
	return (0);
}

int	execute_and(t_ast *node)
{
	int	status;

	status = 0;
	status = execute_ast(node->left);
	if (status == 0)
		status = execute_ast(node->right);
	return (status);
}

int	execute_pipe(t_ast *node)
{
	int			left_pid;
	int			right_pid;
	int			fd[2];
	int			lstatus;
	int			rstatus;
	
	lstatus = 0;
	rstatus = 0;
	if (pipe(fd) < 0)
		return (1);
	left_pid = fork();
	if (left_pid < 0)
		return (1);
	if (left_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute_ast(node->left);
		exit(0);
	}
	right_pid = fork();
	if (right_pid < 0)
		return (1);
	if (right_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execute_ast(node->right);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &lstatus, 0);
	waitpid(right_pid, &rstatus, 0);
	if (lstatus)
		return (lstatus);
	else if (rstatus)
		return (rstatus);
	return (0);
}	
