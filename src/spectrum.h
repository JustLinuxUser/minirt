/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spectrum.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:20:46 by mhornero          #+#    #+#             */
/*   Updated: 2025/08/18 21:03:31 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPECTRUM_H
# define SPECTRUM_H

# include "config.h"
# include "light.h"

/*SAMPLED SPECTRUM*/

typedef struct t_SampledSpectrum
{
	float	values[NUM_SPECTRUM_SAMPLES];
}	t_sampled_spec;

t_sampled_spec				sampled_spectrum_add(t_sampled_spec ret,
								t_sampled_spec to_add);
t_sampled_spec				sampled_spectrum_scale(t_sampled_spec ret,
								float scalar);

/*SAMPLED WAVELENGTHS*/

typedef struct t_SampledWavelengths
{
	float	lambda[NUM_SPECTRUM_SAMPLES]; //wavelengths
	float	pdf[NUM_SPECTRUM_SAMPLES]; //probability density
}	t_sampled_lambdas;

float						lerp(float x, float a, float b);
t_sampled_lambdas			sample_uniform(float u,
								float lambda_min, float lambda_max);
void						cie_xyz(float lambda, float *x, float *y, float *z);

float						blackbody(float lambda, float T);
t_densely_sampled_spectrum	xyz_to_spectrum(t_fvec3 target_xyz,
								bool clamp, float *err);
#endif
