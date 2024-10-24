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

NAME					= minishell

#############################################################################
#									SOURCES									#
#############################################################################

SRCS					= main.c

SRCS_CONFIG				= term_config.c

SRCS_DEBUG				= debug_execution.c debug_history.c debug_parsing.c debug_parsing_utils.c

SRCS_ENVIRONMENT		= env_utils.c path_utils.c variable_utils.c variable_replacer.c

SRCS_HISTORY			= history_file.c history_management.c history_navigation.c

SRCS_EXECUTION			= execute_cmd.c execution_or_and.c execution_types.c execute_path.c execution.c execution_fork.c \
 						  execution_heredoc.c signal_handler.c redirections_input.c redirections_output.c execution_pipes.c pipes_utils.c \
 						  execution_heredoc_utils.c execution_heredoc_pid.c execution_verif.c

SRCS_COMMANDS			= command_maker.c flags_utils.c command_manager.c

SRCS_COMMANDS_CUSTOM	= cd.c cd_utils.c echo.c env.c exit.c history.c pwd.c unset.c export.c export_add.c exit_utils.c

SRCS_MEMORY				= memory_alloc.c memory_free.c builtins_alloc.c

SRCS_PARSING			= ast_management.c parsing.c parsing_mandatory.c parsing_redirection.c parsing_bonus.c parsing_utils.c var_creation.c

SRCS_PARSING_TOKENIZER	= token_extractor.c tokenizer.c token_management.c tokenizer_quote_handler.c split_with_quote.c

SRCS_TERMINAL			= input_utils.c terminal.c terminal_action.c terminal_arrow.c terminal_cursor.c terminal_info.c terminal_prompt.c terminal_signals.c terminal_utils.c \
							put_in_string.c

SRCS_WILDCARD			= wildcard.c wildcard_utils.c

SRCS_ATCP				= tab.c dictionnary/setup_dico.c dictionnary/bst.c dictionnary/bst_insert.c dictionnary/free_bst.c dictionnary/print_bst.c \
							dictionnary/copy_cut_bst.c

#############################################################################
#									FOLDERS									#
#############################################################################

SRCS_ATCP				:= $(addprefix atcp$(DIRSEP), $(SRCS_ATCP))

SRCS_CONFIG				:= $(addprefix config$(DIRSEP), $(SRCS_CONFIG))

SRCS_ENVIRONMENT		:= $(addprefix environment$(DIRSEP), $(SRCS_ENVIRONMENT))

SRCS_DEBUG				:= $(addprefix debug$(DIRSEP), $(SRCS_DEBUG))

SRCS_HISTORY			:= $(addprefix history$(DIRSEP), $(SRCS_HISTORY))

SRCS_EXECUTION			:= $(addprefix execution$(DIRSEP), $(SRCS_EXECUTION))

SRCS_COMMANDS			:= $(addprefix commands$(DIRSEP), $(SRCS_COMMANDS))

SRCS_COMMANDS_CUSTOM	:= $(addprefix commands$(DIRSEP), $(addprefix custom$(DIRSEP), $(SRCS_COMMANDS_CUSTOM)))

SRCS_MEMORY				:= $(addprefix memory$(DIRSEP), $(SRCS_MEMORY))

SRCS_PARSING			:= $(addprefix parsing$(DIRSEP), $(SRCS_PARSING))

SRCS_TOKENIZER			:= $(addprefix parsing$(DIRSEP), $(addprefix tokenizer$(DIRSEP), $(SRCS_PARSING_TOKENIZER)))

SRCS_TERMINAL			:= $(addprefix terminal$(DIRSEP), $(SRCS_TERMINAL))

SRCS_WILDCARD			:= $(addprefix wildcard$(DIRSEP), $(SRCS_WILDCARD))

SRCS					+= $(SRCS_CONFIG) $(SRCS_ENVIRONMENT) $(SRCS_DEBUG) $(SRCS_HISTORY) \
							$(SRCS_COMMANDS) $(SRCS_COMMANDS_CUSTOM) $(SRCS_EXECUTION) \
							$(SRCS_MEMORY) $(SRCS_PARSING) $(SRCS_TOKENIZER) $(SRCS_TERMINAL) $(SRCS_WILDCARD) $(SRCS_ATCP)

SRCS					:= $(addprefix src$(DIRSEP), $(SRCS))

#############################################################################

OBJ_PATH		= obj$(DIRSEP)

OBJ_NAME		= $(SRCS:%.c=%.o)

OBJS			= $(addprefix $(OBJ_PATH), $(OBJ_NAME))

CC				= cc

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

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes # --suppressions=vsupp

# COLORS
DEFCOLOR = \033[0;39m
GREEN    = \033[1;92m
YELLOW   = \033[0;93m
PURPLE   = \033[0;95m
RED		 = \033[0;91m

$(NAME): $(OBJS) libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L $(LIBFT_DIR) -lft -lncurses
	@echo "$(GREEN)$(NAME) has been created successfully.$(DEFCOLOR)"

libft:
	@make -C $(LIBFT_DIR)

libft_clean:
	@make -C $(LIBFT_DIR) clean

libft_fclean:
	@make -C $(LIBFT_DIR) fclean

all: $(NAME)

$(OBJ_PATH)%.o : %.c
	@mkdir -p $(@D) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@echo "$(YELLOW)Compiling $< $(DEFCOLOR)"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean: libft_clean
	@$(RM) -r $(OBJ_PATH) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@echo "$(PURPLE)Object files have been removed.$(DEFCOLOR)"

fclean: libft_fclean
	@$(RM) -r $(OBJ_PATH) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME) has been removed.$(DEFCOLOR)"

re: fclean all

run:
	$(MAKE) DEBUG=0
	./minishell

debug:
	$(MAKE) DEBUG=1 && $(VALGRIND) ./minishell

norm:
	@norminette -R CheckForbiddenSourceHeader  libft src include | grep Error || echo "$(GREEN)Success"

.PHONY: libft libft_clean libft_fclean all clean fclean re run debug norm
