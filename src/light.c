/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:34:57 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:41:30 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounds.h"
#include "cie.h"
#include "light_structs.h"
#include "minirt.h"
#include "spectrum.h"

//Calculate information for lights -> DenselySampledSpectrum
t_densely_sampled_spectrum	densely_sampled_spectrum_blackbody(float T)
{
	t_densely_sampled_spectrum	ret;
	int							i;
	float						bb_const;

	i = CIE_MIN_LAMBDA;
	bb_const = blackbody(2.89e-3f / T * 1e9f, T);
	while (i <= (int)CIE_MAX_LAMBDA)
	{
		ret.samples[i - (int)CIE_MIN_LAMBDA] = blackbody(i, T)
			/ bb_const;
		i++;
	}
	return (ret);
}

void	add_light(t_lights *lights, t_light to_add)
{
	if (!lights)
		return ;
	vec_light_push(&lights->lights, to_add);
}

t_bounding_sphere	scene_bounding_sphere(t_state *state)
{
	t_bounding_sphere	ret;
	t_bounds3f			bounds;

	bounds = bounds_degenerate();
	if (state->bvh)
		bounds = state->bvh->bounds;
	ret.p = bounds_centroid(bounds);
	ret.r = sqrtf(fvec3_len_sq(fvec3_sub(bounds.min, ret.p)));
	return (ret);
}

void	calculate_pdfs(t_state *state)
{
	size_t	i;

	free(state->lights.pdfs.buff);
	state->lights.bounding_sphere = scene_bounding_sphere(state);
	vec_float_init(&state->lights.pdfs, state->lights.lights.len);
	i = 0;
	while (i < state->lights.lights.len)
		state->lights.total_power
			+= light_power(state, state->lights.lights.buff[i++]);
	i = 0;
	while (i < state->lights.lights.len)
	{
		state->lights.pdfs.buff[i]
			= light_power(state, state->lights.lights.buff[i])
			/ state->lights.total_power;
		i++;
	}
}

float	light_power(t_state *state, t_light l)
{
	float				power;

	power = 0;
	if (l.t == POINT_LIGHT)
		power = 4 * PI;
	else if (l.t == DISTANT_LIGHT)
		power = PI * state->lights.bounding_sphere.r
			* state->lights.bounding_sphere.r;
	else if (l.t == AMBIANT_LIGHT)
		power = 4 * PI * state->lights.bounding_sphere.r
			* state->lights.bounding_sphere.r;
	else
		ft_assert("Unreachable" == 0);
	power *= l.intensity * densely_sapmled_spectrum_avg_power(
			state->spectrums.buff + l.spec_idx);
	return (power);
}
