ifeq ($(OS), Windows_NT)
	DIRSEP	= \\
	CP		= copy
	RM		= del -f
else
	DIRSEP	= /
	CP		= cp
	RM		= rm -f
endif

NAME			= minishell

#################################### SOURCES ####################################

SRCS			= main.c

SRCS_COMMAND	= history.c executor.c

SRCS_HANDLER	= sig_handler.c raw.c

#################################################################################

SRCS_COMMAND	:= ${addprefix command${DIRSEP}, $(SRCS_COMMAND)}

SRCS_HANDLER	:= ${addprefix sig${DIRSEP}, $(SRCS_HANDLER)}

SRCS			+= ${SRCS_COMMAND} ${SRCS_HANDLER}

SRCS			:= ${addprefix src${DIRSEP}, ${SRCS}}

OBJ_PATH		= obj${DIRSEP}

OBJ_NAME		= ${SRCS:%.c=%.o}

OBJS			= ${addprefix ${OBJ_PATH}, ${OBJ_NAME}}

CC				= gcc

HEAD			= include

LIBFT_DIR		= libft

CFLAGS			= -I ${HEAD}

# DEBUG
DEBUG ?= 0
ifeq ($(DEBUG), 0)
	CFLAGS += -Wall -Wextra -Werror
else
	CFLAGS += -g -D DEBUG=$(DEBUG)
endif

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
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS} -L ${LIBFT_DIR} -lft -lreadline
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

run:
	@clear && $(MAKE) re DEBUG=1 && valgrind --leak-check=full --show-leak-kinds=all ./minishell

norm:
	@norminette | grep Error || echo "$(GREEN)Success"

.PHONY: all clean fclean re run norm
