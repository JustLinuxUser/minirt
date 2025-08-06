/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:54:22 by anddokhn          #+#    #+#             */
/*   Updated: 2024/09/28 21:14:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include "ft_printf.h"

int	hex_digits(size_t n)
{
	int		count;

	count = 0;
	while (n != 0)
	{
		count++;
		n /= 16;
	}
	return (count);
}

int	hex_padding_len(size_t n, t_fmt_spec spec)
{
	int	digit_count;

	digit_count = hex_digits(n);
	if (spec.precision >= digit_count)
	{
		return (spec.precision - digit_count);
	}
	return (0);
}

int	hex_total_len(size_t n, t_fmt_spec spec)
{
	int	len;

	len = hex_digits(n);
	len += hex_padding_len(n, spec);
	if (spec.flags & FL_POUND && n)
		len += 2;
	return (len);
}

t_fmt_spec	normalize_hex_spec(t_fmt_spec spec, size_t n)
{
	if (spec.precision != -1 || spec.flags & FL_MINUS)
		spec.flags &= ~FL_ZERO;
	if (spec.width != -1 && (spec.flags & FL_ZERO))
	{
		if (spec.flags & FL_POUND && n)
		{
			spec.precision = spec.width - 2;
			spec.width = -1;
		}
		else
		{
			spec.precision = spec.width;
			spec.width = -1;
		}
	}
	if (spec.precision == -1 && n == 0)
		spec.precision = 1;
	return (spec);
}
