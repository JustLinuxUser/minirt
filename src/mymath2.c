/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_vec3_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:16:57 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 19:17:25 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mymath.h"

t_fvec3	fvec3_add(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
	});
}

t_fvec3	fvec3_mul(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3){
		.x = a.x * b.x,
		.y = a.y * b.y,
		.z = a.z * b.z,
	});
}

t_fvec3	fvec3_scale(t_fvec3 v, float scalar)
{
	return ((t_fvec3){
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar,
	});
}

t_fvec3	fvec3_reflect(t_fvec3 v, t_fvec3 norm)
{
	return (fvec3_sub(v,
			fvec3_scale(norm,
				2 * fvec3_dot(norm, v))));
}

t_fvec3	fvec3_lerp(t_fvec3 v1, t_fvec3 v2, float prop)
{
	return ((t_fvec3){
		.x = v1.x * prop + v2.x * (1 - prop),
		.y = v1.y * prop + v2.y * (1 - prop),
		.z = v1.z * prop + v2.z * (1 - prop),
	});
}
