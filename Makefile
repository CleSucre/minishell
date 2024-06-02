ifeq ($(OS), Windows_NT)
	DIRSEP	= \\
	CP		= copy
	RM		= del -f
else
	DIRSEP	= /
	CP		= cp
	RM		= rm -f
endif

NAME		= minishell

SRCS		= src${DIRSEP}main.c

OBJ_PATH	= obj${DIRSEP}

OBJ_NAME	= ${SRCS:%.c=%.o}

OBJS		= ${addprefix ${OBJ_PATH}, ${OBJ_NAME}}

CC			= gcc

HEAD		= include

LIBFT_DIR	= libft

CFLAGS		= -I ${HEAD} -Wall -Wextra -Werror

# DEBUG
DEBUG ?= 0
CFLAGS += -g -D DEBUG=$(DEBUG)

# COLORS
DEFCOLOR = \033[0;39m
GREEN    = \033[1;92m
YELLOW   = \033[0;93m
PURPLE   = \033[0;95m
RED		 = \033[0;91m


all: ${NAME}

${OBJ_PATH}%.o : %.c
	@mkdir -p ${@D} 2> ${DIRSEP}dev${DIRSEP}null || true
	@echo "$(YELLOW)Compiling $< $(DEFCOLOR)"
	@${CC} ${CFLAGS} -o $@ -c $<

${NAME}: ${OBJS}
	@make -C ${LIBFT_DIR}
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS} -L ${LIBFT_DIR} -lft
	@echo "$(GREEN)$(NAME) has been created successfully.$(DEFCOLOR)"


clean:
	@make -C ${LIBFT_DIR} clean
	@${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true
	@echo "$(PURPLE)Object files have been removed.$(DEFCOLOR)"

fclean:
	@make -C ${LIBFT_DIR} fclean
	@${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true
	@${RM} ${NAME}
	@echo "$(RED)$(NAME) has been removed.$(DEFCOLOR)"

re: fclean all

run: all
	@clear && make re DEBUG=1 && valgrind --leak-check=full --show-leak-kinds=all ./minishell

.PHONY: all clean fclean re run