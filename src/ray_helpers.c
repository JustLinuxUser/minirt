/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:21:07 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:32:38 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "cie.h"
#include "minirt.h"
#include "shapes.h"
#include "spectrum.h"
#include <math.h>
#include <stdlib.h>

t_collision	collide_ray_slow(t_state *state, t_ray_isector isect)
{
	t_collision	curr_coll;
	size_t		obj_idx;
	t_shape		shape;
	t_collision	coll;

	curr_coll = (t_collision){.collided = false};
	obj_idx = 0;
	while (obj_idx < state->shapes.len)
	{
		if (&state->shapes.buff[obj_idx].ptr == isect.ignore_shape)
			continue ;
		shape = state->shapes.buff[obj_idx];
		coll = collide_shape(state, shape, isect);
		if (coll.collided)
		{
			if (!curr_coll.collided || coll.t < curr_coll.t)
			{
				curr_coll = coll;
				if (isfinite(isect.t_max) && curr_coll.t < isect.t_max)
					break ;
			}
		}
		obj_idx++;
	}
	return (curr_coll);
}

t_sampled_spec	sample_densely_sampled_spectrum(
		const t_densely_sampled_spectrum *spec, t_sampled_lambdas lambdas)
{
	int				i;
	t_sampled_spec	ret;

	i = -1;
	ret = (t_sampled_spec){0};
	while (++i < NUM_SPECTRUM_SAMPLES)
		ret.values[i] = spec->samples[
			(int)(lambdas.lambda[i] - CIE_MIN_LAMBDA)];
	return (ret);
}

t_sampled_spec	get_surface_color(t_state *state,
		t_sampled_lambdas lambdas, t_collision coll)
{
	return (sample_densely_sampled_spectrum(
			shape_spectrum(state, coll), lambdas));
}

t_sampled_spec	sample_sky(t_state *state, t_sampled_lambdas lambdas)
{
	t_sampled_spec	spec;
	t_light			l;

	spec = (t_sampled_spec){0};
	if (state->sky_light_idx != -1)
	{
		l = state->lights.lights.buff[state->sky_light_idx];
		spec = sample_densely_sampled_spectrum(
				state->spectrums.buff + l.spec_idx, lambdas);
		spec = sampled_spectrum_scale(spec, l.intensity / 10);
	}
	return (spec);
}
