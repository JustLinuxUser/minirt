/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:42:01 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/10 15:42:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_memswap(void *a, void *b, size_t size)
{
	uint8_t	temp[256];
	size_t	total_swapped;
	size_t	to_move;

	total_swapped = 0;
	while (total_swapped < size)
	{
		to_move = size - total_swapped;
		if (sizeof(temp) < to_move)
			to_move = sizeof(temp);
		ft_memcpy(temp, (uint8_t *)a + total_swapped, to_move);
		ft_memcpy((uint8_t *)a + total_swapped,
			(uint8_t *)b + total_swapped, to_move);
		ft_memcpy((uint8_t *)b + total_swapped, temp, to_move);
		total_swapped += to_move;
	}
}
