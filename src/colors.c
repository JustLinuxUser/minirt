/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:11:40 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 14:11:40 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include "cie.h"

float	linear_to_gamma(float c)
{
	if (c < 0)
		return (0);
	return (sqrt(c));
}

// TODO: try this formula
// float linear_to_srgb(float x) {
//     if (x <= 0.0031308f)
//         return 12.92f * x;
//     else
//         return 1.055f * powf(x, 1.0f/2.4f) - 0.055f;
// }

t_fvec3	spectrum_to_xyz(t_sampled_spec s, t_sampled_lambdas lambda)
{
	t_fvec3	ret;
	t_fvec3	tmp;
	float	pdf;
	float	value;
	int		i;

	ret = (t_fvec3){0};
	i = 0;
	while (i < NUM_SPECTRUM_SAMPLES)
	{
		pdf = lambda.pdf[i];
		value = s.values[i];
		if (pdf == 0.0f)
			i++;
		if (pdf == 0.0f)
			continue ;
		cie_xyz(lambda.lambda[i++], &tmp.x, &tmp.y, &tmp.z);
		ret.x += value * tmp.x / pdf;
		ret.y += value * tmp.y / pdf;
		ret.z += value * tmp.z / pdf;
	}
	ret.x /= NUM_SPECTRUM_SAMPLES * CIE_Y_INTEGRAL;
	ret.y /= NUM_SPECTRUM_SAMPLES * CIE_Y_INTEGRAL;
	ret.z /= NUM_SPECTRUM_SAMPLES * CIE_Y_INTEGRAL;
	return (ret);
}

t_8bcolor	xyz_to_rgb(t_fvec3 t)
{
	t_color		rgb;
	t_8bcolor	out;

	rgb.x = 3.2406f * t.x - 1.5372f * t.y - 0.4986f * t.z;
	rgb.y = -0.9689f * t.x + 1.8758f * t.y + 0.0415f * t.z;
	rgb.z = 0.0557f * t.x - 0.2040f * t.y + 1.0570f * t.z;
	rgb = clamp_rgb(rgb);
	out.r = fmin(linear_to_gamma(rgb.x) * 255.0f, 255.0f);
	out.g = fmin(linear_to_gamma(rgb.y) * 255.0f, 255.0f);
	out.b = fmin(linear_to_gamma(rgb.z) * 255.0f, 255.0f);
	return (out);
}

t_fvec3	rgb_to_xyz(t_8bcolor c)
{
	t_color	xyz;
	float	r;
	float	g;
	float	b;

	r = c.r / 255.f;
	g = c.g / 255.f;
	b = c.b / 255.f;
	if (r < 0.04045)
		r /= 12.92;
	else
		r = powf((r + 0.055) / 1.055, 2.4);
	if (g < 0.04045)
		g /= 12.92;
	else
		g = powf((g + 0.055) / 1.055, 2.4);
	if (b < 0.04045)
		b /= 12.92;
	else
		b = powf((b + 0.055) / 1.055, 2.4);
	xyz.x = 0.4124564 * r + 0.3575761 * g + 0.1804375 * b;
	xyz.y = 0.2126729 * r + 0.7151522 * g + 0.0721750 * b;
	xyz.z = 0.0193339 * r + 0.1191920 * g + 0.9503041 * b;
	return (xyz);
}

uint32_t	conv_8bcolor_to_uint32(t_8bcolor c)
{
	return ((uint32_t)c.r << 24 | (uint32_t)c.g << 16
		| (uint32_t)c.b << 8 | 0xff);
}
