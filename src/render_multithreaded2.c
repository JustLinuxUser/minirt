/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_multithreaded2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:09:44 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/13 19:09:44 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw.h"
#include "minirt.h"

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
