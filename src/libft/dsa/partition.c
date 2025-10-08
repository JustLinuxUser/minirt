/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partition.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:41:39 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/10 15:41:39 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../libft.h"
#include "partition.h"

void	*partition(t_partition_args args)
{
	uint8_t	*first;
	uint8_t	*last;

	first = args.data;
	last = first + (args.len - 1) * args.el_size;
	ft_assert(args.el_size != 0);
	if (!args.data || args.len < 2)
		return (first);
	while (first != last && last > first)
	{
		while (args.sep(first, args.arg) == true && last > first)
		{
			first += args.el_size;
		}
		while (args.sep(last, args.arg) == false && last > first)
		{
			last -= args.el_size;
		}
		if (first < last)
			ft_memswap(first, last, args.el_size);
	}
	return (first);
}
