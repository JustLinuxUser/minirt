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
	emergency_exit(state);
	if (!state->rndr.headless)
		mlx_close_window(state->mlx);
}

void	exit_hook(void *state_arg)
{
	t_state	*state;

	state = (t_state *)state_arg;
	exit_app(state);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_state	*state;

	state = (t_state *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_app(state);
}
