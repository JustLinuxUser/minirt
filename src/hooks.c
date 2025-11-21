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
#include "libft/ft_printf/ft_printf.h"
#include "minirt.h"

void	exit_app(t_state *state)
{
	ft_eprintf("Exiting...\n");
	state->rndr.should_stop = true;
	if (!state->rndr.headless)
		mlx_close_window(state->mlx);
}

void	exit_hook(void *state_arg)
{
	t_state	*state;

	state = (t_state *)state_arg;
	emergency_exit(state);
	ft_eprintf("Exiting...\n");
}
