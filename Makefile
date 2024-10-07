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

SRCS_DEBUG				= debug_execution.c debug_history.c debug_parsing.c

SRCS_ENVIRONMENT		= env_variable.c

SRCS_HISTORY			= history_file.c history_management.c history_navigation.c

SRCS_EXECUTION			= execute_cmd.c execution_types.c execute_path.c execution.c signal_handler.c redirections.c

SRCS_COMMANDS			= command_maker.c flags_utils.c command_manager.c

SRCS_COMMANDS_CUSTOM	= cd.c echo.c env.c export.c exit.c history.c pwd.c unset.c

SRCS_MEMORY				= memory_alloc.c memory_free.c builtins_alloc.c

SRCS_PARSING			= ast_management.c parsing.c parsing_args.c parsing_mandatory.c parsing_bonus.c var_creation.c

SRCS_PARSING_TOKENIZER	= tokenizer.c token_management.c

SRCS_TERMINAL			= input_utils.c terminal.c terminal_action.c terminal_arrow.c terminal_cursor.c terminal_info.c terminal_prompt.c terminal_signals.c terminal_utils.c

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

SRCS					+= $(SRCS_CONFIG) $(SRCS_ENVIRONMENT) $(SRCS_DEBUG) $(SRCS_HISTORY) \
							$(SRCS_COMMANDS) $(SRCS_COMMANDS_CUSTOM) $(SRCS_EXECUTION) \
							$(SRCS_MEMORY) $(SRCS_PARSING) $(SRCS_TOKENIZER) $(SRCS_TERMINAL) $(SRCS_ATCP)

SRCS					:= $(addprefix src$(DIRSEP), $(SRCS))

SRCS_TESTS				= $(SRCS_CONFIG) $(SRCS_ENVIRONMENT) $(SRCS_DEBUG) $(SRCS_HISTORY) \
							$(SRCS_COMMANDS) $(SRCS_COMMANDS_CUSTOM) $(SRCS_EXECUTION) \
							$(SRCS_MEMORY) $(SRCS_PARSING) $(SRCS_TOKENIZER) $(SRCS_TERMINAL) $(SRCS_ATCP)

SRCS_TESTS				:= $(addprefix src$(DIRSEP), $(SRCS_TESTS))

SRCS_TESTS				+= tests$(DIRSEP)src$(DIRSEP)main.cpp tests$(DIRSEP)src$(DIRSEP)tester$(DIRSEP)TesterQuote.cpp tests$(DIRSEP)src$(DIRSEP)tester$(DIRSEP)TesterStrSplit.cpp

#############################################################################

OBJ_PATH		= obj$(DIRSEP)

OBJ_NAME		= $(SRCS:%.c=%.o)

OBJS			= $(addprefix $(OBJ_PATH), $(OBJ_NAME))

# .cpp & .c to .o
OBJ_NAME_CPP    = $(SRCS_TESTS:%.c=%.o)
OBJ_NAME_CPP    := $(OBJ_NAME_CPP:%.cpp=%.o)

OBJS_CPP        = $(addprefix $(OBJ_PATH), $(OBJ_NAME_CPP))

CC				= gcc

CXX				= c++

HEAD			= include

LIBFT_DIR		= libft

CFLAGS			= -I $(HEAD) -MMD -MP

CXXFLAGS        = $(CFLAGS) -std=c++17 -I tests/include

# DEBUG
DEBUG ?= 0
ifeq ($(DEBUG), 0)
	CFLAGS += -Wall -Wextra -Werror
	CXXFLAGS += -Wall -Wextra -Werror
else
	CFLAGS += -g -D DEBUG=$(DEBUG)
	CXXFLAGS += -g -D DEBUG=$(DEBUG)
endif

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=valgrind.supp

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
	@norminette src libft | grep Error || echo "$(GREEN)Success"

#############################################################################
#									TESTS									#
#############################################################################

$(OBJ_PATH)%.o: %.cpp
	@mkdir -p $(@D) 2> $(DIRSEP)dev$(DIRSEP)null || true
	@echo "$(YELLOW)Compiling $< $(DEFCOLOR)"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

tests: $(OBJS_CPP) libft
	@$(CXX) $(CXXFLAGS) -o test $(OBJS_CPP) -L $(LIBFT_DIR) -lft
	@echo "$(GREEN)Tests have been created successfully.$(DEFCOLOR)"
	./test
	rm -f test

.PHONY: libft libft_clean libft_fclean all clean fclean re run debug norm tests
