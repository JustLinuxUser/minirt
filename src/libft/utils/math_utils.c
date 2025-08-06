/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:07:27 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/15 04:01:03 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

ssize_t	ft_abs(ssize_t n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

ssize_t	ft_smin(ssize_t a, ssize_t b)
{
	return ((a > b) * b + (a <= b) * a);
}

size_t	ft_min(size_t a, size_t b)
{
	return ((a > b) * b + (a <= b) * a);
}

size_t	ft_max(size_t a, size_t b)
{
	return ((a < b) * b + (a >= b) * a);
}

ssize_t	ft_smax(ssize_t a, ssize_t b)
{
	return ((a < b) * b + (a >= b) * a);
}
