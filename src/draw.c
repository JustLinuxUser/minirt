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
#include <threads.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#include "libft/libft.h"
#include "minirt.h"
#include "draw.h"

void	draw_pixel(uint8_t *pixel, uint32_t color)
{
	*(pixel++) = (uint8_t)(color >> 24);
	*(pixel++) = (uint8_t)(color >> 16);
	*(pixel++) = (uint8_t)(color >> 8);
	*(pixel++) = (uint8_t)(color & 0xFF);
}

void	draw(t_state *state)
{
	int			total_runs_px;
	int			x;
	int			y;
	uint32_t	*pixels;

	pixels = state->img_buffer;
	if (state->mlx_image != 0)
		pixels = (uint32_t *)state->mlx_image->pixels;
	ft_memset(pixels, 255,
		state->screen_width * state->screen_height * sizeof(uint32_t));
	y = -1;
	while (++y < state->screen_height)
	{
		x = -1;
		while (++x < state->screen_width)
		{
			total_runs_px = state->rndr.total_runs;
			if (coord_to_idx(state, x, y) <= state->rndr.curr_px)
				total_runs_px++;
			draw_pixel((uint8_t *)&pixels[x + y * state->screen_width],
				conv_8bcolor_to_uint32(xyz_to_rgb(fvec3_scale(
							state->s_colors[y * state->screen_width + x],
							1. / total_runs_px))));
		}
	}
}

void	render_single_thread(t_state *state)
{
	double			start;
	uint8_t			exit_flag;
	t_render_task	render_task;

	if (state->rndr.curr_px == 0 && state->rndr.render_once
		&& state->rndr.total_runs > 0)
		return ;
	start = mlx_get_time();
	while (mlx_get_time() - start < 1.0 / SINGLE_THREAD_TARGET_FPS
		&& !state->rndr.should_stop)
	{
		if (state->rndr.curr_px < state->screen_width * state->screen_height)
		{
			render_task = (t_render_task){.state = state,
				.start_pixel = state->rndr.curr_px,
				.prng_state = move_tl_prng(state), .num_pixels = 1,
				.thrd_state = &exit_flag};
			render_step(&render_task);
			state->rndr.curr_px++;
		}
		else
		{
			finish_render_loop(state);
		}
	}
}

#ifdef BONUS

void	loop_hook(void *state_param)
{
	t_state	*state;

	state = (t_state *)state_param;
	render_multithread(state);
	if (state->rndr.headless)
		thrd_yield();
	draw(state);
}
#else

void	loop_hook(void *state_param)
{
	t_state	*state;

	state = (t_state *)state_param;
	render_single_thread(state);
	draw(state);
}
#endif
