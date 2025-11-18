/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:23:05 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 09:23:05 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "libft/alloc/mmalloc.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "libft/ft_printf/ft_printf.h"
#include "minirt.h"
#include "shapes.h"
#include "libft/alloc/alloc.h"

void	flatten_bvh(t_linear_bvh_nd *linear,
			int *offset, t_bvh_build_node *tree)
{
	int	curr_offset;
	int	start_offset;

	start_offset = 0;
	if (!offset)
		offset = &start_offset;
	curr_offset = (*offset)++;
	linear[curr_offset].bounds = tree->bounds;
	linear[curr_offset].axis = tree->split_axis;
	linear[curr_offset].n_primitives = tree->n_primitives;
	if (tree->n_primitives)
		linear[curr_offset].u_.primitives_offset = tree->first_prim_offset;
	else
	{
		flatten_bvh(linear, offset, tree->children[0]);
		linear[curr_offset].u_.second_child_offset = *offset;
		flatten_bvh(linear, offset, tree->children[1]);
	}
}

t_bvh_primitive	*init_bvh_primitives(t_state *state)
{
	t_bvh_primitive	*bvh_primitives;
	size_t			i;

	bvh_primitives = mmalloc(sizeof(*bvh_primitives) * state->shapes.len,
			"Allocating a buffer for the bvh primitives");
	i = 0;
	while (i < state->shapes.len)
	{
		bvh_primitives[i] = (t_bvh_primitive){
			.bounds = shape_bounds(state, state->shapes.buff[i]),
			.shape_idx = i,
		};
		i++;
	}
	return (bvh_primitives);
}

void	build_bvh(t_state *state)
{
	t_bvh_primitive		*bvh_primitives;
	t_bvh_build_state	bstate;
	t_bvh_build_node	*tree;

	if (state->shapes.len == 0)
		return ;
	ft_eprintf("Building a BVH...\n");
	bvh_primitives = init_bvh_primitives(state);
	bstate = (t_bvh_build_state){.state = *state,
		.ordered_shapes = {0},
		.allocator = {.arena_size = 20000000}
	};
	vec_shape_init(&bstate.ordered_shapes, state->shapes.len);
	tree = bvh_build_recursive(&bstate,
			(t_bvh_prim_slice){bvh_primitives, state->shapes.len});
	free(bvh_primitives);
	state->bvh = mmalloc(sizeof(t_linear_bvh_nd) * bstate.total_nodes,
			"Allocating a buffer for the linear bvh");
	flatten_bvh(state->bvh, 0, tree);
	bump_alloc_free(&bstate.allocator);
	free(state->shapes.buff);
	state->shapes = bstate.ordered_shapes;
}
