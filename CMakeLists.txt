cmake_minimum_required(VERSION 3.28)
project(minishell)

# Compiler settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG=0")
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

# Make sure the library name matches the one defined in libft's CMakeLists.txt or Makefile
target_link_libraries(${PROJECT_NAME} PRIVATE libft)

# Custom targets for valgrind
add_custom_target(run
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release .
        COMMAND ${CMAKE_COMMAND} --build .
        COMMAND ./minishell
        DEPENDS ${PROJECT_NAME}
)

add_custom_target(debug
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug .
        COMMAND ${CMAKE_COMMAND} --build .
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
        DEPENDS ${PROJECT_NAME}
)

# Custom clean target
add_custom_target(clean_all
        COMMAND ${CMAKE_COMMAND} --build . --target clean
        COMMAND ${CMAKE_COMMAND} -P cmake_clean.cmake
)

# Norminette
add_custom_target(norm
        COMMAND norminette | grep Error || echo "Success"
)

# Add commands for cleaning up
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake "
file(GLOB_RECURSE OBJECT_FILES \"\${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*\")
foreach(OBJECT_FILE \${OBJECT_FILES})
    file(REMOVE \${OBJECT_FILE})
endforeach()
")
