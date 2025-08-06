#include "../include/minishell.h"

void	execute_external(t_ast *node)
{
	char	*cmd;
	char	*env[2];

	env[0] = getenv("PATH");
	env[1] = NULL;

	cmd = get_cmd_path(node->args[0], env[0]);
	if (execve(cmd, node->args, env) == -1)
	{
		perror("execve");
		exit(1);
	}
}

void	execute_built_in(t_ast *node)
{
	if (node->subtype == ECHO)
		ft_echo(node);
	else if (node->subtype == CD)
		ft_cd(node);
	else if (node->subtype == PWD)
		ft_pwd(node);
	else if (node->subtype == EXPORT)
		ft_export(node);
	else if (node->subtype == UNSET)
		ft_unset(node);
	else if (node->subtype == ENV)
		ft_env(node);
	else if (node->subtype == EXIT)
		ft_exit(node);
}

void	execute_ast(t_ast *node)
{

	if (!node)
		return ;
	if (node->type == PIPE)
		execute_pipe(node);
	else
	{
		if (node->subtype == EXTERNAL)
			execute_external(node);
		else
			execute_built_in(node);
	}
}

void	execute_pipe(t_ast *node)
{
	int			left_pid;
	int			right_pid;
	int			fd[2];
	int			wstatus;
	
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	left_pid = fork();
	if (left_pid < 0)
		return ;
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
		return ;
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
	waitpid(left_pid, &wstatus, 0);
	waitpid(right_pid, &wstatus, 0);
}	

/*
 * type (1 = pipe; 2 = cmd)
 * subtype (1 = echo, 2 = external)
 * args = e.g. "echo hi"
 */

/*
int	main(void)
{
	t_ast	*n1;

	n1 = malloc(sizeof(t_ast));
	n1->type = CMD;
	n1->subtype = ECHO;
	n1->args = malloc(sizeof(char *) * 5);
	n1->args[0] = "/usr/bin/echo";
	n1->args[1] = "-n";
	n1->args[2] = "Hello World";
	n1->args[3] = "How are you?";
	n1->args[4] = NULL;
	execute_ast(n1);
	free(n1);
	return (0);
}
*/

int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n1->type = PIPE;
	n1->left = n2;
	n1->right = n3;
	n2->type = CMD;
	n2->subtype = ECHO;
	n2->args = malloc(sizeof(char *) * 4);
	//n2->args[0] = "/usr/bin/echo";
	n2->args[0] = "/usr/bin/echo";
	n2->args[1] = "Hello World";
	n2->args[2] = "How are you?";
	n2->args[3] = NULL;
	n3->type = CMD;
	n3->subtype = EXTERNAL;
	n3->args = malloc(sizeof(char *) * 2);
	n3->args[0] = "wc";
	n3->args[1] = NULL;
	execute_ast(n1);
	free(n1);
	free(n2->args);
	free(n2);
	free(n3);
	return (0);
}

/*
int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;
	t_ast	*n4;
	t_ast	*n5;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n4 = malloc(sizeof(t_ast));
	n5 = malloc(sizeof(t_ast));

	// |
	n1->type = 1;
	n1->left = n2;
	n1->right = n3;
	// |
	n2->type = 1;
	n2->left = n4;
	n2->right = n5;
	// wc -l
	n3->type = 2;
	n3->subtype = 2;
	n3->args = "wc";
	// echo hi
	n4->type = 2;
	n4->subtype = 1;
	n4->args = "ls";
	// cat
	n5->type = 2;
	n5->subtype = 2;
	n5->args = "cat";
	execute_ast(n1);
	free(n1);
	free(n2);
	free(n3);
	free(n4);
	free(n5);
	return (0);
}
*/

/*
void	pipex(void)
{
	int			pid1;
	int			pid2;
	int			fd[2];
	char		*str;
	size_t	n;
	int			wstatus;
	
	str = NULL;
	n = 0;
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		system("echo Hello");
		close(fd[1]);
		exit(0);
	}
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		getline(&str, &n, stdin);
		close(fd[0]);
		printf("%s\n", str);
		free(str);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &wstatus, 0);
	waitpid(pid2, &wstatus, 0);
}	

void	pipex(t_ast *node)
{
	int			pid1;
	int			pid2;
	int			fd[2];
	int			wstatus;
	
	if (!node->left)
		return ;
	pipex(node->left);
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		system(node->left->args);
		close(fd[1]);
		exit(0);
	}
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		system(node->right->args);
		close(fd[0]);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &wstatus, 0);
	waitpid(pid2, &wstatus, 0);
}	
*/
