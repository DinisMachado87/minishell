DINIS

- "cat <test.txt" (first arg of red null)
- echo "> >> < \* ? [ ] | ; [ ] || && ( ) & # $ \ <<"
- handle empty cmds
- request " and ( when incomplete.
- signals for heredoc.
- unlink heredoc files after execution
- Errors that are not system calls should not print with perror
- check free functions;

==AND NODE==

left = regular left args
right = NULL
instead of right we have:
args[0] = "string with the remaining string after the '&&'"

export env=cat && echo hello | tr a-z A-Z && ls -l

type = AND
left = type: CMD :: args: export env=cat
right = NULL
intead of right we have:
args: "echo hello | tr a-z A-Z && ls -l"

$env hello$carhithere$hhhko
e n e n e n

'ls -l'
'yellow'
'ok'

hello
world
helloworld
hello
helloworldhello
world\n
helloworldhelloworld

hello
world
hello
world\n
helloworldhellowold
