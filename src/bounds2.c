/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 10:18:42 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 10:18:42 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounds.h"

uint8_t	bounds_max_dim(t_bounds3f bounds)
{
	t_fvec3	diag;

	diag = fvec3_sub(bounds.max, bounds.min);
	if (diag.x >= diag.y && diag.x >= diag.z)
		return (0);
	if (diag.y >= diag.x && diag.y >= diag.z)
		return (1);
	if (diag.z >= diag.x && diag.z >= diag.y)
		return (2);
	return (2);
}

t_fvec3	bounds_offset(t_bounds3f bounds, t_fvec3 p)
{
	return ((t_fvec3){
		.x = fmax((p.x - bounds.min.x) / (bounds.max.x - bounds.min.x), 0),
		.y = fmax((p.y - bounds.min.y) / (bounds.max.y - bounds.min.y), 0),
		.z = fmax((p.z - bounds.min.z) / (bounds.max.z - bounds.min.z), 0),
	});
}
