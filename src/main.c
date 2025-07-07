#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void  prompt_loop(void);

void	sig_c_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

char  *get_prompt(void)
{
  char  *cwd;
  size_t  len;
  char  *prompt;

  cwd = getcwd(NULL, 0);
  if (!cwd)
    return (NULL);
  len = strlen(cwd);
  prompt = malloc(len + 3);
  if (!prompt)
  {
    free(cwd);
    return (NULL);
  }
  prompt[0] = '\0';
  strncat(prompt, cwd, len);
  strncat(prompt, "$ ", 3);
  free(cwd);
  return (prompt);
}

/*
 * On EOF redline returns NULL
 */
char  *get_input(char *prompt)
{
  char  *input;

  input = readline(prompt);
  if (!input)
	{
		free(prompt);
    exit(1);
	}
  if (*input)
    add_history(input);
  return (input);
}

/*
 * SIGINT = ctrl_c
 * SIGQUIT = ctrl_\
 */
void	prompt_loop(void)
{
  char  *input;
  char  *prompt;
	struct sigaction	sa_c;

  while (1)
	{
		sa_c.sa_flags = SA_RESTART;
		sigemptyset(&sa_c.sa_mask);
		sa_c.sa_handler = sig_c_handler;
		if (sigaction(SIGINT, &sa_c, NULL) == -1)
			perror("sigaction");
		signal(SIGQUIT, SIG_IGN);
		prompt = get_prompt();
		if (!prompt)
			return ;
		input = get_input(prompt);
		if (!input)
		{
			free(prompt);
			return ;
		}
	}
	if (input)
		free(input);
	free(prompt);
}

int main(void)
{
	prompt_loop();
  return (0);
}
