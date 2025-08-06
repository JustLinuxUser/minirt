/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:04:11 by anddokhn          #+#    #+#             */
/*   Updated: 2025/05/05 14:12:32 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <stdbool.h>
# include <sys/types.h>

/* string utils */
int		ft_strnlen(char *s, int n);
// write a slice of memory to stdout
void	ft_putmem(char *ptr, int size);
void	ft_fdputmem(int fd, char *s, int n);
void	ft_eputmem(char *ptr, int size);
bool	str_slice_eq_str(char *s, size_t len, char *s2);

/* math utils */
ssize_t	ft_abs(ssize_t n);
ssize_t	ft_smin(ssize_t a, ssize_t b);
size_t	ft_min(size_t a, size_t b);
size_t	ft_max(size_t a, size_t b);
ssize_t	ft_smax(ssize_t a, ssize_t b);
ssize_t	ft_smod(ssize_t a, size_t b);

/* misc */
void	ft_assert(int cond);
#endif
