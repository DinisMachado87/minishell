/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:10:22 by jlind             #+#    #+#             */
/*   Updated: 2025/09/18 20:27:40 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_external(t_shell *shell, t_ast *node)
{
	char	*cmd;
	char	**list;
	int		pid;
	int		status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		//cmd = get_cmd_path(shell, node->args[0]);
		cmd = get_cmd_path(shell, *node->args[0].tkns);
		if (!cmd)
			exit(shell->exit_status);
		list = convert_env_to_list(shell->env);
		//if (execve(cmd, node->args, list) == -1)
		if (execve(cmd, node->args[0].tkns, list) == -1)
		{
			perror("execve");
			free(cmd);
			free_env_list(list);
			free_ast(shell);
			exit(1);
		}
		free(cmd);
		free_env_list(list);
		free_ast(shell);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	set_handler(0);
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
		ft_exit(shell, node);
	return (0);
}

/*
   I need to loop through all the redirect out files, creating them,
   then setting up the fd redirect using dup2, that's why all the
   files will be created but only the last one containg the output.
   Cause all the files will be touched but the redirect will override
   itself so only the last one will really receive the redirected data
*/
void	execute_ast(t_shell *shell, t_ast *node)
{
	int			fd;
	int			save_stdin;
	int			save_stdout;
	t_shell		subshell;
	struct stat	statbuf;
	int			last_slash_pos;
	char		*last_slash;

	ms_bzero((void *)&statbuf, sizeof(struct stat));
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (!shell->ast || !node)
		shell->exit_status = ERROR;
	if (node->type == OPERATOR && node->subtype == AND)
		execute_and(shell, node);
	else if (node->type == OPERATOR && node->subtype == OR)
		execute_or(shell, node);
	else if (node->type == PIPE)
		execute_pipe(shell, node);
	else if (node->type == CMD)
	{
		cmd_expander(node->args, shell);
		//if (!node->args[0])
		if (!node->args[0].tkns)
			node->subtype = EXTERNAL;
		else
			//node->subtype = strict_subtype(node->args[0]);
			node->subtype = strict_subtype(node->args[0].tkns[0]);
		if (DEBUG)
			print_ast(shell->ast, "After cmd expander");
		//if (node->red_args[IN] || node->red_args[OUT])
		if ((node->args[IN].tkns && *node->args[IN].tkns)
				|| (node->args[OUT].tkns && *node->args[OUT].tkns))
		{
			//if (node->red_args[IN])
			if (node->args[IN].tkns && *node->args[IN].tkns)
			{
				//stat(node->red_args[IN], &statbuf);
				stat(*node->args[IN].tkns, &statbuf);
				if (!statbuf.st_mode)
				{
					shell->exit_status = 1;
					//print_err(node->red_args[IN], "No such file or directory");
					print_err(*node->args[IN].tkns, "No such file or directory");
					return ;
				}
				//fd = open(node->red_args[IN], O_RDONLY);
				fd = open(*node->args[IN].tkns, O_RDONLY);
				dup2(fd, STDIN_FILENO);
				//unlink(node->red_args[IN]);
				unlink(*node->args[IN].tkns);
			}
			else
			{
				//last_slash = ms_strrchr(node->red_args[OUT], '/');
				last_slash = ms_strrchr(*node->args[OUT].tkns, '/');
				if (last_slash)
				{
					//last_slash_pos = ms_strlen(node->red_args[OUT]) - ms_strlen(last_slash);
					last_slash_pos = ms_strlen(*node->args[OUT].tkns) - ms_strlen(last_slash);
					//node->red_args[OUT][last_slash_pos] = '\0';
					*node->args[OUT].tkns[last_slash_pos] = '\0';
					//stat(node->red_args[OUT], &statbuf);
					stat(*node->args[OUT].tkns, &statbuf);
					//node->red_args[OUT][last_slash_pos] = '/';
					*node->args[OUT].tkns[last_slash_pos] = '/';
					if (S_ISDIR(statbuf.st_mode))
					{
						//if (access(node->red_args[OUT], W_OK))
						if (access(*node->args[OUT].tkns, W_OK))
						{
							// replacement?
						// [WARNING:] we now have more than one redirect so one needs to check the corresponding index in type to see if it is APPEND or another subtype
							if (node->args[OUT].type[0] == APPEND)
								//fd = open(node->red_args[OUT],
								fd = open(*node->args[OUT].tkns,
										O_WRONLY | O_CREAT | O_APPEND, 0644);
							else
								//fd = open(node->red_args[OUT],
								fd = open(*node->args[OUT].tkns,
										O_WRONLY | O_CREAT | O_TRUNC, 0644);
						}
					}
					else if (!statbuf.st_mode)
					{
						//print_err(node->red_args[OUT], "No such file or directory");
						print_err(*node->args[OUT].tkns, "No such file or directory");
						shell->exit_status = 1;
						return;
					}
				}
				dup2(fd, STDOUT_FILENO);
			}
			close(fd);
		}
		//if (node->subtype == EXTERNAL && *node->args)
		if (node->subtype == EXTERNAL && *node->args[0].tkns)
			execute_external(shell, node);
		//else if (node->subtype != EXTERNAL && *node->args)
		else if (node->subtype != EXTERNAL && *node->args[0].tkns)
			shell->exit_status = execute_built_in(shell, node);
		dup2(save_stdin, STDIN_FILENO);
		dup2(save_stdout, STDOUT_FILENO);
	}
	else if (node->type == SUBSHELL)
	{
		ms_bzero((void *)&subshell, sizeof(t_shell));
		init_env(&subshell, convert_env_to_list(shell->env));
		//subshell.ast_tree = parser(node->args[0], &subshell.ast_head);
		//[WARNING:] I noticed you are not checking for errors in a lot ofsystem calls,
		//and in the ones being checked I don't think you are exiting the function
		//which will lead to crash.
		//I sent you on slack a print screen
		if (ERROR == parser(node->args[0].tkns[0], &subshell))
			return ;
		execute_ast(&subshell, subshell.ast);
		free_ast(&subshell);
	}
}

void	execute_and(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status == 0)
		execute_ast(shell, node->right);
}

void	execute_or(t_shell *shell, t_ast *node)
{
	execute_ast(shell, node->left);
	if (shell->exit_status != 0)
		execute_ast(shell, node->right);
}

void	execute_pipe(t_shell *shell, t_ast *node)
{
	int			left_pid;
	int			right_pid;
	int			fd[2];
	int			lstatus;
	int			rstatus;

	if (pipe(fd) < 0)
		shell->exit_status = ERROR;
	left_pid = fork();
	if (left_pid < 0)
		shell->exit_status = ERROR;
	if (left_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute_ast(shell, node->left);
		exit(shell->exit_status);
	}
	right_pid = fork();
	if (right_pid < 0)
		shell->exit_status = ERROR;
	if (right_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execute_ast(shell, node->right);
		exit(shell->exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &lstatus, 0);
	if (WIFEXITED(lstatus))
		shell->exit_status = WEXITSTATUS(lstatus);
	waitpid(right_pid, &rstatus, 0);
	if (WIFEXITED(rstatus))
		shell->exit_status = WEXITSTATUS(rstatus);
}
