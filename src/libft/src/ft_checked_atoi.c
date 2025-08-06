/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checked_atoi.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:59:45 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/18 21:20:51 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

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
int	ft_checked_atoi(const char *str, int *ret, int flags)
{
	long	n;
	long	sign;
	int		i;

	i = 0;
	flags = flags * (flags != 42) + 19 * (flags == 42);
	while (ft_isspace(str[i]) && flags & (1 << 0))
		i++;
	sign = -2 * (str[i] == '-') + 1;
	if (str[i] == '-' && !(flags & (1 << 1)))
		return (-1);
	n = ((str[i] == '+' || str[i] == '-') && i++) * 0;
	if (!ft_isdigit(str[i]) && !(flags & (1 << 2)))
		return (-1);
	while (ft_isdigit(str[i]))
	{
		n = n * 10 + str[i++] - '0';
		if ((n * sign > INT_MAX || n * sign < INT_MIN))
			return (-1);
	}
	while (str[i] == ' ' && ((1 << 4) & flags))
		i++;
	if (str[i] && !((1 << 3) & flags))
		return (-1);
	return (*ret = n * sign, 0);
}
