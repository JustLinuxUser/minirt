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

#include "light.h"
#define NUM_SPECTRUM_SAMPLES 4

/*SAMPLED SPECTRUM*/

typedef struct t_SampledSpectrum {
    float values[NUM_SPECTRUM_SAMPLES];
} t_SampledSpectrum;

t_SampledSpectrum create_sample_spectrum(float span[NUM_SPECTRUM_SAMPLES]);
t_SampledSpectrum sampled_spectrum_add(t_SampledSpectrum ret, t_SampledSpectrum to_add);
t_SampledSpectrum sampled_spectrum_scale(t_SampledSpectrum ret, float scalar);

/*SAMPLED WAVELENGTHS*/

typedef struct t_SampledWavelengths {
    float lambda[NUM_SPECTRUM_SAMPLES]; //wavelengths
    float pdf[NUM_SPECTRUM_SAMPLES]; //probability density

} t_SampledWavelengths;

float lerp(float x, float a, float b);
t_SampledWavelengths SampleUniform(float u, float lambda_min, float lambda_max);
void cie_xyz(float lambda, float *x, float *y, float *z);


/*SPECTRUM????*/

typedef struct t_PieceWiseSpectrum {
    float* lambdas;
    float* values;
} t_PieceWiseSpectrum;

/*COLOR*/

typedef struct t_ColorRGB {
    float r;
    float g;
    float b;
} t_ColorRGB;


//#TODO: BLACKBODYEMITTERS
float blackbody(float lambda, float T);

t_densely_sampled_spectrum xyz_to_spectrum(t_fvec3 target_xyz, bool clamp, float *err);
#endif
