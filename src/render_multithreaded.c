/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_multithreaded.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:08:04 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/13 19:08:04 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw.h"
#include "minirt.h"

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
		finish_render_loop(state);
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
