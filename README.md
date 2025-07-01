# Minishell
## Basic Idea
1. Parse input
2. Generate basic AST
3. Execute the extracted command

### Parser
Parse the input and extract command, flags and arguments
### AST
Generate a basic AST containing the command with it's flags and args and pipes/redirects
### Executor
Execute the commands and pipes/redirects provided by the AST
### Example inputs
1.
jlind@pop-os:~/projects/minishell/src(mini_version)$ echo -ne "Hello\nHello2"
Hello
Hello2jlind@pop-os:~/projects/minishell/src(mini_version)$
2.
jlind@pop-os:~/projects/minishell/src(mini_version)$ echo -ne "Hello\nHello2" | grep 2
Hello2
