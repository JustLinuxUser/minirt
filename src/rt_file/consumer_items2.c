/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_items2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 10:12:33 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 10:12:33 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"
#include "../cie.h"

bool	process_ambiant(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	float		scale;
	t_fvec3		color;
	t_fvec3		xyz;
	int			i;

	if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 1.0))
		return (false);
	scale = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	color = get_fvec3(nd.token);
	xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});
	tl->state->ambiant_light_spec = xyz_to_spectrum(xyz, false, 0);
	i = 0;
	while (i < CIE_SAMPLES)
	{
		tl->state->ambiant_light_spec.samples[i] *= scale;
		i++;
	}
	return (true);
}

static bool	process_light_color(t_rt_consumer_tl *tl,
				bool is_blackbody, t_light *l)
{
	t_rt_node	nd;

	if (is_blackbody)
	{
		if (get_tl_typed(tl, "temperature", RT_ND_TUPLE_F1, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 100000))
			return (false);
		l->spec_idx = push_blackbody(nd.token, tl->state);
	}
	else
	{
		if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 255))
			return (false);
		l->spec_idx = push_color(nd.token, tl->state, false);
	}
	return (true);
}

bool	process_light(t_rt_consumer_tl *tl, bool is_blackbody)
{
	t_rt_node	nd;
	t_light		l;

	l = (t_light){.t = POINT_LIGHT};
	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);
	if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (!process_light_color(tl, is_blackbody, &l))
		return (false);
	add_light(&tl->state->lights, l);
	return (true);
}

bool	process_inf_light(t_rt_consumer_tl *tl, bool is_blackbody)
{
	t_rt_node	nd;
	t_light		l;

	l = (t_light){.t = DISTANT_LIGHT};
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);
	if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (!process_light_color(tl, is_blackbody, &l))
		return (false);
	add_light(&tl->state->lights, l);
	return (true);
}

bool	process_sky(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	t_light		l;

	l = (t_light){0};
	if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	l.t = AMBIANT_LIGHT;
	l.spec_idx = push_color(nd.token, tl->state, false);
	tl->state->sky_light_idx = tl->state->lights.lights.len;
	add_light(&tl->state->lights, l);
	return (true);
}
