/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:03:02 by anddokhn          #+#    #+#             */
/*   Updated: 2025/05/05 14:12:13 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include "../libft.h"

int	ft_strnlen(char *s, int n)
{
	int	i;

	i = 0;
	while (i < n && s[i])
		i++;
	return (i);
}

void	ft_fdputmem(int fd, char *s, int n)
{
	int	i;
	int	written;

	written = 0;
	while (written < n)
	{
		i = write(fd, s + written, n - written);
		if (i <= 0)
			break ;
		written += i;
	}
}

void	ft_putmem(char *s, int n)
{
	ft_fdputmem(1, s, n);
}

void	ft_eputmem(char *s, int n)
{
	ft_fdputmem(2, s, n);
}

bool	str_slice_eq_str(char *s, size_t len, char *s2)
{
	size_t	len_s2;

	len_s2 = ft_strlen(s2);
	if (len != len_s2)
		return (false);
	if (ft_strncmp(s, s2, len) == 0)
		return (true);
	return (false);
}
