/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   samplers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:37:14 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:44:44 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "samplers.h"
#include "float_utils.h"
#include "spectrum.h"
#include <stdbool.h>

t_fvec3	rand_direction(t_rand_state *rand_state)
{
	t_fvec3	ret;
	float	lensq;

	while (1)
	{
		ret.x = rand_float(rand_state) * 2. - 1.;
		ret.y = rand_float(rand_state) * 2. - 1.;
		ret.z = rand_float(rand_state) * 2. - 1.;
		lensq = fvec3_len_sq(ret);
		if (lensq > MACHINE_EPSILON && lensq < 1)
			break ;
	}
	return (fvec3_norm(ret));
}

t_fvec3	rand_halfsphere(t_fvec3 norm, t_rand_state *rand_state)
{
	t_fvec3	ret;

	ret = rand_direction(rand_state);
	if (fvec3_dot(ret, norm) < 0)
		ret = fvec3_invert(ret);
	return (ret);
}

bool	sample_stratified(t_sampler_state *state, t_fvec2 *sample)
{
	sample->x = (float)state->last_x / state->stratified_x;
	sample->y = (float)state->last_y / state->stratified_y;
	if (state->last_y == state->stratified_y)
		return (false);
	state->last_x++;
	if (state->last_x == state->stratified_x)
	{
		state->last_x = 0;
		state->last_y++;
	}
	return (true);
}

t_sampled_lambdas	sample_uniform(float u, float lambda_min, float lambda_max)
{
	t_sampled_lambdas	ret;
	float				delta;
	int					i;

	delta = (lambda_max - lambda_min) / NUM_SPECTRUM_SAMPLES;
	ret.lambda[0] = lerp(u, lambda_min, lambda_max);
	i = 1;
	while (i < NUM_SPECTRUM_SAMPLES)
	{
		ret.lambda[i] = ret.lambda[i - 1] + delta;
		if (ret.lambda[i] > lambda_max)
			ret.lambda[i] = lambda_min + (ret.lambda[i] - lambda_max);
		i++;
	}
	i = 0;
	while (i < NUM_SPECTRUM_SAMPLES)
	{
		ret.pdf[i] = 1.f / (lambda_max - lambda_min);
		i++;
	}
	return (ret);
}
