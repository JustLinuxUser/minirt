/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:18:28 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:18:28 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_str.h"

int	dyn_str_pushnbr(t_dyn_str *v, int nbr)
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
