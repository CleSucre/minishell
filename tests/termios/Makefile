# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: julthoma <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/02 17:07:47 by julthoma          #+#    #+#              #
#    Updated: 2023/11/02 17:07:47 by julthoma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(OS), Windows_NT)
	DIRSEP	= \\
	CP		= copy
	RM		= del -f
else
	DIRSEP	= /
	CP		= cp
	RM		= rm -f
endif

NAME		= termios

SRCS		= main.c

OBJ_PATH	= obj${DIRSEP}

SRC_PATH	= src${DIRSEP}

OBJ_NAME	= ${addprefix ${SRC_PATH}, ${SRCS:%.c=%.o}}

OBJS		= ${addprefix ${OBJ_PATH}, ${OBJ_NAME}}

CC			= gcc

HEAD		= include

LIBFT_DIR	= ../../libft

CFLAGS		= -I ${HEAD}

${OBJ_PATH}%.o : %.c
	mkdir -p ${@D} 2> /dev/null || true
	${CC} ${CFLAGS} -o $@ -c $<

${NAME}: ${OBJS}
	make -C ${LIBFT_DIR}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} -L ${LIBFT_DIR} -lft -lm

all: ${NAME}

clean: 
	make -C ${LIBFT_DIR} clean
	${RM} -r ${OBJ_PATH} 2> /dev/null || true

fclean:
	make -C ${LIBFT_DIR} fclean
	${RM} -r ${OBJ_PATH} 2> /dev/null || true
	${RM} ${NAME}
	${RM} ${NAME}_bonus

bonus: ${OBJS_BONUS}
	make -C ${LIBFT_DIR}
	${CC} ${CFLAGS} -o ${NAME}_bonus ${OBJS_BONUS} -L ${LIBFT_DIR} -lft -lm

re: fclean all

.PHONY: all clean fclean re bonus