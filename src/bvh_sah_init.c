/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_sah_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:21:49 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:21:49 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"

static void	sah_bucket_set_bounds_degenerate(t_sah_buckets *buckets)
{
	size_t			i;

	i = 0;
	while (i < SAH_BUCKETS)
		buckets->bounds[i++] = BOUNDS_DEGENERATE;
}

static t_bounds3f	get_centroid_bounds(t_bvh_prim_slice prims)
{
	t_bounds3f		centroid_bounds;
	size_t			i;

	centroid_bounds = BOUNDS_DEGENERATE;
	i = 0;
	while (i < prims.len)
		centroid_bounds = bounds_extend_pt(
				centroid_bounds,
				bounds_centroid(prims.buff[i++].bounds));
	return (centroid_bounds);
}

t_sah_buckets	init_centroid_buckets(t_bvh_prim_slice prims,
			t_bounds3f *centroid_bounds, int dim)
{
	t_sah_buckets	buckets;
	size_t			i;
	t_bounds3f		curr_bound;
	int				bucket;

	buckets = (t_sah_buckets){0};
	sah_bucket_set_bounds_degenerate(&buckets);
	*centroid_bounds = get_centroid_bounds(prims);
	i = 0;
	while (i < prims.len)
	{
		curr_bound = prims.buff[i].bounds;
		bucket = fvec3_idx(bounds_offset(*centroid_bounds,
					bounds_centroid(curr_bound)), dim) * SAH_BUCKETS;
		ft_assert(bucket >= 0);
		if (bucket == SAH_BUCKETS)
			bucket--;
		buckets.counts[bucket]++;
		buckets.bounds[bucket]
			= bounds_extend_bounds(buckets.bounds[bucket], curr_bound);
		i++;
	}
	return (buckets);
}
