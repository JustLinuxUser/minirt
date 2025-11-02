/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymath.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:54:08 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 19:18:37 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mymath.h"
#include <math.h>

t_fvec3	fvec3_cross(t_fvec3 v1, t_fvec3 v2)
{
	t_fvec3	result;

	result = (t_fvec3){
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
	return (result);
}

t_fvec2	vec2_rotate(t_fvec2 v, float angle)
{
	float	s;
	float	c;
	t_fvec2	ret;

	s = sin(angle);
	c = cos(angle);
	ret.x = v.x * c + v.y * s;
	ret.y = v.x * s - v.y * c;
	return (ret);
}

t_fvec3	vec3_rotate_yaw_pitch(t_fvec3 v, float yaw, float pitch)
{
	t_fvec2	zy;
	t_fvec2	xy;

	xy.x = v.x;
	xy.y = v.y;
	xy = vec2_rotate(xy, yaw);
	v.x = xy.x;
	v.y = xy.y;
	zy.x = v.z;
	zy.y = v.y;
	zy = vec2_rotate(zy, pitch);
	v.z = zy.x;
	v.y = zy.y;
	return (v);
}

float	fvec3_idx(t_fvec3 v, int idx)
{
	if (idx == 0)
		return (v.x);
	if (idx == 1)
		return (v.y);
	if (idx == 2)
		return (v.z);
	return (0);
}
