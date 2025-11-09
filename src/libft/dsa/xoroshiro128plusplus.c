/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xoroshiro128plusplus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 13:39:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/09 13:39:41 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */
/* This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

#include <stddef.h>
#include <stdint.h>
#include "xoroshiro128plusplus.h"

static inline uint64_t	rotl(const uint64_t x, int k)
{
	return ((x << k) | (x >> (64 - k)));
}

uint64_t	xoroshiro128plusplus(t_rand_state *s)
{
	const uint64_t	s0 = s->s[0];
	const uint64_t	result = rotl(s0 + s->s[1], 17) + s0;
	uint64_t		s1;

	s1 = s->s[1];
	s1 ^= s0;
	s->s[0] = rotl(s0, 49) ^ s1 ^ (s1 << 21);
	s->s[1] = rotl(s1, 28);
	return (result);
}

/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
void	xoroshiro128plusplus_jump(t_rand_state *s)
{
	uint64_t				s0;
	uint64_t				s1;
	static const uint64_t	jump[] = {0x2bd7a6a6e99c2ddc, 0x0992ccaf6a6fca05};
	int						i;
	int						b;

	s0 = 0;
	s1 = 0;
	i = -1;
	while ((size_t)++i < sizeof jump / sizeof (*jump))
	{
		b = -1;
		while (++b < 64)
		{
			if (jump[i] & UINT64_C(1) << b)
			{
				s0 ^= s->s[0];
				s1 ^= s->s[1];
			}
			xoroshiro128plusplus(s);
		}
	}
	s->s[0] = s0;
	s->s[1] = s1;
}

/* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */
void	xoroshiro128plusplus_long_jump(t_rand_state *s)
{
	static const uint64_t	long_jump[2]
		= {0x360fd5f2cf8d5d99, 0x9c6e6877736c46e3};
	uint64_t				s0;
	uint64_t				s1;
	int						i;
	int						b;

	s0 = 0;
	s1 = 0;
	i = -1;
	while ((size_t)++i < sizeof (long_jump) / sizeof (*long_jump))
	{
		b = -1;
		while (++b < 64)
		{
			if (long_jump[i] & UINT64_C(1) << b)
			{
				s0 ^= s->s[0];
				s1 ^= s->s[1];
			}
			xoroshiro128plusplus(s);
		}
	}
	s->s[0] = s0;
	s->s[1] = s1;
}
