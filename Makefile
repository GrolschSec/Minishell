# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rlouvrie <rlouvrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 14:37:37 by nagvaill          #+#    #+#              #
#    Updated: 2023/06/23 17:23:43 by rlouvrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		=	src/
OBJ_DIR		=	obj/
DIR_LIB		=	./libft
NAMELFT		=	./libft/libft.a
NAME		=	minishell
INCLUDE		=	-I./include -I./libft/include
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra
Make		=	Make

FILES = 	main					\
			path					\
			split_list				\
			split_list_utils		\
			assign_type				\
			parsing_cmd				\
			count_cmd				\
			fill_redirections		\
			fill_eof				\
			env 					\
			export					\
			echo					\
			exit					\
			exec					
			# unset					

SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

OBJF = .cache_exists

$(OBJF):
	@mkdir -p $(OBJ_DIR)

all: make_lib $(NAME)

make_lib : 
	@make -C $(DIR_LIB)

$(OBJ_DIR)%.o	: $(SRC_DIR)%.c | $(OBJF)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME)    :    $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline libft/libft.a -o $(NAME)

clean    :
	@rm -rf $(OBJ_DIR)
	@rm -f $(OBJF)
	@cd $(DIR_LIB) && $(MAKE) clean

fclean    :    clean
	@rm -f $(NAME)

re        :    fclean all

valgrind  :	   
	valgrind --track-fds=yes --log-file=leaks.txt --trace-children=yes --suppressions=rl_leaks.txt --leak-check=full --show-leak-kinds=all -s ./minishell

debug: CFLAGS += -g -O0
debug: all

.PHONY    :    all clean fclean re valgrind
