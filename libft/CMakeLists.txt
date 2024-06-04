cmake_minimum_required(VERSION 3.28.0)

# Nom du projet
project(libft C)

# Définition des sources
set(SRC_PATH src)
set(OBJ_PATH obj)
set(HEAD include)

set(SRCS
        ${SRC_PATH}/ft_isalpha.c ${SRC_PATH}/ft_isdigit.c ${SRC_PATH}/ft_isalnum.c ${SRC_PATH}/ft_isascii.c ${SRC_PATH}/ft_isprint.c
        ${SRC_PATH}/ft_strlen.c ${SRC_PATH}/ft_memset.c ${SRC_PATH}/ft_bzero.c ${SRC_PATH}/ft_memcpy.c ${SRC_PATH}/ft_memmove.c
        ${SRC_PATH}/ft_strlcpy.c ${SRC_PATH}/ft_strlcat.c ${SRC_PATH}/ft_toupper.c ${SRC_PATH}/ft_isupper.c ${SRC_PATH}/ft_islower.c
        ${SRC_PATH}/ft_tolower.c ${SRC_PATH}/ft_strchr.c ${SRC_PATH}/ft_strrchr.c ${SRC_PATH}/ft_strcmp.c ${SRC_PATH}/ft_strncmp.c
        ${SRC_PATH}/ft_memchr.c ${SRC_PATH}/ft_memcmp.c ${SRC_PATH}/ft_strnstr.c ${SRC_PATH}/ft_atoi.c ${SRC_PATH}/ft_isspace.c
        ${SRC_PATH}/ft_substr.c ${SRC_PATH}/ft_strjoin.c ${SRC_PATH}/ft_strjoin_list.c ${SRC_PATH}/ft_charjoin.c ${SRC_PATH}/ft_strtrim.c ${SRC_PATH}/ft_split.c ${SRC_PATH}/ft_str_count_char.c
        ${SRC_PATH}/ft_str_count_chars.c ${SRC_PATH}/ft_nbrlen.c ${SRC_PATH}/ft_itoa.c ${SRC_PATH}/ft_setitoa.c ${SRC_PATH}/ft_dtoa.c ${SRC_PATH}/ft_setdtoa.c ${SRC_PATH}/ft_strmapi.c ${SRC_PATH}/ft_striteri.c
        ${SRC_PATH}/ft_putchar_fd.c ${SRC_PATH}/ft_putstr_fd.c ${SRC_PATH}/ft_putendl_fd.c ${SRC_PATH}/ft_putnbr_fd.c ${SRC_PATH}/ft_strdup.c
        ${SRC_PATH}/ft_calloc.c ${SRC_PATH}/ft_putnbr_base_fd.c ${SRC_PATH}/ft_putlnbr_fd.c ${SRC_PATH}/ft_strlentab.c ${SRC_PATH}/ft_realloc.c
        ${SRC_PATH}/ft_putnbr_unsigned_fd.c ${SRC_PATH}/ft_putpointer_fd.c ${SRC_PATH}/ft_count_words.c ${SRC_PATH}/ft_putnbr_float_fd.c ${SRC_PATH}/get_lines.c
        ${SRC_PATH}/lst/ft_lstnew.c ${SRC_PATH}/lst/ft_lstadd_front.c
        ${SRC_PATH}/lst/ft_lstsize.c ${SRC_PATH}/lst/ft_lstlast.c ${SRC_PATH}/lst/ft_lstadd_back.c ${SRC_PATH}/ft_trunc.c
        ${SRC_PATH}/lst/ft_lstdelone.c ${SRC_PATH}/lst/ft_lstclear.c ${SRC_PATH}/lst/ft_lstiter.c ${SRC_PATH}/lst/ft_lstmap.c ${SRC_PATH}/lst/ft_lstprint.c
        ${SRC_PATH}/get_next_line/get_next_line.c
        ${SRC_PATH}/ft_printf/ft_printf.c
        ${SRC_PATH}/mem/ft_freetab.c
)

# Définir l'exécutable de l'archive
add_library(${PROJECT_NAME} STATIC ${SRCS})

# Options de compilation
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE ${HEAD})

# Règles de nettoyage
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${OBJ_PATH})

# Définir les commandes de nettoyage
add_custom_target(clean-all COMMAND ${CMAKE_COMMAND} -E rm -rf ${OBJ_PATH} ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
