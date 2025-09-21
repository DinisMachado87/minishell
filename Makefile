# ==== C Settings ====
CC := gcc
CFLAGS = -Wall -Wextra -Werror -I$(HEADER_DIR)
RELEASE_FLAGS = $(CFLAGS) -DDEBUG=0
DEBUG_FLAGS = $(CFLAGS) -g -O0 -DDEBUG=1
HEADER_DIR = include/
HEADER = $(addprefix $(HEADER_DIR), minishell.h)
NAME = minishell
DEBUG_NAME = debug_minishell

# ==== dir var setup ====
SRC_DIR = src/
OBJ_DIR = obj/
DEBUG_OBJ_DIR = obj_debug/

# Release object directories
REL_AST_OBJ_DIR = $(OBJ_DIR)ast/
REL_CMD_OBJ_DIR = $(OBJ_DIR)commands/
REL_HLP_OBJ_DIR = $(OBJ_DIR)helper/
REL_PRS_OBJ_DIR = $(OBJ_DIR)parser/
REL_OBJ_DIRS = $(REL_AST_OBJ_DIR) $(REL_CMD_OBJ_DIR) $(REL_HLP_OBJ_DIR) $(REL_PRS_OBJ_DIR)

# Debug object directories
DBG_AST_OBJ_DIR = $(DEBUG_OBJ_DIR)ast/
DBG_CMD_OBJ_DIR = $(DEBUG_OBJ_DIR)commands/
DBG_HLP_OBJ_DIR = $(DEBUG_OBJ_DIR)helper/
DBG_PRS_OBJ_DIR = $(DEBUG_OBJ_DIR)parser/
DBG_OBJ_DIRS = $(DBG_AST_OBJ_DIR) $(DBG_CMD_OBJ_DIR) $(DBG_HLP_OBJ_DIR) $(DBG_PRS_OBJ_DIR)

# ==== files ====
AST_FILE_NAMES = execute_ast.c
CMD_FILE_NAMES = built_ins.c  export_utils.c  get_cmd_path.c
HLP_FILE_NAMES = env.c  itoa.c  loop.c \
				 ms_strchr.c  ms_strncat.c  ms_strncpy.c \
				 ms_strndup.c  print_err.c \
				 ms_isalpha.c ms_atoi.c ms_isdigit.c \
				 is_valid_identifier.c free_shell.c
PRS_FILE_NAMES = ast_utils.c \
				 free_utils.c \
				 split_cmd_flags.c \
				 count_token.c \
				 extract_cmd.c \
				 extract_redirect.c \
				 extract_utils.c \
				 expand_tkn_arr.c \
				 str_utils.c \
				 heredoc.c \
				 parser.c \
				 print_ast.c \
				 count_cmd_tokens.c \
				 expand_cmd.c \
				 extract_operator.c \
				 extract_subshell.c \
				 tkns_to_words.c \
				 gen_utils.c \
				 parser_utils.c \
				 structure_ast.c \
				 validate_pairs.c

# ==== generate release paths ====
REL_AST_OBJ_FILES = $(patsubst %.c, $(REL_AST_OBJ_DIR)%.o, $(AST_FILE_NAMES))
REL_CMD_OBJ_FILES = $(patsubst %.c, $(REL_CMD_OBJ_DIR)%.o, $(CMD_FILE_NAMES))
REL_HLP_OBJ_FILES = $(patsubst %.c, $(REL_HLP_OBJ_DIR)%.o, $(HLP_FILE_NAMES))
REL_PRS_OBJ_FILES = $(patsubst %.c, $(REL_PRS_OBJ_DIR)%.o, $(PRS_FILE_NAMES))
REL_MAIN_OBJ_FILE = $(OBJ_DIR)main.o
REL_OBJS = $(REL_AST_OBJ_FILES) $(REL_CMD_OBJ_FILES) $(REL_HLP_OBJ_FILES) $(REL_PRS_OBJ_FILES) $(REL_MAIN_OBJ_FILE)

# ==== generate debug paths ====
DBG_AST_OBJ_FILES = $(patsubst %.c, $(DBG_AST_OBJ_DIR)%.o, $(AST_FILE_NAMES))
DBG_CMD_OBJ_FILES = $(patsubst %.c, $(DBG_CMD_OBJ_DIR)%.o, $(CMD_FILE_NAMES))
DBG_HLP_OBJ_FILES = $(patsubst %.c, $(DBG_HLP_OBJ_DIR)%.o, $(HLP_FILE_NAMES))
DBG_PRS_OBJ_FILES = $(patsubst %.c, $(DBG_PRS_OBJ_DIR)%.o, $(PRS_FILE_NAMES))
DBG_MAIN_OBJ_FILE = $(DEBUG_OBJ_DIR)main.o
DBG_OBJS = $(DBG_AST_OBJ_FILES) $(DBG_CMD_OBJ_FILES) $(DBG_HLP_OBJ_FILES) $(DBG_PRS_OBJ_FILES) $(DBG_MAIN_OBJ_FILE)

# ==== targets ====
all: $(NAME) debug

debug: $(DEBUG_NAME)

$(NAME): $(REL_OBJS)
	$(CC) $(REL_OBJS) -lreadline -o $(NAME)

$(DEBUG_NAME): $(DBG_OBJS)
	$(CC) $(DBG_OBJS) -lreadline -o $(DEBUG_NAME)

# Release object compilation rules
$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER)
	@-mkdir -p $(dir $@)
	$(CC) $(RELEASE_FLAGS) -c $< -o $@

# Debug object compilation rules
$(DEBUG_OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER)
	@-mkdir -p $(dir $@)
	$(CC) $(DEBUG_FLAGS) -c $< -o $@

clean:
	-rm -f $(REL_OBJS) $(DBG_OBJS)

fclean: clean
	-rm -f $(NAME) $(DEBUG_NAME)

re: fclean all

# ==== misc ====
.PHONY: all debug clean fclean re
