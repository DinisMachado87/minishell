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

AST_SRCS = $(AST_DIR)/execute_ast.c

CMD_SRCS = $(CMD_DIR)/built_ins.c \
		   $(CMD_DIR)/export_utils.c \
		   $(CMD_DIR)/get_cmd_path.c

HELPER_SRCS = $(HELPER_DIR)/env.c \
			  $(HELPER_DIR)/itoa.c \
			  $(HELPER_DIR)/loop.c \
			  $(HELPER_DIR)/print_err.c \
			  $(HELPER_DIR)/ms_strndup.c \
			  $(HELPER_DIR)/ms_strncat.c \
			  $(HELPER_DIR)/ms_strncpy.c \
			  $(HELPER_DIR)/ms_strchr.c

PARSER_SRCS = $(PARSER_DIR)/ast_utils.c \
			  $(PARSER_DIR)/extract_operator.c \
			  $(PARSER_DIR)/extract_utils.c \
			  $(PARSER_DIR)/parser_utils.c \
			  $(PARSER_DIR)/structure_ast.c \
			  $(PARSER_DIR)/extract_cmd.c \
			  $(PARSER_DIR)/extract_subshell.c \
			  $(PARSER_DIR)/gen_utils.c \
			  $(PARSER_DIR)/parser.c \
			  $(PARSER_DIR)/print_ast.c \
			  $(PARSER_DIR)/heredoc.c \
			  $(PARSER_DIR)/extract_redirect.c

PARSER_HELP_MAIN = $(HELPER_DIR)/itoa.c \
				   $(PARSER_DIR)/main_parser.c

MAIN = $(SRC_DIR)/main.c

AST_OBJS = $(AST_SRCS:.c=.o)
CMD_OBJS = $(CMD_SRCS:.c=.o)
HELPER_OBJS = $(HELPER_SRCS:.c=.o)
PARSER_OBJS = $(PARSER_SRCS:.c=.o)
MAIN_OBJ = $(MAIN:.c=.o)
PARSER_HELP_OBJS = $(PARSER_HELP_MAIN:.c=.o)

ALL_OBJS = $(AST_OBJS) $(CMD_OBJS) $(HELPER_OBJS) $(PARSER_OBJS) $(MAIN_OBJ)

all: $(NAME)

$(NAME): $(ALL_OBJS)
	$(CC) $(CFLAGS) $(ALL_OBJS) -lreadline -o $(NAME)

parser: $(PARSER_OBJS) $(PARSER_HELP_OBJS) $(HEADER)
	$(CC) $(DEBUG_FLAGS) $(PARSER_OBJS) $(PARSER_HELP_OBJS) -lreadline -o parser

debug: $(ALL_OBJS)
	$(CC) $(DEBUG_FLAGS) $(ALL_OBJS) -lreadline -o $(NAME)

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
	rm -f $(ALL_OBJS) $(PARSER_HELP_OBJS)

fclean: clean
	rm -f $(NAME) parser

re: fclean all

.PHONY: all parser debug clean fclean re
