/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build_recursive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:21:18 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:21:18 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "libft/dsa/smoothsort.h"
#include "libft/ft_printf/ft_printf.h"
#include "libft/libft.h"

void	bvh_create_leaf_node(t_bvh_build_state *state,
						t_bvh_prim_slice prims,
						t_bvh_build_node *ret)
{
	size_t		i;
	t_shape		shape;
	t_bounds3f	bounds;

	ret->first_prim_offset = state->ordered_shapes.len;
	i = 0;
	while (i < prims.len)
	{
		shape = state->state.shapes.buff[prims.buff[i].shape_idx];
		bounds = shape_bounds(&state->state, shape);
		if (!bounds_check_enclosed(ret->bounds, bounds))
			ft_printf("Failing in bvh leaf node\n");
		if (ft_memcmp(&bounds, &prims.buff[i].bounds, sizeof(bounds)))
		{
			ft_printf("Got different bounds\n");
		}
		i++;
		vec_shape_push(&state->ordered_shapes, shape);
	}
	ret->n_primitives = prims.len;
	state->total_nodes++;
}

void	bvh_midpoint(t_bvh_build_state *state,
							t_bvh_prim_slice slice, t_bvh_build_node *ret)
{
	int	mid;
	int	dim;

	mid = slice.len / 2;
	dim = bounds_max_dim(ret->bounds);
	my_smoothsort((t_sort_args){.arr = slice.buff,
		.arg = &dim,
		.arr_len = slice.len,
		.el_size = sizeof(*slice.buff),
		.cmp = cmp_bvh_primitives});
	ret->children[0] = bvh_build_recursive(state,
			(t_bvh_prim_slice){slice.buff, mid});
	ret->children[1] = bvh_build_recursive(state,
			(t_bvh_prim_slice){slice.buff + mid, slice.len - mid});
	ret->n_primitives = 0;
	ret->split_axis = dim;
	state->total_nodes++;
}

t_bvh_build_node	*bvh_build_recursive(t_bvh_build_state *state,
						t_bvh_prim_slice slice)
{
	size_t				i;
	t_bvh_build_node	*ret;

	assert(slice.len > 0);
	ret = bump_alloc(&state->allocator, sizeof(*ret));
	*ret = (t_bvh_build_node){.bounds = BOUNDS_DEGENERATE};
	i = 0;
	while (i < slice.len)
		ret->bounds = bounds_extend_bounds(ret->bounds, slice.buff[i++].bounds);
	if (bounds_surphace_area(ret->bounds) == 0 || slice.len == 1)
	{
		bvh_create_leaf_node(state, slice, ret);
		return (ret);
	}
	if (USE_SAH)
		bvh_sah(state, slice, ret);
	else
		bvh_midpoint(state, slice, ret);
	return (ret);
}
