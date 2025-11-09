/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xorshiro128plusplus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 13:51:36 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/09 13:51:36 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XORSHIRO128PLUSPLUS_H
# define XORSHIRO128PLUSPLUS_H
# include <stdint.h>

typedef struct s_rand_state
{
	uint64_t	s[2];
}	t_rand_state;

uint64_t	xorshiro128plusplus(t_rand_state *s);

/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
void		xorshiro128plusplus_jump(t_rand_state *s);
#endif
