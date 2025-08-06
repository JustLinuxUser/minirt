/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:06:48 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/22 04:06:45 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>
#include "../utils/utils.h"

void	writer_hex_lowcase(t_buffer *buff, size_t n, int add_prefix, int digits)
{
	if (n != 0 && add_prefix)
	{
		writer_char(buff, '0');
		writer_char(buff, 'x');
	}
	if (n / 16 || digits > 1)
		writer_hex_lowcase(buff, ft_abs(n / 16), 0, digits - 1);
	if (digits > 0)
		writer_char(buff, "0123456789abcdef"[ft_abs(n % 16)]);
}

void	writer_hex_upcase(t_buffer *buff, size_t n, int add_prefix, int digits)
{
	if (n != 0 && add_prefix)
	{
		writer_char(buff, '0');
		writer_char(buff, 'X');
	}
	if (n / 16 || digits > 1)
		writer_hex_upcase(buff, ft_abs(n / 16), 0, digits - 1);
	if (digits > 0)
		writer_char(buff, "0123456789ABCDEF"[ft_abs(n % 16)]);
}

int	writer_init(t_buffer *ret)
{
	*ret = (t_buffer){0};
	ret->cap = 50;
	ret->buff = malloc(ret->cap + 1);
	if (!ret->buff)
		return (1);
	ret->buff[0] = 0;
	ret->out_fd = 1;
	return (0);
}
