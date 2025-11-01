/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/18 21:19:18 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "dyn_str.h"
#include "../utils/utils.h"

int	dyn_str_init(t_dyn_str *ret)
{
	*ret = (t_dyn_str){0};
	return (0);
}

int	dyn_str_double(t_dyn_str *v)
{
	char	*temp;
	size_t	i;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64;
	temp = malloc(sizeof(int) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	dyn_str_push(t_dyn_str *v, char el)
{
	if (v->len + 1 >= v->cap || v->buff == 0)
		if (dyn_str_double(v))
			return (1);
	v->buff[v->len++] = el;
	v->buff[v->len] = 0;
	return (0);
}

char	dyn_str_pop(t_dyn_str *v)
{
	char	ret;

	ft_assert(v->len > 0);
	ret = v->buff[--v->len];
	v->buff[v->len] = 0;
	return (ret);
}

int	dyn_str_idx(t_dyn_str *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}

int		dyn_str_pushnbr(t_dyn_str *v, int nbr)
{
	long	n;

	n = nbr;
	if (n < 0)
	{
		if (dyn_str_push(v, '-'))
			return (1);
		n = -n;
	}
	if (n >= 10)
		if (dyn_str_pushnbr(v, n / 10))
			return (1);
	if (dyn_str_push(v, "0123456789"[n % 10]))
		return (1);
	return (0);
}
