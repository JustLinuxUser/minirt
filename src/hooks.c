/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 17:51:58 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/15 17:51:58 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw.h"
#include "minirt.h"

void	exit_app(t_state *state)
{
	mlx_close_window(state->mlx);
}

void	exit_hook(void *state_arg)
{
	t_state	*state;

	state = (t_state *)state_arg;
	emergency_exit(state);
}
