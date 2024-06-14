#############################################################################
#									OS										#
#############################################################################

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

#############################################################################
#									SOURCES									#
#############################################################################

SRCS					= main.c

SRCS_CONFIG				= term_config.c

SRCS_DEBUG				= debug_execution.c debug_history.c debug_parsing.c

SRCS_HISTORY			= history_file.c history_management.c history_navigation.c

SRCS_EXECUTION			= execution.c

SRCS_MEMORY				= memory_alloc.c memory_free.c

SRCS_PARSING			= ast_creation.c ast_management.c parser.c tokenizer.c

SRCS_TERMINAL			= terminal.c terminal_action.c terminal_info.c erase_put.c

#############################################################################
#									FOLDERS									#
#############################################################################

SRCS_CONFIG				:= $(addprefix config$(DIRSEP), $(SRCS_CONFIG))

SRCS_DEBUG				:= $(addprefix debug$(DIRSEP), $(SRCS_DEBUG))

SRCS_HISTORY			:= $(addprefix history$(DIRSEP), $(SRCS_HISTORY))

SRCS_EXECUTION			:= $(addprefix execution$(DIRSEP), $(SRCS_EXECUTION))

SRCS_MEMORY				:= $(addprefix memory$(DIRSEP), $(SRCS_MEMORY))

SRCS_PARSING			:= $(addprefix parsing$(DIRSEP), $(SRCS_PARSING))

SRCS_TERMINAL			:= $(addprefix terminal$(DIRSEP), $(SRCS_TERMINAL))

SRCS					+= $(SRCS_CONFIG) $(SRCS_DEBUG) $(SRCS_HISTORY) \
							$(SRCS_EXECUTION) $(SRCS_MEMORY) $(SRCS_PARSING) \
							$(SRCS_TERMINAL)

SRCS					:= $(addprefix src$(DIRSEP), $(SRCS))

#############################################################################

OBJ_PATH		= obj$(DIRSEP)

OBJ_NAME		= $(SRCS:%.c=%.o)

OBJS			= $(addprefix $(OBJ_PATH), $(OBJ_NAME))

CC				= gcc

HEAD			= include

LIBFT_DIR		= libft

CFLAGS			= -I $(HEAD) -MMD -MP

# DEBUG
DEBUG ?= 0
ifeq ($(DEBUG), 0)
	CFLAGS += -Wall -Wextra -Werror
else
	CFLAGS += -g -D DEBUG=$(DEBUG)
endif

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# COLORS
DEFCOLOR = \033[0;39m
GREEN    = \033[1;92m
YELLOW   = \033[0;93m
PURPLE   = \033[0;95m
RED		 = \033[0;91m

all: $(NAME)

$(OBJ_PATH)%.o : %.c
	@mkdir -p $(@D) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@echo "$(YELLOW)Compiling $< $(DEFCOLOR)"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L $(LIBFT_DIR) -lft -lncurses
	@echo "$(GREEN)$(NAME) has been created successfully.$(DEFCOLOR)"


clean:
	@make -C $(LIBFT_DIR) clean
	@$(RM) -r $(OBJ_PATH) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@echo "$(PURPLE)Object files have been removed.$(DEFCOLOR)"

fclean:
	@make -C $(LIBFT_DIR) fclean
	@$(RM) -r $(OBJ_PATH) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME) has been removed.$(DEFCOLOR)"

re: fclean all

run:
	$(MAKE) DEBUG=0 && ./minishell

debug:
	$(MAKE) DEBUG=1 && $(VALGRIND) ./minishell

norm:
	@norminette src libft | grep Error || echo "$(GREEN)Success"

.PHONY: all clean fclean re run debug norm
