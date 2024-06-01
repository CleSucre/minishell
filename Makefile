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

CFLAGS		= -g -I ${HEAD} -Wall -Wextra -Werror

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
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS}
	@echo "$(GREEN)$(NAME) has been created successfully.$(DEFCOLOR)"


clean:
	@${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true
	@echo "$(PURPLE)Object files have been removed.$(DEFCOLOR)"

fclean:
	@${RM} -r ${OBJ_PATH} 2> ${DIRSEP}dev${DIRSEP}null || true
	@${RM} ${NAME}
	@echo "$(RED)$(NAME) has been removed.$(DEFCOLOR)"

re: fclean all

.PHONY: all clean fclean re
