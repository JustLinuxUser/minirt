/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spectrum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:20:37 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:41:28 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "cie.h"

t_sampled_spec	sampled_spectrum_add(t_sampled_spec ret, t_sampled_spec to_add)
{
	int	i;

	i = -1;
	while (++i < NUM_SPECTRUM_SAMPLES)
		ret.values[i] += to_add.values[i];
	return (ret);
}

t_sampled_spec	sampled_spectrum_scale(t_sampled_spec ret, float scalar)
{
	int	i;

	i = -1;
	while (++i < NUM_SPECTRUM_SAMPLES)
		ret.values[i] *= scalar;
	return (ret);
}

/*WAVELENGTHS TO XYZ (invented converision values based on graphic)*/
void	cie_xyz(float lambda, float *x, float *y, float *z)
{
	float	pos;
	int		idx;
	float	t;

	if (lambda <= CIE_MIN_LAMBDA)
	{
		*x = cie_x()[0];
		*y = cie_y()[0];
		*z = cie_z()[0];
		return ;
	}
	if (lambda >= CIE_MAX_LAMBDA)
	{
		*x = cie_x()[CIE_SAMPLES - 1];
		*y = cie_y()[CIE_SAMPLES - 1];
		*z = cie_z()[CIE_SAMPLES - 1];
		return ;
	}
	pos = lambda - CIE_MIN_LAMBDA;
	idx = (int)pos;
	t = pos - idx;
	*x = cie_x()[idx] * (1.0f - t) + cie_x()[idx + 1] * t;
	*y = cie_y()[idx] * (1.0f - t) + cie_y()[idx + 1] * t;
	*z = cie_z()[idx] * (1.0f - t) + cie_z()[idx + 1] * t;
}

t_fvec3	densely_sampled_spectrum_to_xyz(t_densely_sampled_spectrum *s)
{
	t_fvec3	ret;
	t_fvec3	bar;
	float	value;
	int		i;

	ret = (t_fvec3){0};
	bar = (t_fvec3){0};
	i = CIE_MIN_LAMBDA;
	while (i <= CIE_MAX_LAMBDA)
	{
		value = s->samples[i - (int)CIE_MIN_LAMBDA];
		cie_xyz(i, &bar.x, &bar.y, &bar.z);
		ret.x += value * bar.x;
		ret.y += value * bar.y;
		ret.z += value * bar.z;
		i++;
	}
	ret.x /= CIE_SAMPLES / CIE_Y_INTEGRAL;
	ret.y /= CIE_SAMPLES / CIE_Y_INTEGRAL;
	ret.z /= CIE_SAMPLES / CIE_Y_INTEGRAL;
	ret.x /= CIE_Y_INTEGRAL;
	ret.y /= CIE_Y_INTEGRAL;
	ret.z /= CIE_Y_INTEGRAL;
	return (ret);
}

float	densely_sapmled_spectrum_avg_power(t_densely_sampled_spectrum *s)
{
	float	sum;
	int		i;

	sum = 0;
	i = 0;
	while (i < CIE_SAMPLES)
	{
		sum += s->samples[i];
		i++;
	}
	return (sum / CIE_SAMPLES);
}
