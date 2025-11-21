/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:34:57 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:41:30 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cie.h"
#include "light_structs.h"
#include "spectrum.h"

//Calculate information for lights -> DenselySampledSpectrum
t_densely_sampled_spectrum	densely_sampled_spectrum_blackbody(float T)
{
	t_densely_sampled_spectrum	ret;
	int							i;
	float						bb_const;

	i = CIE_MIN_LAMBDA;
	bb_const = blackbody(2.89e-3f / T * 1e9f, T);
	while (i <= (int)CIE_MAX_LAMBDA)
	{
		ret.samples[i - (int)CIE_MIN_LAMBDA] = blackbody(i, T)
			/ bb_const;
		i++;
	}
	return (ret);
}

void	add_light(t_lights *lights, t_light to_add)
{
	if (!lights)
		return ;
	vec_light_push(&lights->lights, to_add);
	lights->total_power += to_add.intensity;
}

void	calculate_pdfs(t_lights *lights)
{
	size_t	i;

	i = 0;
	free(lights->pdfs.buff);
	vec_float_init(&lights->pdfs, lights->lights.len);
	while (i < lights->lights.len)
	{
		lights->pdfs.buff[i] = lights->lights.buff[i].intensity
			/ lights->total_power;
		i++;
	}
}
