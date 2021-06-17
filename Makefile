# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/15 21:33:47 by besellem          #+#    #+#              #
#    Updated: 2021/06/17 23:57:44 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COMPILATION

CC			= clang
CFLAGS 		= -Wall -Wextra -Werror -g3 -fsanitize=address
IFLAGS 		= -I./incs -I./libft/incs
LIBFLAGS 	= -L./libft -lft

# DIRECTORIES

BUILD		:= .build
LIB_DIR		:= libft
INC_DIR		:= incs
SRC_DIR		:= srcs
SUB_DIR		:= parser \
			   utils
OBJ_DIR 	:= $(BUILD)/obj
DIRS		:= $(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_DIR))

# FILES

NAME	:= ft_ls
SRC		:= main.c
SUB_SRC	:= parse_args.c
SRC		+= $(addprefix parser/, $(SUB_SRC))
SUB_SRC	:= general.c
SRC		+= $(addprefix utils/, $(SUB_SRC))

OBJ 	:= $(SRC:%.c=$(OBJ_DIR)/%.o)

# COLORS

DEFAULT_COLOR	= \033[0m
BLACK_COLOR		= \033[1;30m
RED_COLOR		= \033[1;31m
GREEN_COLOR		= \033[1;32m
YELLOW_COLOR	= \033[1;33m
BLUE_COLOR		= \033[1;34m
MAGENTA_COLOR 	= \033[1;35m
CYAN_COLOR 		= \033[1;36m
WHITE_COLOR 	= \033[1;107m

# MAKEFILE

$(NAME): $(OBJ)
	@echo "Compiling $(RED_COLOR)Libft $(DEFAULT_COLOR)..."
	@$(MAKE) -C $(LIB_DIR) >/dev/null
	@echo "Creating $(RED_COLOR) $@ $(DEFAULT_COLOR)..."
	@$(CC) $(CFLAGS) $(IFLAGS) $^ -o $@ $(LIBFLAGS)
	@echo "$(GREEN_COLOR)Compilation $(YELLOW_COLOR)of $(RED_COLOR)$@ $(BLUE_COLOR)done$(DEFAULT_COLOR)"

all: $(NAME)

bonus: all

clean:
	@echo "Deleting $(CYAN_COLOR).o file $(DEFAULT_COLOR)of libft ..."
	@$(MAKE) clean -C $(LIB_DIR) >/dev/null
	@echo "Deleting $(CYAN_COLOR).o file $(DEFAULT_COLOR)of ft_ls ..."
	@rm -rf $(BUILD)

fclean: clean
	@echo "Deleting $(CYAN_COLOR)libft $(DEFAULT_COLOR)file ..."
	@$(MAKE) fclean -C $(LIB_DIR) >/dev/null
	@echo "Deleting $(CYAN_COLOR)ft_ls $(DEFAULT_COLOR)file ..."
	@rm -rf $(NAME)
	@rm -rf $(NAME).dSYM

re: fclean all

$(BUILD):
	@echo "Creating $(RED_COLOR)$@ $(DEFAULT_COLOR)..."
	@mkdir $@ $(DIRS)
	
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(BUILD)
	@echo "Compiling $(YELLOW_COLOR)$< $(DEFAULT_COLOR)..."
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

.PHONY: all bonus clean fclean
