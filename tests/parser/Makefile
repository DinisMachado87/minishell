# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/18 21:45:56 by dimachad          #+#    #+#              #
#    Updated: 2025/08/18 21:46:21 by dimachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = parser
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0
HEADER_DIR = ../../include
HEADER = $(HEADER_DIR)/minishell.h
SRC_DIR = ../../src
HELPER_DIR = $(SRC_DIR)/helper
PARSER_DIR = $(SRC_DIR)/parser

# Source files without directory prefixes
HELPER_FILES = itoa.c
PARSER_FILES = ast_utils.c extract_operator.c extract_utils.c parser_utils.c \
			   structure_ast.c extract_cmd.c extract_subshell.c gen_utils.c \
			   parser.c print_ast.c heredoc.c extract_redirect.c main_parser.c

# Source files with directory prefixes
HELPER_SRCS = $(addprefix $(HELPER_DIR)/, $(HELPER_FILES))
PARSER_SRCS = $(addprefix $(PARSER_DIR)/, $(PARSER_FILES))

ALL_SRCS = $(HELPER_SRCS) $(PARSER_SRCS)
ALL_OBJS = $(ALL_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(ALL_OBJS)
	$(CC) $(CFLAGS) $(ALL_OBJS) -lreadline -o $(NAME)

# Object file compilation rules
$(HELPER_DIR)/%.o: $(HELPER_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(ALL_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
