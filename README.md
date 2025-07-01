# Minishell
## Basic Idea
1. Tokenize/Lex the input (create tokens out of the input)
2. Expansion, expand variables echo $var, command substitution echo $(ls), arithmetic echo $((2 + 2)) and wildcars echo *
3. Parse the tokens into the AST
4. Execute the AST

## Tokenizing
### Process
Go through the input and extract words. 
At the beginning there is no meaning to the "words" and hence they're all of type word, despite the redirects and pipes, they're special.
### Errors to detect in this stage
Unclosed quotes
Invalid character sequences e.g. '|||', '><<'
Unescaped special characters: `, &, ;, <, >, (, ), #
## Expansion
### Process
Determine kind of expansion
Based on the kind look for variable, execute command to substitute and insert it's output into the commands token, do arithmetic or replace wildcard
Order of expansion: Tilde → Parameter → Command → Arithmetic → Word Splitting → Globbing
Good to know:
Variables can be stored in a env structs that consist of key: value pairs
### Errors to detect in this stage
Undefined variables
Invalid arithmeitc
Invalid command substitution
## Parsing
### Process
Create the AST from the tokens
### Errors to detect in this stage
## Execution
### Process
### Errors to detect in this stage
## Example inputs
```
jlind@pop-os:~/projects/minishell/src(mini_version)$ echo -ne "Hello\nHello2"
Hello
Hello2jlind@pop-os:~/projects/minishell/src(mini_version)$
```
```
jlind@pop-os:~/projects/minishell/src(mini_version)$ echo -ne "Hello\nHello2" | grep 2
Hello2
```
```
jlind@pop-os:~/projects/minishell/src(mini_version)$ echo -e "Hello\nHello2"|grep 2
Hello2
```
