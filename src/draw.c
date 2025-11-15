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
#include "draw.h"

void	draw(t_state *state)
{
	int	total_runs_px;
	int	x;
	int	y;

	ft_memset(
		state->mlx_image->pixels, 255,
		state->mlx_image->width * state->mlx_image->height * sizeof(uint32_t));
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

void	render_single_thread(t_state *state)
{
	double			start;
	uint8_t			exit_flag;
	t_render_task	render_task;

	if (state->rndr.curr_px == 0 && state->rndr.render_once
		&& state->rndr.total_runs > 0)
		return ;
	start = mlx_get_time();
	while (mlx_get_time() - start < 1.0 / SINGLE_THREAD_TARGET_FPS)
	{
		if (state->rndr.curr_px < state->screen_width * state->screen_height)
		{
			render_task = (t_render_task){.state = state,
				.start_pixel = state->rndr.curr_px,
				.prng_state = move_tl_prng(state),
				.num_pixels = 1,
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

void	exit_hook(void *state_arg)
{
	t_state	*state;

	state = (t_state *)state_arg;
	emergency_exit(state);
}

#ifdef BONUS

void	loop_hook(void *state_param)
{
	t_state	*state;

	state = (t_state *)state_param;
	render_multithread(state);
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
