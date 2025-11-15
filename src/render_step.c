/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_step.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:44:21 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/13 18:44:21 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cie.h"
#include "draw.h"
#include "minirt.h"
#include "samplers.h"

t_ray	projection_ray(t_render_task *t, int x, int z, t_fvec2 sample)
{
	t_ray	ret;
	t_fvec3	l1;
	t_fvec3	l2;
	t_fvec3	pt;

	ret.pos = t->state->cam.cam.pos;
	l1 = fvec3_lerp(t->state->cam.x0y0, t->state->cam.x0y1,
			(z + sample.y) / t->state->screen_height);
	l2 = fvec3_lerp(t->state->cam.x1y0, t->state->cam.x1y1,
			(z + sample.y) / t->state->screen_height);
	pt = fvec3_norm(fvec3_lerp(l2, l1, (x + sample.x)
				/ t->state->screen_width));
	ret.dir = pt;
	return (ret);
}

t_fvec3	render_subpixel(t_render_task *t, t_ray ray, t_rand_state *rand_state)
{
	t_sampled_lambdas	lambdas;

	lambdas = sample_uniform(
			rand_float(rand_state), CIE_MIN_LAMBDA, CIE_MAX_LAMBDA);
	return (spectrum_to_xyz(cast_reflectable_ray_new(
				t->state, ray, lambdas, rand_state),
			lambdas));
}

int	render_pixel(t_render_task *t, int x, int z)
{
	t_fvec2			sample;
	t_sampler_state	sstate;
	t_fvec3			xyz_color;
	t_rand_state	rand_state;

	sstate = (t_sampler_state){.stratified_x = t->state->samples_x,
		.stratified_y = t->state->samples_y};
	xyz_color = (t_fvec3){0};
	rand_state = t->prng_state;
	xoroshiro128plusplus_jump(&t->prng_state);
	while (sample_stratified(&sstate, &sample))
	{
		if (t->exit_immediatelly == 1 || coord_to_idx(t->state, x, z)
			- t->start_pixel > t->num_pixels - 1)
		{
			*t->thrd_state = THRD_FINISHED;
			return (0);
		}
		xyz_color = fvec3_add(render_subpixel(t,
					projection_ray(t, x, z, sample), &rand_state), xyz_color);
	}
	t->state->s_colors[z * t->state->screen_width + x] = fvec3_add(t->state
			->s_colors[z * t->state->screen_width + x], fvec3_scale(xyz_color,
				1. / (sstate.stratified_x * sstate.stratified_y)));
	return (1);
}

void	*render_step(void *arg)
{
	t_render_task	*t;
	int				x;
	int				z;

	t = (t_render_task *)arg;
	idx_to_coords(t->state, t->start_pixel, &x, &z);
	while (z < t->state->screen_height)
	{
		while (x < t->state->screen_width)
		{
			if (!render_pixel(t, x, z))
				return (0);
			x++;
		}
		x = 0;
		z++;
	}
	*t->thrd_state = THRD_FINISHED;
	return (0);
}
