/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:29:34 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/20 20:14:32 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdint.h>
# include <stdlib.h>
# include "gnl/get_next_line.h"
# include "ft_printf/ft_printf.h"
# include "utils/utils.h"
# include "dsa/vec_dyn_str.h"
# include "alloc/alloc.h"

size_t		ft_strlen(const char *str);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strnstr(const char *str, const char *to_find, size_t len);
bool		str_ends_with(const char *haystack, const char *needle);

void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);

void		ft_memswap(void *a, void *b, size_t size);

int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);

int			ft_toupper(int c);
int			ft_tolower(int c);

char		*ft_strchr(const char *s, int c);
char		*ft_strnchr(const char *s, int c, int len);
char		*ft_strrchr(const char *s, int c);

int			ft_atoi(const char *str);

/*
 * bit 0 - allow spaces
 *
 * bit 1 - allow negative
 *
 * bit 2 - allow empty numbers
 *
 * bit 3 - allow trailing chars
 *
 * bit 4 - allow trailing spaces
 */
int			ft_checked_atoi(const char *str, int *ret, int flags);

void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, int len);

char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);

char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		**ft_split(char *str, char c);
char		**ft_split_str(char *str, char *sep);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}			t_list;

t_list		*ft_lstnew(void *content);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_front(t_list **lst, t_list *nw);
void		ft_lstadd_back(t_list **lst, t_list *nw);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// rand
typedef struct s_prng_state
{
	uint32_t	state_array[624];
	int			state_index;
}	t_prng_state;

void		prng_initialize_state(t_prng_state *state, uint32_t seed);
uint32_t	random_uint32(t_prng_state *state);
uint64_t	xorshift64(uint64_t *state);
#endif
