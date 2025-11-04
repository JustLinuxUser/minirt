/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_vec3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:15:22 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 19:15:56 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mymath.h"
#include <math.h>

t_fvec3	fvec3_sub(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3){
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z,
	});
}

float	fvec3_dot(t_fvec3 a, t_fvec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_fvec3	fvec3_norm(t_fvec3 v)
{
	t_fvec3	result;
	float	length;
	float	ilength;

	result = v;
	length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length != 0.0f)
	{
		ilength = 1.0f / length;
		result.x *= ilength;
		result.y *= ilength;
		result.z *= ilength;
	}
	else
		result.x = 1;
	return (result);
}

float	fvec3_len_sq(t_fvec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

t_fvec3	fvec3_invert(t_fvec3 v)
{
	return ((t_fvec3){.x = -v.x, .y = -v.y, .z = -v.z});
}
