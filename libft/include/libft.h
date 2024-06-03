/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:07:53 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:07:53 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# define WHITESPACES " \t\n\v\f\r"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// ========LIBFT========

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
size_t	ft_strlentab(const char **s);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
int		ft_toupper(int c);
int		ft_isupper(int c);
int		ft_tolower(int c);
int		ft_islower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
int		ft_isspace(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_list(char const **strs);
char	*ft_charjoin(char *str, char c);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *str, char const *charset);
size_t	ft_str_count_char(char const *str, int c);
size_t	ft_str_count_chars(char const *s1, char const *s2);
size_t	ft_nbrlen(long n);
char	*ft_itoa(int n);
char	*ft_setitoa(char *dst, int n);
char	*ft_dtoa(double n, int precision);
void	*ft_setdtoa(char *dst, double n, int precision);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_putchar_fd(char c, int fd);
int		ft_putstr_fd(char *str, int fd);
int		ft_putendl_fd(char *s, int fd);
int		ft_putnbr_fd(int n, int fd);
char	*ft_strdup(const char *s1);
void	*ft_calloc(size_t count, size_t size);
void	ft_realloc(char **s1, const char *s2);
int		ft_putnbr_base_fd(int nbr, char *base, int fd);
int		ft_putlnbr_fd(long n, int fd);
int		ft_putnbr_unsigned_fd(unsigned int nb, int fd);
int		ft_putnbr_float_fd(double n, int fd);
int		ft_putpointer_fd(void *ptr, int fd);
int		ft_count_words(char const *str, char const *charset);
void	ft_trunc(char **str, size_t era);

// ========LIST========

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstprint(t_list *lst);

// ========GET_NEXT_LINE========

char	*get_next_line(int fd);
char	**get_lines(int fd);

// ========PRINTF========

int		ft_printf(const char *format, ...);
int		ft_fprintf(int fd, const char *format, ...);

// ========MEMORY========

int		ft_freetab(char **tab);

#endif
