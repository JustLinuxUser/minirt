/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 11:27:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 11:27:12 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "mymath.h"

t_fvec3	perspective_cam_ray(t_state *state, t_fvec2 px, t_fvec2 sample)
{
	return (fvec3_norm((t_fvec3){
			.x = (px.x + sample.x - state->screen_width / 2.)
			* state->cam.proj_coef,
			.y = state->cam.screen_dist,
			.z = (px.y + sample.y - state->screen_height / 2.)
			* state->cam.proj_coef}));
}
