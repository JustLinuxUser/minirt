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
#include "libft/libft.h"

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

float	lerp(float x, float a, float b)
{
	return ((1 - x) * a + x * b);
}

/*SAMPLED WAVELENGTHS*/

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

float	fclamp(float x, float min, float max)
{
	if (x < min)
		return (min);
	else if (x > max)
		return (max);
	else
		return (x);
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

t_color	clamp_rgb(t_color c)
{
	t_color	out;

	out.x = fclamp(c.x, 0.0f, 1.0f);
	out.y = fclamp(c.y, 0.0f, 1.0f);
	out.z = fclamp(c.z, 0.0f, 1.0f);
	return (out);
}

float	blackbody(float lambda, float t)
{
	float	c;
	float	h;
	float	l ;
	float	le;

	if (t <= 0)
		return (0);
	c = 299792458.f;
	h = 6.62606957e-34f;
	l = lambda * 1e-9f;
	le = (2 * h * c * c) / (powf(l, 5) * (expf((h * c)
					/ (l * 1.3806488e-23f * t)) - 1));
	return (le);
}

inline static float	color_error(t_fvec3 c0, t_fvec3 c1)
{
	return (fvec3_len_sq(fvec3_sub(c0, c1)));
}

void add_contrib(t_densely_sampled_spectrum *spec, float diff, float contrib, float *cie)
{
	float factor = diff / contrib / 10;

	for (int i = 0; i < CIE_SAMPLES; i++) {
		spec->samples[i] += cie[i] * factor;
	}
}

void	clamp_spectre(t_densely_sampled_spectrum *spec)
{
	float	max;
	int		i;

	max = 0;
	i = 0;
	while (i < CIE_SAMPLES)
	{
		spec->samples[i] = fmax(spec->samples[i], 0);
		max = fmax(spec->samples[i], max);
		i++;
	}
	max = fmax(max, 1);
	i = 0;
	while (i < CIE_SAMPLES)
		spec->samples[i++] /= max;
}

void	reduce_err_iter(t_densely_sampled_spectrum *spec,
			t_fvec3 target, t_fvec3 contrib)
{
	t_fvec3	curr;
	int		i;

	i = 0;
	while (i < 100)
	{
		curr = densely_sampled_spectrum_to_xyz(spec);
		if (i % 3 == 0)
			add_contrib(spec, target.x - curr.x, contrib.x, cie_x());
		else if (i % 3 == 1)
			add_contrib(spec, target.y - curr.y, contrib.y, cie_y());
		else
			add_contrib(spec, target.z - curr.z, contrib.z, cie_z());
		i++;
	}
	i = 0;
	while (i < CIE_SAMPLES)
	{
		spec->samples[i] = fmax(spec->samples[i], 0);
		i++;
	}
}
t_densely_sampled_spectrum	xyz_to_spectrum(t_fvec3 target,
		bool clamp, float *err)
{
	t_densely_sampled_spectrum	spec;
	t_fvec3						contrib;
	int							i;
	t_fvec3						curr_xyz;

	ft_memcpy(spec.samples, cie_x(), sizeof(spec.samples));
	contrib.x = densely_sampled_spectrum_to_xyz(&spec).x;
	ft_memcpy(spec.samples, cie_y(), sizeof(spec.samples));
	contrib.y = densely_sampled_spectrum_to_xyz(&spec).y;
	ft_memcpy(spec.samples, cie_z(), sizeof(spec.samples));
	contrib.z = densely_sampled_spectrum_to_xyz(&spec).z;
	spec = (t_densely_sampled_spectrum){0};
	i = 0;
	while (i < 10)
	{
		curr_xyz = densely_sampled_spectrum_to_xyz(&spec);
		reduce_err_iter(&spec, target, contrib);
		if (err)
			*err = color_error(target, curr_xyz);
		i++;
	}
	if (clamp)
		clamp_spectre(&spec);
	return (spec);
}
