/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:22:38 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:22:38 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOUNDS_H
# define BOUNDS_H
# include "mymath.h"
# include <stdbool.h>
# include <stdint.h>
# include "math.h"

typedef struct s_bounds3f
{
	t_fvec3	min;
	t_fvec3	max;
}	t_bounds3f;

inline static t_bounds3f	bounds_degenerate(void)
{
	return ((t_bounds3f){{NAN, NAN, NAN}, {NAN, NAN, NAN}});
}

t_bounds3f	bounds_extend_pt(t_bounds3f bounds, t_fvec3 pt);
t_bounds3f	bounds_extend_bounds(t_bounds3f b1, t_bounds3f b2);
t_fvec3		bounds_centroid(t_bounds3f bounds);
float		bounds_surphace_area(t_bounds3f bounds);
uint8_t		bounds_max_dim(t_bounds3f bounds);
t_fvec3		bounds_offset(t_bounds3f bounds, t_fvec3 p);
bool		bounds_check_enclosed(t_bounds3f big, t_bounds3f small);
bool		intersect_bounds(t_bounds3f bounds,
								t_ray r,
								float t_max,
								float *hitt0);
#endif
