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

NAME		= libft.a

SRCS		= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			  ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
			  ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_isupper.c ft_islower.c \
			  ft_tolower.c ft_strchr.c ft_strrchr.c ft_strcmp.c ft_strncmp.c \
			  ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_isspace.c \
			  ft_substr.c ft_strjoin.c ft_strjoin_list.c ft_charjoin.c ft_strtrim.c ft_split.c ft_str_count_char.c \
			  ft_str_count_chars.c ft_nbrlen.c ft_itoa.c ft_setitoa.c ft_dtoa.c ft_setdtoa.c ft_strmapi.c ft_striteri.c \
			  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strdup.c \
			  ft_calloc.c ft_putnbr_base_fd.c ft_putlnbr_fd.c ft_strlentab.c ft_realloc.c \
			  ft_putnbr_unsigned_fd.c ft_putpointer_fd.c ft_count_words.c ft_putnbr_float_fd.c get_lines.c \
			  lst${DIRSEP}ft_lstnew.c lst${DIRSEP}ft_lstadd_front.c \
			  lst${DIRSEP}ft_lstsize.c lst${DIRSEP}ft_lstlast.c lst${DIRSEP}ft_lstadd_back.c ft_trunc.c \
			  lst${DIRSEP}ft_lstdelone.c lst${DIRSEP}ft_lstclear.c lst${DIRSEP}ft_lstiter.c lst${DIRSEP}ft_lstmap.c lst${DIRSEP}ft_lstprint.c \
			  get_next_line${DIRSEP}get_next_line.c \
			  ft_printf${DIRSEP}ft_printf.c \
			  mem${DIRSEP}ft_freetab.c

OBJ_PATH	= obj${DIRSEP}

SRC_PATH	= src${DIRSEP}

OBJ_NAME	= ${addprefix ${SRC_PATH}, ${SRCS:%.c=%.o}}

OBJS		= ${addprefix ${OBJ_PATH}, ${OBJ_NAME}}

CC			= gcc

HEAD		= include

CFLAGS		= -g -Wall -Wextra -Werror -I ${HEAD}

${OBJ_PATH}%.o: %.c
	@mkdir -p ${@D} 2> /dev/null || true
	@${CC} ${CFLAGS} -o $@ -c $<

${NAME}: ${OBJS}
	@ar cr ${NAME} ${OBJS}

all: ${NAME}

clean: 
	@${RM} -r ${OBJ_PATH} 2> /dev/null || true

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
