/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:09:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:09:15 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "mymath.h"

static int	float_cmp(float a, float b)
{
	if (a > b)
		return (1);
	if (a < b)
		return (-1);
	return (0);
}

static int	centroid_cmp_dim(t_fvec3 a, t_fvec3 b, uint8_t dim)
{
	if (dim == 0)
		return (float_cmp(a.x, b.x));
	else if (dim == 1)
		return (float_cmp(a.y, b.y));
	return (float_cmp(a.z, b.z));
}

int	cmp_bvh_primitives(const void *av, const void *bv, void *arg)
{
	t_bvh_primitive	*a;
	t_bvh_primitive	*b;
	int				dim;
	t_fvec3			a_c;
	t_fvec3			b_c;

	a = (t_bvh_primitive *)av;
	b = (t_bvh_primitive *)bv;
	dim = *(int *)arg;
	a_c = bounds_centroid(a->bounds);
	b_c = bounds_centroid(b->bounds);
	return (centroid_cmp_dim(a_c, b_c, dim));
}
