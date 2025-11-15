/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:43:39 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/13 18:43:39 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "draw.h"
#include "rt_utils.h"

int	coord_to_idx(t_state *state, int x, int y)
{
	return (state->screen_width * y + x);
}

void	idx_to_coords(t_state *state, size_t idx, int *x, int *y)
{
	*x = idx % state->screen_width;
	*y = idx / state->screen_width;
}

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

void	finish_render_loop(t_state *state)
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
