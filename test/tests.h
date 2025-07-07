#ifndef TEST_INPUTS_H
#define TEST_INPUTS_H

const char *test_inputs[] = {
    // 1. Basic
    "ls",
    "pwd",
    "echo hello",
    "echo -n hello world",

    // 2. Quoting
    "echo 'hello world'",
    "echo \"hello $USER\"",
    "echo 'it'\\''s fine'",
    "echo \"nested \\\"quotes\\\" ok\"",
    "echo ''",
    "echo \"\"",

    // 3. Redirections
    "ls > out.txt",
    "cat < infile.txt",
    "echo test >> append.txt",
    "grep hello < in.txt > out.txt",
    "echo nope > /no/such/path/file",
    "> out.txt echo hi",

    // 4. Pipes
    "ls | grep txt",
    "cat file.txt | wc -l | sort",
    "echo hello |",
    "| echo hello",
    "ls || echo fallback",

    // 5. Env vars
    "echo $USER",
    "echo \"$HOME/$USER\"",
    "echo $UNSET_VAR",
    "export VAR=value && echo $VAR",
    "VAR=test ./myprog",

    // 6. Separators
    "echo hello ; echo world",
    "echo first && echo second",
    "false && echo won't print",
    "true || echo fallback",

    // 7. Subshells
    "(echo hello)",
    "(cd .. && pwd)",
    "((echo nested))",

    // 8. Invalid / malformed
    "",
    "       ",
    "echo \"unterminated",
    "echo 'bad",
    "ls | | grep x",
    "cat <<",
    "ls > > out.txt",

    // 9. Whitespace
    "   echo    spaced   out    ",
    "echo \t tabbed",

    // 10. Built-ins
    "cd /",
    "cd",
    "export FOO=bar",
    "unset FOO",
    "env",
    "exit",
    "exit 1",
    "exit 256",
    "exit abc",
	0
};

#endif

