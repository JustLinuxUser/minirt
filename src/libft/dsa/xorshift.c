/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xorshift.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:51:08 by anddokhn          #+#    #+#             */
/*   Updated: 2025/10/31 16:51:08 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// uint64_t	xorshift64(uint64_t *state)
// {
// 	uint64_t	x = *state;
//
// 	x ^= x >> 12;
//     x ^= x << 25;
//     x ^= x >> 27;
// 	x = x * 0x2545F4914F6CDD1DULL;
//     return *state = x;
// }

uint64_t	xorshift64(uint64_t *state)
{
	uint64_t	x;

	x = *state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return (*state = x);
}
