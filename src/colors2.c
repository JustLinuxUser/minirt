/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 17:58:23 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/15 17:58:23 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	clamp_rgb(t_color c)
{
	t_color	out;

	out.x = fclamp(c.x, 0.0f, 1.0f);
	out.y = fclamp(c.y, 0.0f, 1.0f);
	out.z = fclamp(c.z, 0.0f, 1.0f);
	return (out);
}

float	blackbody(float lambda, float t)
{
	float	c;
	float	h;
	float	l ;
	float	le;

	if (t <= 0)
		return (0);
	c = 299792458.f;
	h = 6.62606957e-34f;
	l = lambda * 1e-9f;
	le = (2 * h * c * c) / (powf(l, 5) * (expf((h * c)
					/ (l * 1.3806488e-23f * t)) - 1));
	return (le);
}
