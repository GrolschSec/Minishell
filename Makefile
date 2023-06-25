# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/23 19:11:07 by mrabourd          #+#    #+#              #
#    Updated: 2023/06/25 18:13:07 by mrabourd         ###   ########.fr        #
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
			add_nodes				\
			split_list_utils		\
			split_doublequote		\
			split_singlequote		\
			assign_type				\
			assign_type_utils		\
			parsing_cmd				\
			count_cmd				\
			fill_redirections		\
			fill_eof				\
			open_files				\
			env 					\
			export					\
			echo					\
			exit					\
			exec					\
			TEST_print				\
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
