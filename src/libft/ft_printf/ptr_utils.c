/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:54:22 by anddokhn          #+#    #+#             */
/*   Updated: 2024/09/29 18:52:46 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include "ft_printf.h"

int	ptr_total_len(size_t n, t_fmt_spec spec)
{
	int	len;

	len = hex_digits(n);
	len += hex_padding_len(n, spec);
	if (spec.flags & FL_POUND && n)
		len += 2;
	if (spec.flags & FL_PLUS || spec.flags & FL_SPACE)
		len++;
	return (len);
}

t_fmt_spec	normalize_ptr_spec(t_fmt_spec spec)
{
	if (spec.precision != -1 || spec.flags & FL_MINUS)
		spec.flags &= ~FL_ZERO;
	if (spec.width != -1 && (spec.flags & FL_ZERO))
	{
		spec.precision = spec.width - 2;
		if (spec.flags & FL_PLUS || spec.flags & FL_SPACE)
			spec.precision--;
		spec.width = -1;
	}
	return (spec);
}
