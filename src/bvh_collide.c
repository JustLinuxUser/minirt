/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_collide.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:18:09 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 10:18:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "ray.h"
#include <stdbool.h>

void	negative_dirs(t_ray_isector isect, bool dir_is_neg[3])
{
	t_fvec3		inv_dir;

	inv_dir = (t_fvec3){.x = 1 / isect.ray.dir.x,
		.y = 1 / isect.ray.dir.y, .z = 1 / isect.ray.dir.z};
	dir_is_neg[0] = inv_dir.x < 0;
	dir_is_neg[1] = inv_dir.y < 0;
	dir_is_neg[2] = inv_dir.z < 0;
}

inline static void	collide_bvh_primitives(t_state *state,
		t_bvh_isector *b_isector, t_linear_bvh_nd curr)
{
	int			i;
	t_collision	coll;
	int			prim_offs;

	i = 0;
	while (i < curr.n_primitives)
	{
		prim_offs = curr.u_.primitives_offset;
		coll = collide_shape(
				state, state->shapes.buff[prim_offs + i], b_isector->isect);
		if (coll.collided && (coll.t < b_isector->curr_best.t
				|| !b_isector->curr_best.collided))
			b_isector->curr_best = coll;
		i++;
	}
}

inline static void	collide_bvh_bounds(t_state *state,
		t_bvh_isector *b_isector)
{
	float			hit0;
	size_t			curr_nd_idx;
	t_linear_bvh_nd	curr;

	curr_nd_idx = b_isector->stack[b_isector->stack_offs--];
	curr = state->bvh[curr_nd_idx];
	if (!intersect_bounds(curr.bounds, b_isector->isect.ray,
			b_isector->isect.t_max, &hit0))
		return ;
	if (curr.n_primitives > 0)
		collide_bvh_primitives(state, b_isector, curr);
	else
	{
		if (b_isector->dir_is_neg[curr.axis])
		{
			b_isector->stack[++b_isector->stack_offs] = curr_nd_idx + 1;
			b_isector->stack[++b_isector->stack_offs]
				= curr.u_.second_child_offset;
			return ;
		}
		b_isector->stack[++b_isector->stack_offs]
			= curr.u_.second_child_offset;
		b_isector->stack[++b_isector->stack_offs] = curr_nd_idx + 1;
	}
}

t_collision	collide_bvh(t_state *state, t_ray_isector isect)
{
	t_bvh_isector	b_isector;
	size_t			i;
	t_collision		coll;

	b_isector = (t_bvh_isector){
		.isect = isect,
	};
	negative_dirs(isect, b_isector.dir_is_neg);
	while (state->bvh && b_isector.stack_offs >= 0)
		collide_bvh_bounds(state, &b_isector);
	i = 0;
	while (i < state->unbounded_shapes.len)
	{
		coll = collide_shape(
				state, state->unbounded_shapes.buff[i], isect);
		if (coll.collided
			&& (coll.t < b_isector.curr_best.t
				|| !b_isector.curr_best.collided))
			b_isector.curr_best = coll;
		i++;
	}
	return (b_isector.curr_best);
}
