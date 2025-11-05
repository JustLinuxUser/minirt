/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_getters2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:32:51 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 18:41:28 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"
#include "../mymath.h"
#include "../minirt.h"

bool	get_bool(t_rt_token tk)
{
	ft_assert(tk.t == RT_BOOL);
	return (tk.vals_f[0] != 0);
}

t_fvec2	get_fvec2(t_rt_token tk)
{
	ft_assert(tk.tuple_len == 2);
	return ((t_fvec2){.x = tk.vals_f[0], .y = tk.vals_f[1]});
}

int	push_color(t_rt_token tk, t_state *state, bool clamp)
{
	t_fvec3	color;
	t_fvec3	xyz;

	color = get_fvec3(tk);
	xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});
	vec_densely_sampled_spectrum_push(&state->spectrums,
		xyz_to_spectrum(xyz, clamp, 0));
	return (state->spectrums.len - 1);
}

int	push_blackbody(t_rt_token tk, t_state *state)
{
	float	temperature;

	temperature = get_float(tk);
	vec_densely_sampled_spectrum_push(&state->spectrums,
		densely_sampled_spectrum_blackbody(temperature));
	return (state->spectrums.len - 1);
}
