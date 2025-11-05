/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build_sah.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:19:31 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:19:31 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "libft/dsa/partition.h"

void	sah_bucket_splits(t_sah_buckets *buckets)
{
	t_bounds3f	bound_below;
	int			bl;
	t_bounds3f	bound_above;
	int			ab;
	size_t		i;

	bound_below = BOUNDS_DEGENERATE;
	bl = 0;
	i = 0;
	while (i < SAH_BUCKETS - 1)
	{
		bound_below = bounds_extend_bounds(bound_below, buckets->bounds[i]);
		bl += buckets->counts[i];
		buckets->split_counts[i] += bl;
		buckets->split_costs[i++] += bounds_surphace_area(bound_below) * bl;
	}
	bound_above = BOUNDS_DEGENERATE;
	ab = 0;
	i = SAH_BUCKETS - 1;
	while (i > 0)
	{
		bound_above = bounds_extend_bounds(bound_above, buckets->bounds[i]);
		ab += buckets->counts[i];
		buckets->split_costs[i-- - 1] += bounds_surphace_area(bound_above) * ab;
	}
}

void	sah_bucket_min_cost(t_sah_buckets *buckets, float *min_cost,
		int *min_split_idx, t_bvh_build_node *ret_node)
{
	int	i;

	*min_cost = INFINITY;
	*min_split_idx = 0;
	i = 0;
	while (i < SAH_BUCKETS - 1)
	{
		if (buckets->split_costs[i] < *min_cost)
		{
			*min_cost = buckets->split_costs[i];
			*min_split_idx = i;
		}
		i++;
	}
	*min_cost = 1.f / 2.f + *min_cost / bounds_surphace_area(ret_node->bounds);
}

bool	split_fn(void *ptr, void *arg_v)
{
	t_bvh_primitive		*prim;
	t_sah_splitfn_arg	*arg;
	int					b;

	prim = ptr;
	arg = arg_v;
	b = (fvec3_idx(bounds_offset(arg->bounds, bounds_centroid(prim->bounds)),
				arg->dim) * SAH_BUCKETS);
	return (arg->best_split >= b);
}

void	partition_bvh_sah(t_bvh_build_state *state, t_bvh_prim_slice prims,
			t_bvh_build_node *ret, t_sah_splitfn_arg splitfn_args)
{
	t_partition_args	pargs;
	t_bvh_primitive		*p2;
	size_t				mid;

	pargs = (t_partition_args){.arg = &splitfn_args, .data = prims.buff,
		.len = prims.len, .el_size = sizeof(*prims.buff), .sep = split_fn};
	p2 = partition(pargs);
	mid = p2 - prims.buff;
	if (mid == 0 || mid == prims.len)
		bvh_create_leaf_node(state, prims, ret);
	else
	{
		ret->children[0] = bvh_build_recursive(state,
				(t_bvh_prim_slice){prims.buff, mid});
		ret->children[1] = bvh_build_recursive(state,
				(t_bvh_prim_slice){p2, prims.len - mid});
		ret->n_primitives = 0;
		ret->split_axis = splitfn_args.dim;
	}
}

void	bvh_sah(t_bvh_build_state *state,
							t_bvh_prim_slice prims, t_bvh_build_node *ret)
{
	int				dim;
	t_bounds3f		centroid_bounds;
	t_sah_buckets	buckets;
	float			min_cost;
	int				min_split_idx;

	state->total_nodes++;
	dim = bounds_max_dim(ret->bounds);
	buckets = init_centroid_buckets(prims, &centroid_bounds, dim);
	sah_bucket_splits(&buckets);
	sah_bucket_min_cost(&buckets, &min_cost, &min_split_idx, ret);
	if (prims.len < min_cost || prims.len < BVH_SAH_MIN_NUM_NODES)
		bvh_create_leaf_node(state, prims, ret);
	else
		partition_bvh_sah(state, prims, ret, (t_sah_splitfn_arg)
		{.bounds = centroid_bounds, .dim = dim, .best_split = min_split_idx});
}
