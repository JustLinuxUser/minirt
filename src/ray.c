/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:58:51 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:33:48 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "cie.h"
#include "minirt.h"
#include "samplers.h"
#include "shapes.h"
#include "spectrum.h"
#include <math.h>
#include <stdlib.h>
#include "ray_helpers.h"

t_light	sample_effective_light(t_state *state, t_ray_isector *isector,
		uint64_t *rand_state, t_fvec3 norm)
{
	t_light		light;
	float		light_t;
	t_collision	light_coll;
	int			light_idx;
	t_fvec3		light_dir;

	light = (t_light){0};
	light_idx = sample_alias_table(&state->lights, rand_float(rand_state));
	if (light_idx == -1)
		return (light);
	light = state->lights.lights.buff[light_idx];
	light_t = INFINITY;
	light.intensity = light.intensity / state->lights.pdfs.buff[light_idx];
	if (light.t == POINT_LIGHT)
	{
		light_dir = fvec3_sub(light.position, isector->ray.pos);
		light_t = sqrtf(fvec3_len_sq(light_dir));
		light.intensity /= light_t * light_t;
	}
	else if (light.t == DISTANT_LIGHT)
	{
		light.intensity /= 10.0;
		light_dir = light.position;
	}
	else if (light.t == AMBIANT_LIGHT)
	{
		light.intensity /= 10.0;
		light.intensity /= 2.0;
		light_dir = rand_halfsphere(norm, rand_state);
	}
	else
	{
		ft_assert("Unreachable" == 0);
		return (light.t = LIGHT_NONE, light);
	}
	isector->ray.dir = fvec3_norm(light_dir);
	isector->t_max = light_t;
	light_coll = collide_bvh(state, *isector);
	if (light_coll.collided)
		light.t = LIGHT_NONE;
	return (light);
}

t_sampled_spec cast_reflectable_ray_new(t_state *state, t_ray ray,
			t_sampled_lambdas lambdas, int iters_left, uint64_t *rand_state)
{
	t_sampled_spec	L = {0};
	t_sampled_spec	beta = {1.f};
	t_collision			coll;
	int					i;
	int					iter = -1;
	t_fvec3				p;
	t_fvec3				norm = {0};
	void				*ignored_shape = 0;

	i = -1;
	while (++i < NUM_SPECTRUM_SAMPLES)
		beta.values[i] = 1.f;
	while (iters_left-- > 0)
	{
		iter++;
		coll = collide_bvh(state, (t_ray_isector){.ray = ray, .t_max = INFINITY,
				.t_min = 0.01, .ignore_shape = ignored_shape});
		if (!coll.collided)
		{
			if (iter == 0)
			{
				t_sampled_spec sky_spec = sample_sky(state, lambdas);
				i = -1;
				while (++i < NUM_SPECTRUM_SAMPLES)
					L.values[i] += beta.values[i] * sky_spec.values[i];
			}
			break ;
		}
		ignored_shape = coll.shape.ptr;
		p = fvec3_add(fvec3_scale(ray.dir, coll.t), ray.pos);
		norm = collision_norm(state, coll, p);
		if (fvec3_dot(norm, ray.dir) > 0)
			norm = fvec3_invert(norm);
        float lu = rand_float(rand_state);
		t_sampled_spec color = {0};
		color = get_surface_color(state, lambdas, coll);
		t_sampled_spec light_spec = {0};
		t_ray_isector isector = {.ray = {.pos = p}, .t_min = 0.01, .ignore_shape = ignored_shape};
		t_light light = sample_effective_light(state, &isector, rand_state, norm);
		if (light.t != LIGHT_NONE) {
			float dot = fmax(fvec3_dot(norm, isector.ray.dir), 0);
			light_spec = sampled_spectrum_scale(
					sample_densely_sampled_spectrum(
						&state->spectrums.buff[light.spec_idx],
						lambdas), light.intensity * dot);
		}
		t_sampled_spec ambiant_spec = sample_densely_sampled_spectrum(
				&state->ambiant_light_spec, lambdas);
		i = -1;
		while (++i < NUM_SPECTRUM_SAMPLES)
			L.values[i] += beta.values[i] * color.values[i]
				* (light_spec.values[i] + ambiant_spec.values[i]
					* fabs(fvec3_dot(norm, ray.dir)));
		t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm, rand_state)};
		ray = new_ray;
		float dot = fmax(fvec3_dot(norm, new_ray.dir), 0);
		i = -1;
		while (++i < NUM_SPECTRUM_SAMPLES)
		{
			if (lambdas.pdf[i] == 0.f)
				beta.values[i] = 0.f;
			else
				beta.values[i] *= color.values[i] * dot;
		}
	}
	return (L);
}
