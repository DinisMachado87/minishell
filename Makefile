NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -lreadline
DEBUG_FLAGS = $(CFLAGS) -g -O0

HEADER_DIR = include
HEADER = $(HEADER_DIR)/minishell.h

SRC_DIR = src
AST_DIR = $(SRC_DIR)/ast
CMD_DIR = $(SRC_DIR)/commands
HELPER_DIR = $(SRC_DIR)/helper
PARSER_DIR = $(SRC_DIR)/parser

AST_SRC_FILES =		execute_ast.c
CMD_SRC_FILES =		built_ins.c export_utils.c get_cmd_path.c
HELPER_SRC_FILES =	env.c itoa.c loop.c print_err.c
PARSER_SRC_FILES =	ast_utils.c extract_operator.c extract_utils.c parser_utils.c structure_ast.c extract_cmd.c extract_subshell.c\
			  		gen_utils.c parser.c print_ast.c

MAIN = $(SRC_DIR)/main.c

AST_SRCS = $(addprefix $(AST_DIR)/, $(AST_SRC_FILES))
CMD_SRCS = $(addprefix $(CMD_DIR)/, $(CMD_SRC_FILES))
HELPER_SRCS = $(addprefix $(HELPER_DIR)/, $(HELPER_SRC_FILES))
PARSER_SRCS = $(addprefix $(PARSER_DIR)/, $(PARSER_SRC_FILES))

OBJ_DIR = obj

AST_OBJ_FILES = $(AST_SRC:.c=.o)
AST_OBJS = $(addprefix $(OBJ_DIR)/, $(AST_OBJ_FILES))
CMD_OBJ_FILES = $(CMD_SRC:.c=.o)
CMD_OBJS = $(addprefix $(OBJ_DIR)/, $(CMD_OBJ_FILES))
HELPER_OBJ_FILES = $(HELPER_SRC:.c=.o)
HELPER_OBJS = $(addprefix $(OBJ_DIR)/, $(HELPER_OBJ_FILES))
PARSER_OBJ_FILES = $(PARSER_SRC:.c=.o)
PARSER_OBJS = $(addprefix $(OBJ_DIR)/, $(PARSER_OBJ_FILES))

all: $(NAME)

$(NAME): $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(HEADER) $(MAIN)
	$(CC) $(CFLAGS) $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(HEADER) $(MAIN) -o $(NAME)

debug: $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(HEADER) $(MAIN)
	$(CC) $(DEBUG_FLAGS) $(AST_SRCS) $(CMD_SRCS) $(HELPER_SRCS) $(PARSER_SRCS) $(HEADER) $(MAIN) -o $(NAME)

clean:
	rm -f $(NAME)

fclean:
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
