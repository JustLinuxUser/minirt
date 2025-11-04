/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:30:01 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 20:30:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: Change this to BVH_H
#pragma once

#include "minirt.h"
#include "libft/alloc/alloc.h"
#include "bounds.h"
#include <stddef.h>

typedef struct s_bvh_node
{
	size_t		shape_idx;
	t_bounds3f	bounds;
}	t_bvh_primitive;

typedef struct s_bvh_build_node
{
	t_bounds3f					bounds;
	struct s_bvh_build_node		*children[2];
	int							first_prim_offset;
	uint16_t					n_primitives;
	uint8_t						split_axis;
}	t_bvh_build_node;

typedef struct bvh_build_state
{
	t_state				state;
	t_vec_shape			ordered_shapes;
	int					total_nodes;
	t_bump_allocator	allocator;
}	t_bvh_build_state;

typedef struct bvh_prim_slice
{
	t_bvh_primitive	*buff;
	size_t			len;
}	t_bvh_prim_slice;

typedef struct s_sah_buckets
{
	t_bounds3f	bounds[SAH_BUCKETS];
	size_t		counts[SAH_BUCKETS];
	float		split_costs[SAH_BUCKETS - 1];
	int			split_counts[SAH_BUCKETS - 1];
}	t_sah_buckets;

typedef struct s_sah_splitfn_arg
{
	int			best_split;
	t_bounds3f	bounds;
	int			dim;
}	t_sah_splitfn_arg;

t_bvh_build_node	*bvh_build_recursive(t_bvh_build_state *state,
						t_bvh_prim_slice prims);
