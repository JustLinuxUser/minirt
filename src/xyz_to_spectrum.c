/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xyz_to_spectrum.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:17:42 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/13 18:17:42 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cie.h"
#include "light_structs.h"
#include "minirt.h"
#include "mymath.h"
#include <math.h>
#include "libft/libft.h"

inline static float	color_error(t_fvec3 c0, t_fvec3 c1)
{
	return (fvec3_len_sq(fvec3_sub(c0, c1)));
}

void	add_contrib(t_densely_sampled_spectrum *spec, float diff,
					float contrib, float *cie)
{
	float	factor;
	int		i;

	factor = diff / contrib / 10;
	i = 0;
	while (i < CIE_SAMPLES)
	{
		spec->samples[i] += cie[i] * factor;
		i++;
	}
}

void	clamp_spectre(t_densely_sampled_spectrum *spec)
{
	float	max;
	int		i;

	max = 0;
	i = 0;
	while (i < CIE_SAMPLES)
	{
		spec->samples[i] = fmax(spec->samples[i], 0);
		max = fmax(spec->samples[i], max);
		i++;
	}
	max = fmax(max, 1);
	i = 0;
	while (i < CIE_SAMPLES)
		spec->samples[i++] /= max;
}

void	reduce_err_iter(t_densely_sampled_spectrum *spec,
			t_fvec3 target, t_fvec3 contrib)
{
	t_fvec3	curr;
	int		i;

	i = 0;
	while (i < 100)
	{
		curr = densely_sampled_spectrum_to_xyz(spec);
		if (i % 3 == 0)
			add_contrib(spec, target.x - curr.x, contrib.x, cie_x());
		else if (i % 3 == 1)
			add_contrib(spec, target.y - curr.y, contrib.y, cie_y());
		else
			add_contrib(spec, target.z - curr.z, contrib.z, cie_z());
		i++;
	}
	i = 0;
	while (i < CIE_SAMPLES)
	{
		spec->samples[i] = fmax(spec->samples[i], 0);
		i++;
	}
}

t_densely_sampled_spectrum	xyz_to_spectrum(t_fvec3 target,
		bool clamp, float *err)
{
	t_densely_sampled_spectrum	spec;
	t_fvec3						contrib;
	int							i;
	t_fvec3						curr_xyz;

	ft_memcpy(spec.samples, cie_x(), sizeof(spec.samples));
	contrib.x = densely_sampled_spectrum_to_xyz(&spec).x;
	ft_memcpy(spec.samples, cie_y(), sizeof(spec.samples));
	contrib.y = densely_sampled_spectrum_to_xyz(&spec).y;
	ft_memcpy(spec.samples, cie_z(), sizeof(spec.samples));
	contrib.z = densely_sampled_spectrum_to_xyz(&spec).z;
	spec = (t_densely_sampled_spectrum){0};
	i = 0;
	while (i < 10)
	{
		curr_xyz = densely_sampled_spectrum_to_xyz(&spec);
		reduce_err_iter(&spec, target, contrib);
		if (err)
			*err = color_error(target, curr_xyz);
		i++;
	}
	if (clamp)
		clamp_spectre(&spec);
	return (spec);
}
