#include "../../include/minishell.h"

int	execute_external(t_shell *shell, t_ast *node)
{
	char	*cmd;
	char	**list;
	int		pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		cmd = get_cmd_path(node->args[0], get_env_node(shell->env, "PATH")->value);
		if (!cmd)
		{
			print_err(node->args[0], "command not found");
			exit(1);
		}
		list = convert_env_to_list(shell->env);
		if (execve(cmd, node->args, list) == -1)
		{
			perror("execve");
			free_env_list(list);
			exit(1);
		}
		free_env_list(list);
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (status);
}

int	execute_built_in(t_shell *shell, t_ast *node)
{
	if (node->subtype == ECHO)
		return (ft_echo(node));
	else if (node->subtype == CD)
		return (ft_cd(shell, node));
	else if (node->subtype == PWD)
		return (ft_pwd());
	else if (node->subtype == EXPORT)
		return (ft_export(shell, node));
	else if (node->subtype == UNSET)
		return (ft_unset(shell, node));
	else if (node->subtype == ENV)
		return (ft_env(shell, node));
	else if (node->subtype == EXIT)
		ft_exit(shell);
	return (0);
}

int	execute_ast(t_shell *shell, t_ast *node)
{
	int		status;
	char	*stat;
	int		fd;
	int		save_stdin;
	int		save_stdout;
	t_shell	subshell;
	
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	status = 0;
	if (!shell->ast_tree || !node)
		return (1);
	if (node->type == OPERATOR && node->subtype == AND)
		status = execute_and(shell, node);
	else if (node->type == OPERATOR && node->subtype == OR)
		status = execute_or(shell, node);
	else if (node->type == PIPE)
		status = execute_pipe(shell, node);
	else if (node->type == CMD)
	{
		if (node->red_args[IN] || node->red_args[OUT])
		{
			if (node->red_args[IN])
			{
				fd = open(node->red_args[IN], O_RDONLY);
				dup2(fd, STDIN_FILENO);
			}
			else
			{
				if (node->append)
					fd = open(node->red_args[OUT], O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd = open(node->red_args[OUT], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(fd, STDOUT_FILENO);
			}
			if (fd == -1)
			{
				perror(node->red_args[OUT]);
				dup2(save_stdin, STDIN_FILENO);
				dup2(save_stdout, STDOUT_FILENO);
				return (-1);
			}
			close(fd);
		}
		if (node->subtype == EXTERNAL)
			status = execute_external(shell, node);
		else
			status = execute_built_in(shell, node);
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
	}
	else if (node->type == SUBSHELL)
	{
		ms_bzero((void *)&subshell, sizeof(t_shell));
		subshell.env = init_env();
		subshell.ast_tree = parser(node->args[0], &subshell.ast_head);
		execute_ast(&subshell, subshell.ast_tree);
		free_ast(&subshell.ast_head);
	}
	stat = itoa(status);
	set_env_node(&shell->env, "?", stat);
	return (status);
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

int	execute_or(t_shell *shell, t_ast *node)
{
	int	status;

	status = 0;
	status = execute_ast(shell, node->left);
	if (status != 0)
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
