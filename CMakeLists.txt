cmake_minimum_required(VERSION 3.28)
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
        src/terminal/history.c
        src/terminal/executor.c
        src/terminal/terminal.c
        src/sig/raw.c
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
