cmake_minimum_required(VERSION 3.27)
project(minishell)

# Compiler settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_DEBUG "-g -D DEBUG=0")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Werror")

# Include directories
include_directories(include)

# Source files
set(SRCS
        src/main.c

        src/terminal/erase_put.c
        src/terminal/terminal.c
        src/terminal/terminal_action.c
        src/terminal/terminal_arrow.c
        src/terminal/terminal_cursor.c
        src/terminal/terminal_info.c
        src/terminal/terminal_prompt.c
        src/terminal/terminal_utils.c

        src/parsing/parsing_args.c
        src/parsing/ast_management.c
        src/parsing/parsing.c
        src/parsing/tokenizer.c

        src/memory/memory_alloc.c
        src/memory/memory_free.c

        src/history/history_file.c
        src/history/history_management.c
        src/history/history_navigation.c

        src/execution/execution.c

        src/environment/env_variable.c

        src/debug/debug_execution.c
        src/debug/debug_history.c
        src/debug/debug_parsing.c

        src/config/term_config.c

        src/commands/command_maker.c

        src/commands/custom/cd.c
        src/commands/custom/echo.c
        src/commands/custom/env.c
        src/commands/custom/exit.c
        src/commands/custom/export.c
        src/commands/custom/history.c
        src/commands/custom/pwd.c
        src/commands/custom/unset.c
)

# Object files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# External libraries
set(LIBFT_DIR ${CMAKE_SOURCE_DIR}/libft)
add_subdirectory(${LIBFT_DIR})
include_directories(${LIBFT_DIR}/include)

# Executable
add_executable(${PROJECT_NAME} ${SRCS})

# Link libraries (libft)
target_link_libraries(${PROJECT_NAME} PRIVATE libft)
