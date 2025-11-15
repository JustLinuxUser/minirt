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
#include "ray_structs.h"
#include "minirt.h"
#include "samplers.h"
#include "shapes.h"
#include "spectrum.h"
#include <math.h>
#include "ray_helpers.h"

bool	dir_intencity(t_light_color_sampler *s, t_rand_state *rand_state,
			t_fvec3 norm, t_ray_isector *isector)
{
	if (s->light.t == POINT_LIGHT)
	{
		s->light_dir = fvec3_sub(s->light.position, isector->ray.pos);
		s->light_t = sqrtf(fvec3_len_sq(s->light_dir));
		s->light.intensity /= s->light_t * s->light_t;
	}
	else if (s->light.t == DISTANT_LIGHT)
	{
		s->light.intensity /= 10.0;
		s->light_dir = s->light.position;
	}
	else if (s->light.t == AMBIANT_LIGHT)
	{
		s->light.intensity /= 10.0;
		s->light.intensity /= 2.0;
		s->light_dir = rand_halfsphere(norm, rand_state);
	}
	else
	{
		ft_assert("Unreachable" == 0);
		return (false);
	}
	return (true);
}

t_light	sample_effective_light(t_state *state, t_ray_isector *isector,
		t_rand_state *rand_state, t_fvec3 norm)
{
	t_light_color_sampler	s;

	s.light = (t_light){0};
	s.light_idx = sample_alias_table(&state->lights, rand_float(rand_state));
	if (s.light_idx == -1)
		return (s.light);
	s.light = state->lights.lights.buff[s.light_idx];
	s.light_t = INFINITY;
	s.light.intensity = s.light.intensity
		/ state->lights.pdfs.buff[s.light_idx];
	if (!dir_intencity(&s, rand_state, norm, isector))
		return ((t_light){0});
	isector->ray.dir = fvec3_norm(s.light_dir);
	isector->t_max = s.light_t;
	s.light_coll = collide_bvh(state, *isector);
	if (s.light_coll.collided)
		s.light.t = LIGHT_NONE;
	return (s.light);
}

bool	process_collision(t_state *state,
			t_integrator *i, t_sampled_lambdas lambdas)
{
	t_sampled_spec	sky_spec;

	i->coll = collide_bvh(state, (t_ray_isector){.ray = i->ray, .t_max
			= INFINITY, .t_min = 0.01, .ignore_shape = i->ignored_shape});
	if (!i->coll.collided)
	{
		if (i->iter == 0)
		{
			sky_spec = sample_sky(state, lambdas);
			i->i = -1;
			while (++i->i < NUM_SPECTRUM_SAMPLES)
				i->l.values[i->i]
					+= i->beta.values[i->i] * sky_spec.values[i->i];
		}
		return (false);
	}
	i->ignored_shape = i->coll.shape.ptr;
	i->p = fvec3_add(fvec3_scale(i->ray.dir, i->coll.t), i->ray.pos);
	i->norm = collision_norm(state, i->coll, i->p);
	if (fvec3_dot(i->norm, i->ray.dir) > 0)
		i->norm = fvec3_invert(i->norm);
	return (true);
}

t_sampled_spec	add_color(t_state *state, t_sampled_lambdas lambdas,
					t_integrator *i, t_rand_state *rand_state)
{
	t_sampled_spec	light_spec;
	t_sampled_spec	color;
	t_ray_isector	isector;
	t_light			light;
	t_sampled_spec	ambiant_spec;

	light_spec = (t_sampled_spec){0};
	color = get_surface_color(state, lambdas, i->coll);
	isector = (t_ray_isector){.ray = {.pos = i->p}, .t_min = 0.01,
		.ignore_shape = i->ignored_shape};
	light = sample_effective_light(state, &isector, rand_state, i->norm);
	if (light.t != LIGHT_NONE)
	{
		light_spec = sampled_spectrum_scale(sample_densely_sampled_spectrum(
					&state->spectrums.buff[light.spec_idx], lambdas),
				light.intensity * fmax(fvec3_dot(i->norm, isector.ray.dir), 0));
	}
	ambiant_spec = sample_densely_sampled_spectrum(
			&state->ambiant_light_spec, lambdas);
	i->i = -1;
	while (++i->i < NUM_SPECTRUM_SAMPLES)
		i->l.values[i->i] += i->beta.values[i->i] * color.values[i->i]
			* (light_spec.values[i->i] + ambiant_spec.values[i->i]
				* fabs(fvec3_dot(i->norm, i->ray.dir)));
	return (color);
}

t_sampled_spec	cast_reflectable_ray_new(t_state *state, t_ray start_ray,
			t_sampled_lambdas lambdas, t_rand_state *rand_state)
{
	t_integrator	i;
	t_sampled_spec	color;

	i = (t_integrator){0};
	i.iter = -1;
	i.ray = start_ray;
	i.i = -1;
	while (++i.i < NUM_SPECTRUM_SAMPLES)
		i.beta.values[i.i] = 1.f;
	while (++i.iter < state->rndr.max_reflections)
	{
		if (!process_collision(state, &i, lambdas))
			break ;
		color = add_color(state, lambdas, &i, rand_state);
		i.ray = (t_ray){.pos = i.p, .dir = rand_halfsphere(i.norm, rand_state)};
		i.i = -1;
		while (++i.i < NUM_SPECTRUM_SAMPLES)
		{
			i.beta.values[i.i] *= color.values[i.i]
				* fmax(fvec3_dot(i.norm, i.ray.dir), 0);
		}
	}
	return (i.l);
}
