/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:14:01 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 14:14:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#include "libft/libft.h"
#include "minirt.h"
#include "rt_utils.h"
#include "samplers.h"
#include "draw.h"

t_rand_state	move_tl_prng(t_state *state)
{
	while (state->rndr.rand_px != state->rndr.curr_px)
	{
		state->rndr.rand_px++;
		state->rndr.rand_px = state->rndr.rand_px
			% (state->screen_width * state->screen_height);
		xoroshiro128plusplus_jump(&state->rndr.rand_state);
	}
	return (state->rndr.rand_state);
}

int	coord_to_idx(t_state *state, int x, int y)
{
	return (state->screen_width * y + x);
}

void	idx_to_coords(t_state *state, size_t idx, int *x, int *y)
{
	*x = idx % state->screen_width;
	*y = idx / state->screen_width;
}

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

void	*render_step(void *arg)
{
	t_render_task	*t;
	int				x;
	int				z;
	t_fvec2			sample;
	t_ray			curr_ray;

	t = (t_render_task *)arg;
	idx_to_coords(t->state, t->start_pixel, &x, &z);
	while (z < t->state->screen_height) {
		while (x < t->state->screen_width) {
			t_sampled_spec res_color = {0};
			t_sampler_state sstate = {.stratified_x = t->state->samples_x,
				.stratified_y = t->state->samples_y};
			t_fvec3 xyz_color = {0};
			t_rand_state rand_state = t->prng_state;
			xoroshiro128plusplus_jump(&t->prng_state);
			while (sample_stratified(&sstate, &sample)) {
				if (t->exit_immediatelly == 1
					|| coord_to_idx(t->state, x, z) - t->start_pixel > t->num_pixels - 1)
				{
					*t->thrd_state = THRD_FINISHED;
					return (0);
				}
				float lu = rand_float(&rand_state);
				t_sampled_lambdas lambdas = sample_uniform(lu, 360, 830);
				curr_ray = projection_ray(t, x, z, sample);
				res_color = cast_reflectable_ray_new(
					t->state, curr_ray, lambdas, t->state->rndr.max_reflections,
					&rand_state);
				xyz_color =
					fvec3_add(spectrum_to_xyz(res_color, lambdas), xyz_color);
			}
			xyz_color = fvec3_scale(xyz_color,
					1. / (sstate.stratified_x * sstate.stratified_y));
			t->state->s_colors[z * t->state->screen_width + x] = fvec3_add(
					t->state->s_colors[z * t->state->screen_width + x], xyz_color);
			x++;
		}
		x = 0;
		z++;
	}
	*t->thrd_state = THRD_FINISHED;
	return (0);
}

void	draw(t_state *state)
{
	int	total_runs_px;
	int	x;
	int	y;

	ft_printf("total_runs: %i\n", state->rndr.total_runs);
	ft_memset(
		state->mlx_image->pixels, 255,
		state->mlx_image->width * state->mlx_image->height * sizeof(uint32_t));
	ft_printf("%i\n", state->rndr.curr_px);
	y = -1;
	while (++y < state->screen_height)
	{
		x = -1;
		while (++x < state->screen_width)
		{
			total_runs_px = state->rndr.total_runs;
			if (coord_to_idx(state, x, y) <= state->rndr.curr_px)
				total_runs_px++;
			mlx_put_pixel(state->mlx_image, x, y,
				conv_8bcolor_to_uint32(xyz_to_rgb(fvec3_scale(
							state->s_colors[y * state->screen_width + x],
							1. / total_runs_px))));
		}
	}
}

int	thread_idx(t_state *state, int mask)
{
	int	j;

	j = -1;
	while (++j < state->rndr.num_threads)
		if (state->rndr.thrd_states[j] & mask)
			return (j);
	return (-1);
}

int	get_num_threads(t_state *state, int mask)
{
	int	ret;
	int	j;

	ret = 0;
	j = -1;
	while (++j < state->rndr.num_threads)
		if (state->rndr.thrd_states[j] & mask)
			ret++;
	return (ret);
}

void	render_single_thread(t_state *state)
{
	double	start;
	uint8_t	exit_flag;

	if (state->rndr.curr_px == 0 && state->rndr.render_once &&
		state->rndr.total_runs > 0)
		return ;
	start = mlx_get_time();
	while (mlx_get_time() - start < 1.0 / SINGLE_THREAD_TARGET_FPS)
	{
		if (state->rndr.curr_px < state->screen_width * state->screen_height)
		{
			t_render_task render_task = {.state = state,
				.start_pixel = state->rndr.curr_px,
				.prng_state = move_tl_prng(state),
				.num_pixels = 1,
				.thrd_state = &exit_flag};
			render_step(&render_task);
			state->rndr.curr_px++;
		}
		else
		{
			state->rndr.total_runs++;
			state->rndr.curr_px = 0;
			if (state->output_path.len)
			{
				draw(state);
				write_image_to_ppm(state->mlx_image, state->output_path.buff);
			}
			if (state->rndr.exit_after_render)
				exit_app(state);
		}
	}
}

void	launch_worker_threads(t_state *state)
{
	int	i;
	int	pos;

	i = -1;
	while (state->rndr.curr_px < state->screen_width * state->screen_height
		&& (thread_idx(state, THRD_NONE | THRD_FINISHED)) != -1
		&& ++i < state->rndr.num_threads)
	{
		pos = thread_idx(state, THRD_NONE | THRD_FINISHED);
		if (state->rndr.thrd_states[pos] == THRD_FINISHED)
			pthread_join(state->rndr.threads[pos], NULL);
		state->rndr.thrd_states[pos] = THRD_RUNNING;
		state->rndr.tasks[pos]
			= (t_render_task){.state = state,
			.start_pixel = state->rndr.curr_px,
			.num_pixels = state->rndr.chunk_size,
			.prng_state = move_tl_prng(state),
			.thrd_state = &state->rndr.thrd_states[pos]};
		state->rndr.curr_px += state->rndr.chunk_size;
		pthread_create(&state->rndr.threads[pos], NULL, render_step,
			&state->rndr.tasks[pos]);
	}
}

static void	collect_and_wait_multithreaded(t_state *state)
{
	int	curr;

	while (1)
	{
		curr = thread_idx(state, THRD_FINISHED);
		if (curr == -1)
			break ;
		pthread_join(state->rndr.threads[curr], NULL);
		state->rndr.thrd_states[curr] = THRD_NONE;
	}
	if (get_num_threads(state, THRD_NONE) == state->rndr.num_threads)
	{
		state->rndr.curr_px = 0;
		state->rndr.total_runs++;
		if (state->output_path.len)
			draw(state);
		if (state->output_path.len)
			write_image_to_ppm(state->mlx_image, state->output_path.buff);
		if (state->rndr.exit_after_render)
			exit_app(state);
	}
}

void	render_multithread(t_state *state)
{
	if (state->rndr.curr_px == 0 && state->rndr.render_once
		&& state->rndr.total_runs > 0)
		return ;
	if (state->rndr.curr_px < state->screen_width * state->screen_height)
		launch_worker_threads(state);
	else
		collect_and_wait_multithreaded(state);
}

void	emergency_exit(t_state *state)
{
	int	curr;
	int	i;

	i = 0;
	while (i < state->rndr.num_threads)
		state->rndr.tasks[i++].exit_immediatelly = 1;
	while (1)
	{
		curr = thread_idx(state, THRD_RUNNING | THRD_FINISHED);
		if (curr == -1)
			break ;
		pthread_join(state->rndr.threads[curr], NULL);
		state->rndr.thrd_states[curr] = THRD_NONE;
	}
}

void	exit_hook(void *state_arg)
{
	t_state	*state;

	state = (t_state *)state_arg;
	emergency_exit(state);
}

void	loop_hook(void *state_param)
{
	t_state	*state;

	state = (t_state *)state_param;
	render_multithread(state);
	draw(state);
}
