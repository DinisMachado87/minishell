# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/18 21:47:15 by dimachad          #+#    #+#              #
#    Updated: 2025/08/18 21:56:55 by dimachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = norm
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0
HEADER_DIR = ../../include
HEADER = $(HEADER_DIR)/minishell.h
SRC_DIR = ../../src
PARSER_DIR = $(SRC_DIR)/parser

# Source files without directory prefixes
NORMALIZER_FILES = normalizer.c main_normalizer.c gen_utils.c parser_utils.c

# Source files with directory prefixes
NORMALIZER_SRCS = $(addprefix $(PARSER_DIR)/, $(NORMALIZER_FILES))

ALL_OBJS = $(NORMALIZER_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(ALL_OBJS)
	$(CC) $(CFLAGS) $(ALL_OBJS) -o $(NAME)

# Object file compilation rules
$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(ALL_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
