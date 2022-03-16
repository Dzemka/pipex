#TARGETS

NAME = pipex

NAME_BONUS = pipex_bonus

#DIRECTORIES

DIR_SRC = src/

DIR_OBJ = obj/

DIR_INC = include/

DIR_MANDATORY = ./pipex_mandatory_part/

DIR_BONUS = ./pipex_bonus_part/

#PATH

PATH_MANDATORY_INC = $(DIR_MANDATORY)$(DIR_INC)

PATH_BONUS_INC = $(DIR_BONUS)$(DIR_INC)

PATH_MANDATORY_SRC = $(DIR_MANDATORY)$(DIR_SRC)

PATH_BONUS_SRC = $(DIR_BONUS)$(DIR_SRC)

PATH_MANDATORY_OBJ = $(DIR_MANDATORY)$(DIR_OBJ)

PATH_BONUS_OBJ = $(DIR_BONUS)$(DIR_OBJ)

#HEADERS

HEADER_MANDATORY = $(NAME).h

HEADER_BONUS = $(NAME_BONUS).h

#SOURCE FILES

SRC = pipex.c ft_strncmp.c ft_split.c ft_strjoin.c pipex_utils.c

SRC_BONUS = pipex_bonus.c ft_strncmp_bonus.c ft_split_bonus.c ft_strjoin_bonus.c pipex_utils_bonus.c get_next_line.c \
	get_next_line_utils.c

#OBJECT FILES

OBJ = $(addprefix $(PATH_MANDATORY_OBJ), $(SRC:%.c=%.o))

OBJ_BONUS = $(addprefix $(PATH_BONUS_OBJ), $(SRC_BONUS:%.c=%.o))

#COMMANDS

CC = gcc

RM = rm -rf

#FLAGS

CFLAGS = -Werror -Wextra -Wall

.PHONY	:	all bonus clean fclean re

all	:	$(NAME)

$(NAME)	:	$(PATH_MANDATORY_OBJ) $(OBJ)
		$(CC) -o $@ $(OBJ)

$(PATH_MANDATORY_OBJ)	:
	mkdir -p $@

$(PATH_MANDATORY_OBJ)%.o 	:	$(addprefix $(PATH_MANDATORY_SRC), %.c) $(PATH_MANDATORY_INC)$(HEADER_MANDATORY)
	$(CC) $(CFLAGS) -c $< -o $@

bonus	:	$(NAME_BONUS)

$(NAME_BONUS)	:	$(PATH_BONUS_OBJ) $(OBJ_BONUS)
	$(CC) -o $@ $(OBJ_BONUS)

$(PATH_BONUS_OBJ)	:
	mkdir -p $@

$(PATH_BONUS_OBJ)%.o	:	$(addprefix $(PATH_BONUS_SRC), %.c) $(PATH_BONUS_INC)$(HEADER_BONUS)
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(RM) $(PATH_MANDATORY_OBJ) $(PATH_BONUS_OBJ)

fclean	:	clean
	$(RM) $(NAME) $(NAME_BONUS)

re 		:	fclean all