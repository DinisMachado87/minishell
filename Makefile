NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = $(CFLAGS) -g -O0
HEADER_DIR = include
HEADER = $(HEADER_DIR)/minishell.h
SRC_DIR = src
AST_DIR = $(SRC_DIR)/ast
CMD_DIR = $(SRC_DIR)/commands
HELPER_DIR = $(SRC_DIR)/helper
PARSER_DIR = $(SRC_DIR)/parser

# Source files without directory prefixes
AST_FILES =		execute_ast.c \
				expand_cmd.c
CMD_FILES =		built_ins.c \
				export_utils.c \
				get_cmd_path.c
HELPER_FILES =	env.c \
				itoa.c \
				loop.c \
				print_err.c \
				ms_strndup.c \
				ms_strncat.c \
				ms_strncpy.c \
				ms_strchr.c
PARSER_FILES =	ast_utils.c \
				extract_operator.c \
				extract_utils.c \
				parser_utils.c \
				structure_ast.c \
				extract_cmd.c \
				extract_subshell.c \
				gen_utils.c \
				parser.c \
				print_ast.c \
				heredoc.c \
				count_token.c \
				count_cmd_tokens.c \
				extract_redirect.c
MAIN_FILE =		main.c

# Source files with directory prefixes
AST_SRCS = $(addprefix $(AST_DIR)/, $(AST_FILES))
CMD_SRCS = $(addprefix $(CMD_DIR)/, $(CMD_FILES))
HELPER_SRCS = $(addprefix $(HELPER_DIR)/, $(HELPER_FILES))
PARSER_SRCS = $(addprefix $(PARSER_DIR)/, $(PARSER_FILES))
MAIN = $(addprefix $(SRC_DIR)/, $(MAIN_FILE))

# Object files
AST_OBJS = $(AST_SRCS:.c=.o)
CMD_OBJS = $(CMD_SRCS:.c=.o)
HELPER_OBJS = $(HELPER_SRCS:.c=.o)
PARSER_OBJS = $(PARSER_SRCS:.c=.o)
MAIN_OBJ = $(MAIN:.c=.o)

ALL_OBJS = $(AST_OBJS) $(CMD_OBJS) $(HELPER_OBJS) $(PARSER_OBJS) $(MAIN_OBJ)

all: $(NAME)

$(NAME): $(ALL_OBJS)
	$(CC) $(CFLAGS) $(ALL_OBJS) -lreadline -o $(NAME)

debug: $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(MAIN) $(HEADER)
	$(CC) $(DEBUG_FLAGS) -I$(HEADER_DIR) $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(MAIN) -lreadline -o $(NAME)

# Test targets that use separate Makefiles
parser:
	$(MAKE) -C tests/parser

norm:
	$(MAKE) -C tests/normalizer

parser-clean:
	$(MAKE) -C tests/parser clean

norm-clean:
	$(MAKE) -C tests/normalizer clean

# Object file compilation rules
$(AST_DIR)/%.o: $(AST_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(CMD_DIR)/%.o: $(CMD_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(HELPER_DIR)/%.o: $(HELPER_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(ALL_OBJS)

fclean: clean parser-clean norm-clean
	rm -f $(NAME)

re: fclean all

.PHONY: all parser norm parser-clean norm-clean debug clean fclean re
