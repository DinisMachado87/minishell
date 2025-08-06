#include "../include/minishell.h"

int	execute_external(t_shell *shell, t_ast *node)
{
	char	*cmd;

	cmd = get_cmd_path(node->args[0], get_env_node(shell->env, "PATH")->value);
	if (execve(cmd, node->args, convert_env_to_list(shell->env)) == -1)
	{
		perror("execve");
		return (1);
	}
	return (0);
}

int	execute_built_in(t_shell *shell, t_ast *node)
{
	if (node->subtype == ECHO)
		return (ft_echo( node));
	else if (shell->ast_tree->subtype == CD)
		return (ft_cd(shell, node));
	else if (shell->ast_tree->subtype == PWD)
		return (ft_pwd());
	else if (shell->ast_tree->subtype == EXPORT)
		return (ft_export(shell));
	else if (shell->ast_tree->subtype == UNSET)
		return (ft_unset(shell));
	else if (shell->ast_tree->subtype == ENV)
		return (ft_env(shell));
	else if (shell->ast_tree->subtype == EXIT)
		return (ft_exit(shell));
	return (0);
}

int	execute_ast(t_shell *shell, t_ast *node)
{
	if (!shell->ast_tree || !node)
		return (1);
	if (node->type == AND)
		return (execute_and(shell, node));
	if (node->type == PIPE)
		return (execute_pipe(shell, node));
	else if (node->type == CMD)
	{
		if (node->subtype == EXTERNAL)
			return (execute_external(shell, node));
		else
			return (execute_built_in(shell, node));
	}
	return (0);
}

int	execute_and(t_shell *shell, t_ast *node)
{
	int	status;

	status = 0;
	status = execute_ast(shell, node->left);
	if (status == 0)
		status = execute_ast(shell, node->right);
	return (status);
}

int	execute_pipe(t_shell *shell, t_ast *node)
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
		execute_ast(shell, node->left);
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
		execute_ast(shell, node->right);
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
