# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrabourd <mrabourd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/23 19:11:07 by mrabourd          #+#    #+#              #
#    Updated: 2023/07/18 15:37:51 by mrabourd         ###   ########.fr        #
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
			split_list				\
			split_list_utils		\
			split_quote				\
			split_in_prev_node		\
			split_meta				\
			assign_type				\
			assign_dollar			\
			assign_dollar_env		\
			assign_dollar_exitcode	\
			assign_redirections		\
			parsing_cmd				\
			count_cmd				\
			init_exec				\
			fill_redirections		\
			fill_eof				\
			open_files				\
			env						\
			error_message			\
			exit					\
			exec_1					\
			exec_2					\
			exec_3					\
			exec_utils_1			\
			exec_utils_2			\
			builtin_exit			\
			builtin_cd				\
			builtin_export			\
			builtin_echo			\
			builtin_pwd				\
			builtin_utils			\
			builtin_error			\
			builtin_unset			\
			builtin_env				\
			heredoc					\
			signal					\
			TEST_print							

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

tester : 
	@make -C ../minishell-tester

clean    :
	@rm -rf $(OBJ_DIR)
	@rm -f $(OBJF)
	@make fclean -C $(DIR_LIB)

fclean    :    clean
	@rm -f $(NAME)

re        :    fclean all

valgrind  :	   
	valgrind --track-fds=yes --log-file=leaks.txt --trace-children=yes --suppressions=rl_leaks.txt --leak-check=full --show-leak-kinds=all -s ./minishell

debug: CFLAGS += -g -O0
debug: all

.PHONY    :    all clean fclean re valgrind
