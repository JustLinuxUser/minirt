/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spectrum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:20:37 by mhornero          #+#    #+#             */
/*   Updated: 2025/07/25 14:17:33 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

    ret.lambda[0] = lerp(u, lambda_min, lambda_max);

    float delta = (lambda_max - lambda_min) / NUM_SPECTRUM_SAMPLES;
    int i = 0;
    while (++i < NUM_SPECTRUM_SAMPLES)
    {
        ret.lambda[i] = ret.lambda[i - 1] + delta;
        if (ret.lambda[i] > lambda_max)
            ret.lambda[i] = lambda_min + (ret.lambda[i] - lambda_min);
    }
    i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
        ret.pdf[i] = 1 / (lambda_max - lambda_min);
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
    //Should be a LookUp table
    if (lambda < 450) {
        *x = 0.1f; *y = 0.0f; *z = 0.6f;
    } else if (lambda < 600) {
        *x = 0.0f; *y = 0.6f; *z = 0.2f;
    } else {
        *x = 0.6f; *y = 0.2f; *z = 0.0f;
    }
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

    return xyz_to_rgb(X, Y, Z);
}

t_ColorRGB clamp_rgb(t_ColorRGB c) {
    t_ColorRGB out;
    out.r = clamp(c.r, 0.0f, 255.0f);
    out.g = clamp(c.g, 0.0f, 255.0f);
    out.b = clamp(c.b, 0.0f, 255.0f);
    return out;
}

// int main()
// {
//     float span[NUM_SPECTRUM_SAMPLES] = { 0.2f, 0.5f, 0.9f };
//     t_SampledSpectrum spec = create_sample_spectrum(span);
//     t_SampledWavelengths lambda = SampleUniform(0.5f, 400.0f, 700.0f);
//     t_ColorRGB rgb = spectrum_to_rgb(spec, lambda);
//     //t_ColorRGB rgb_clamped = clamp_rgb(rgb);
//     printf("RGB: %.3f %.3f %.3f\n", rgb.r, rgb.g, rgb.b);

//     return (0);
// }