/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spectrum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:20:37 by mhornero          #+#    #+#             */
/*   Updated: 2025/08/01 16:04:06 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>

/*SAMPLED SPECTRUM*/

t_SampledSpectrum create_sample_spectrum(float span[NUM_SPECTRUM_SAMPLES])
{
    t_SampledSpectrum spec;
    int i = -1;
    
    while (++i < NUM_SPECTRUM_SAMPLES)
        spec.values[i] = span[i];
    return spec;
}

t_SampledSpectrum sampled_spectrum_add(t_SampledSpectrum ret, t_SampledSpectrum to_add)
{
    int i = -1;
    
    while (++i < NUM_SPECTRUM_SAMPLES)
        ret.values[i] += to_add.values[i];
    return ret;
}

t_SampledSpectrum sampled_spectrum_scale(t_SampledSpectrum ret, float scalar)
{
    int i = -1;
    
    while (++i < NUM_SPECTRUM_SAMPLES)
        ret.values[i] *= scalar;
    return ret;
}

float lerp(float x, float a, float b)
{
    return (1 - x) * a + x * b;
}

/*SAMPLED WAVELENGTHS*/

t_SampledWavelengths SampleUniform(float u, float lambda_min, float lambda_max)
{
    t_SampledWavelengths ret;
    float delta = (lambda_max - lambda_min) / NUM_SPECTRUM_SAMPLES;

    ret.lambda[0] = lerp(u, lambda_min, lambda_max);

    for (int i = 1; i < NUM_SPECTRUM_SAMPLES; i++) {
        //ret.lambda[i] = lambda_min + (i + u) * delta;
        ret.lambda[i] = ret.lambda[i - 1] + delta;
        if (ret.lambda[i] > lambda_max)
            ret.lambda[i] = lambda_min + (ret.lambda[i] - lambda_max);
    }

    for (int i = 0; i < NUM_SPECTRUM_SAMPLES; i++) {
        ret.pdf[i] = 1.f / (lambda_max - lambda_min); 
    }
    //FOR NOW, A KIND OF SOLUTION
    // ret.lambda[0] = 430.f;
    // ret.lambda[1] = 530.f;
    // ret.lambda[2] = 630.f;
    // ret.lambda[3] = 830.f;
    //printf("lambdas: %f, %f, %f, %f\n", ret.lambda[0], ret.lambda[1], ret.lambda[2], ret.lambda[3]);
    return ret;
}

/*SPECTRUM*/

/*???*/

/*COLOR*/

float clamp(float x, float minVal, float maxVal)
{
    if (x < minVal)
        return minVal;
    else if (x > maxVal)
        return maxVal;
    else
        return x;
}


/*FROM sRGB conversion matrix XYZ to RGB*/
t_ColorRGB xyz_to_rgb(float x, float y, float z)
{
    t_ColorRGB rgb;
    rgb.r =  3.2406f * x - 1.5372f * y - 0.4986f * z;
    rgb.g = -0.9689f * x + 1.8758f * y + 0.0415f * z;
    rgb.b =  0.0557f * x - 0.2040f * y + 1.0570f * z;
    return rgb;
}

/*WAVELENGTHS TO XYZ (invented converision values based on graphic)*/
void cie_xyz(float lambda, float *x, float *y, float *z)
{
    if (lambda <= CIE_MIN_LAMBDA) {
        *x = CIE_X[0];
        *y = CIE_Y[0];
        *z = CIE_Z[0];
        return;
    }
    if (lambda >= CIE_MAX_LAMBDA) {
        *x = CIE_X[CIE_SAMPLES-1];
        *y = CIE_Y[CIE_SAMPLES-1];
        *z = CIE_Z[CIE_SAMPLES-1];
        return;
    }
    // float pos = (lambda - CIE_MIN_LAMBDA) / CIE_STEP;
    float pos = lambda - CIE_MIN_LAMBDA;
    int idx = (int)pos;
    float t = pos - idx;
    // float t = lambda - (CIE_MIN_LAMBDA + idx);
    *x = CIE_X[idx] * (1.0f - t) + CIE_X[idx+1] * t;
    *y = CIE_Y[idx] * (1.0f - t) + CIE_Y[idx+1] * t;
    *z = CIE_Z[idx] * (1.0f - t) + CIE_Z[idx+1] * t;
}

t_ColorRGB spectrum_to_rgb(t_SampledSpectrum s, t_SampledWavelengths lambda) {
    float X = 0.0f, Y = 0.0f, Z = 0.0f;

    for (int i = 0; i < NUM_SPECTRUM_SAMPLES; ++i) {
        float l = lambda.lambda[i];
        float pdf = lambda.pdf[i];
        float value = s.values[i];
        
        //They use saveDiv, but was easier for now
        if (pdf == 0.0f)
            continue;

        //Need the color evaluation (static for now)
        //Need to understand chromacity
        float x_bar, y_bar, z_bar;
        cie_xyz(l, &x_bar, &y_bar, &z_bar);
        
        X += value * x_bar / pdf;
        Y += value * y_bar / pdf;
        Z += value * z_bar / pdf;
    }

    //The average part
    X /= NUM_SPECTRUM_SAMPLES;
    Y /= NUM_SPECTRUM_SAMPLES;
    Z /= NUM_SPECTRUM_SAMPLES;
    
    //Correction by the CIE_Y integral
    X /= CIE_Y_integral;
    Y /= CIE_Y_integral;
    Z /= CIE_Y_integral;
    
    return xyz_to_rgb(X, Y, Z);
}

t_ColorRGB clamp_rgb(t_ColorRGB c) {
    t_ColorRGB out;
    out.r = clamp(c.r, 0.0f, 1.0f);
    out.g = clamp(c.g, 0.0f, 1.0f);
    out.b = clamp(c.b, 0.0f, 1.0f);
    return out;
}

// int main()
// {
//     float span[NUM_SPECTRUM_SAMPLES] = { 0.1f, 0.1f, 0.1f, 0.1f };
//     t_SampledSpectrum spec = create_sample_spectrum(span);
//     printf("spec: %.3f %.3f %.3f %.3f\n", spec.values[0], spec.values[1], spec.values[2], spec.values[3]); 
//     t_SampledWavelengths lambda = SampleUniform(0.f, 360.0f, 830.0f);
//     printf("lamda: %.3f %.3f %.3f %.3f\n", lambda.lambda[0], lambda.lambda[1], lambda.lambda[2], lambda.lambda[3]); 
//     printf("lamda: %.3f %.3f %.3f %.3f\n", lambda.pdf[0], lambda.pdf[1], lambda.pdf[2], lambda.pdf[3]); 
//     t_ColorRGB rgb = spectrum_to_rgb(spec, lambda);
//     //t_ColorRGB rgb_clamped = clamp_rgb(rgb);
//     printf("RGB: %f %f %f\n", rgb.r, rgb.g, rgb.b);

//     return (0);
// }