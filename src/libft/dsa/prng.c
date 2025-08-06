/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prng.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:29:32 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/24 12:04:19 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../libft.h"

void	prng_initialize_state(t_prng_state *state, uint32_t seed)
{
	uint32_t	*state_array;
	int			i;

	state_array = &(state->state_array[0]);
	state_array[0] = seed;
	i = 1;
	while (i < 624)
	{
		seed = 1812433253UL * (seed ^ (seed >> (32 - 2))) + i;
		state_array[i] = seed;
		i++;
	}
	state->state_index = 0;
}

static uint32_t	random_uint32_inner(t_prng_state *state, int k, int j, int x)
{
	uint32_t	*state_array;
	uint32_t	xa;
	uint32_t	y;
	uint32_t	z;

	state_array = &(state->state_array[0]);
	xa = x >> 1;
	if (x & 0x00000001UL)
		xa ^= 0x9908b0dfUL;
	j = k - (624 - 397);
	if (j < 0)
		j += 624;
	x = state_array[j] ^ xa;
	state_array[k++] = x;
	if (k >= 624)
		k = 0;
	state->state_index = k;
	y = x ^ (x >> 11);
	y = y ^ ((y << 7) & 0x9d2c5680UL);
	y = y ^ ((y << 15) & 0xefc60000UL);
	z = y ^ (y >> 18);
	return (z);
}

//https://en.wikipedia.org/wiki/Mersenne_Twister
uint32_t	random_uint32(t_prng_state *state)
{
	uint32_t	*state_array;
	int			k;
	int			j;
	uint32_t	x;

	state_array = &(state->state_array[0]);
	k = state->state_index;
	j = k - (624 - 1);
	if (j < 0)
		j += 624;
	x = (state_array[k] & (0xffffffffUL << 31))
		| (state_array[j] & (0xffffffffUL >> (32 - 31)));
	return (random_uint32_inner(state, k, j, x));
}
