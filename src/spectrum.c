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
#include <stdio.h>
#include "cie.h"
#include "libft/libft.h"

/*SAMPLED SPECTRUM*/

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

t_fvec3 densely_sampled_spectrum_to_xyz(t_densely_sampled_spectrum s) {
    float X = 0.0f, Y = 0.0f, Z = 0.0f;

    for (int i = CIE_MIN_LAMBDA; i <= CIE_MAX_LAMBDA; ++i) {
        float l = i;
        float value = s.samples[i - (int)CIE_MIN_LAMBDA];
     
        //Need the color evaluation (static for now)
        //Need to understand chromacity
        float x_bar, y_bar, z_bar;
        cie_xyz(l, &x_bar, &y_bar, &z_bar);
        X += value * x_bar;
        Y += value * y_bar;
        Z += value * z_bar;
    }

    //The average part
    X /= CIE_SAMPLES / CIE_Y_integral;
    Y /= CIE_SAMPLES / CIE_Y_integral;
    Z /= CIE_SAMPLES / CIE_Y_integral;
    //Correction by the CIE_Y integral
    X /= CIE_Y_integral;
    Y /= CIE_Y_integral;
    Z /= CIE_Y_integral;
 
    return (t_fvec3){X, Y, Z};
}

t_color clamp_rgb(t_color c) {
    t_color out;
    out.x = clamp(c.x, 0.0f, 1.0f);
    out.y = clamp(c.y, 0.0f, 1.0f);
    out.z = clamp(c.z, 0.0f, 1.0f);
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

float blackbody(float lambda, float t)
{
    if (t <= 0)
    {
        return 0;
    }
    float c = 299792458.f;
    float h =  6.62606957e-34f;
    float kb = 1.3806488e-23f;
    
    float l = lambda * 1e-9f;
    float Le = (2 * h * c * c) / (powf(l, 5) * (expf((h * c) / (l * kb * t)) - 1));
    //printf("LE: %f\n", Le);
    return Le;
}

// typedef struct s_sigmoid {
// 	float c1, c2, c3;
// } t_sigmoid;
//
// float sigmoid(float x) 
// {
// 	if (isinf(x))
// 	{
// 		if (x > 0)
// 		 return 1;
// 		return 0;
// 	}
// 	return (0.5 + x / (2 * sqrtf(1 + x * x)));
// }
//
// float eval_sigmoid(t_sigmoid sigm, float lambda) {
// 	float f = eval_poly_2(lambda, sigm.c1, sigm.c2, sigm.c3);
// 	// printf("%f\n", f);
// 	return sigmoid(f);
// }
//
// void sample_sigmoid(t_sigmoid sigm, t_densely_sampled_spectrum *spec) {
//     int i = CIE_MIN_LAMBDA;
//     while (i < CIE_MAX_LAMBDA)
//     {
//         spec->samples[i - (int)CIE_MIN_LAMBDA] = eval_sigmoid(sigm, i);
//         i++;
//     }
// }
//
// void print_spec_color(t_densely_sampled_spectrum *spec) {
//
// 	t_color xyz = densely_sampled_spectrum_to_xyz(*spec);
// 	ft_printf("x: %f, y: %f, z: %f\n", xyz.x, xyz.y, xyz.z);
// 	t_8bcolor color = xyz_to_rgb(xyz);
// 	ft_printf("%i %i %i\n", color.r, color.g, color.b);
// 	ft_printf("\033[48;2;%i;%i;%im                       \033[0m\n", color.r, color.g, color.b);
// }
//
// void print_densly_sampled_spec(t_densely_sampled_spectrum *spec) {
// 	for (int i = 0; i <= CIE_SAMPLES; i++) {
// 		ft_printf("%.10f", spec->samples[i]);
// 		for (int j = 0; j < spec->samples[i] * 50; j++) ft_printf(" ");
// 		ft_printf("*\n");
// 	}
// }
//
// t_fvec3 sigm_to_xyz(t_sigmoid sig) {
// 	t_densely_sampled_spectrum spec = {0}; //calculate_densely_sampled_spectrum(6200);
// 	sample_sigmoid(sig, &spec);
//
// 	print_spec_color(&spec);
// 	return densely_sampled_spectrum_to_xyz(spec);
// }
//
//
inline static float color_error(t_fvec3 c0, t_fvec3 c1) {
	return fvec3_len_sq(fvec3_sub(c0, c1));
}

t_densely_sampled_spectrum xyz_to_spectrum(t_fvec3 target_xyz, bool clamp, float *err)
{

	t_densely_sampled_spectrum spec = {0};

	ft_memcpy(spec.samples, CIE_X, sizeof(CIE_X));
	float x_contrib = densely_sampled_spectrum_to_xyz(spec).x;

	ft_memcpy(spec.samples, CIE_Y, sizeof(CIE_Y));
	float y_contrib = densely_sampled_spectrum_to_xyz(spec).y;

	ft_memcpy(spec.samples, CIE_Z, sizeof(CIE_Z));
	float z_contrib = densely_sampled_spectrum_to_xyz(spec).z;

	t_densely_sampled_spectrum curr_spec = {0};

	for (int i = 0; i < 10; i++) {
		for (int i = 0; i < 100; i++) {
			t_fvec3 curr_xyz = densely_sampled_spectrum_to_xyz(curr_spec);

			if (i % 3 == 0) {
				float diff_x = target_xyz.x - curr_xyz.x;
				float factor_x = diff_x / x_contrib / 10;
				for (int i = 0; i < CIE_SAMPLES; i++) {
					curr_spec.samples[i] += CIE_X[i] * factor_x;
				}
			} else if (i % 3 == 1) {
				float diff_y = target_xyz.y - curr_xyz.y;
				float factor_y = diff_y / y_contrib / 10;
				for (int i = 0; i < CIE_SAMPLES; i++) {
					curr_spec.samples[i] += CIE_Y[i] * factor_y;
				}
			} else {
				float diff_z = target_xyz.z - curr_xyz.z;
				float factor_z = diff_z / z_contrib / 10;
				for (int i = 0; i < CIE_SAMPLES; i++) {
					curr_spec.samples[i] += CIE_Z[i] * factor_z;
				}
			}
		}
		for (int i = 0; i < CIE_SAMPLES; i++) {
			curr_spec.samples[i] = fmax(curr_spec.samples[i], 0);
		}
		t_fvec3 curr_xyz = densely_sampled_spectrum_to_xyz(curr_spec);
		if (err)
			*err = color_error(target_xyz, curr_xyz);
	}
	if (clamp) {
		float max = 0;
		for (int i = 0; i < CIE_SAMPLES; i++) {
			curr_spec.samples[i] = fmax(curr_spec.samples[i], 0);
			max = fmax(curr_spec.samples[i], max);
		}
		max = fmax(max, 1);
		for (int i = 0; i < CIE_SAMPLES; i++) {
			curr_spec.samples[i] /= max;
		}
	}
	// print_spec_color(&curr_spec);
	return curr_spec;
}
