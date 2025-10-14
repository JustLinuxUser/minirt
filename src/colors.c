#include "minirt.h"
#include <math.h>
#include "cie.h"

#include <stdlib.h>

float linear_to_gamma(float c) {
    if (c < 0)
        return 0;
    return (sqrt(c));
}
t_8bcolor ColortoRGB(t_color c) {
    return (t_8bcolor){.a = 255,
                   .r = fmin(linear_to_gamma(c.x) * 255, 255),
                   .g = fmin(linear_to_gamma(c.y) * 255, 255),
                   .b = fmin(linear_to_gamma(c.z) * 255, 255)};
}

//THIS
// float linear_to_srgb(float x) {
//     if (x <= 0.0031308f)
//         return 12.92f * x;
//     else
//         return 1.055f * powf(x, 1.0f/2.4f) - 0.055f;
// }

float random_generator(){
    return (float)rand()/RAND_MAX;
}

t_fvec3 SpectrumToXYZ(t_SampledSpectrum s, t_SampledWavelengths lambda)
{
    //float X = 0.0f, Y = 0.0f, Z = 0.0f;
    t_fvec3 ret = {0};

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
        
        ret.x += value * x_bar / pdf;
        ret.y += value * y_bar / pdf;
        ret.z += value * z_bar / pdf;
    }

    //The average part
    ret.x /= NUM_SPECTRUM_SAMPLES;
    ret.y /= NUM_SPECTRUM_SAMPLES;
    ret.z /= NUM_SPECTRUM_SAMPLES;
    
    //Correction by the CIE_Y integral
    ret.x /= CIE_Y_integral;
    ret.y /= CIE_Y_integral;
    ret.z /= CIE_Y_integral;

    return ret;
}

t_8bcolor XYZToRGB(t_fvec3 t)
{
    //printf("%f, %f, %f", t.values[0], t.values[1], t.values[2]);
    // return (Color){.a = 255,
    //                .r = fmin(linear_to_gamma(t.values[0]) * 255, 255),
    //                .g = fmin(linear_to_gamma(t.values[1]) * 255, 255),
    //                .b = fmin(linear_to_gamma(t.values[2]) * 255, 255)};
    // t_SampledWavelengths fixed_lambda = {
    //     .lambda = { 430.f, 500.f, 680.f, 700.f },
    //     .pdf    = { 0.3f, .1f, .25f, .4f }
    // };
    //#TODO: CHANGE O.5 FOR AUTOMATIC GENERATED U
    // float lu = random_generator();
    // //t_SampledWavelengths lambda = SampleUniform(lu, 360.f, 830.f);
    // //RANGE
    // t_SampledWavelengths lambda = SampleUniform(lu, 360, 830);
    //t_ColorRGB rgb = spectrum_to_rgb(t, lambda);

    //t_ColorRGB rgb = xyz_to_rgb(t);
    t_color rgb;
    rgb.x =  3.2406f * t.x - 1.5372f * t.y - 0.4986f * t.z;
    rgb.y = -0.9689f * t.x + 1.8758f * t.y + 0.0415f * t.z;
    rgb.z =  0.0557f * t.x - 0.2040f * t.y + 1.0570f * t.z;

    rgb = clamp_rgb(rgb);

    t_8bcolor out;
    out.a = 255;
    out.r = fmin(linear_to_gamma(rgb.x) * 255.0f, 255.0f);
    out.g = fmin(linear_to_gamma(rgb.y) * 255.0f, 255.0f);
    out.b = fmin(linear_to_gamma(rgb.z) * 255.0f, 255.0f);

    return out;
}

uint32_t conv_8bcolor_to_uint32(t_8bcolor c) {
	return ((uint32_t)c.r << 24 | (uint32_t)c.g << 16 | (uint32_t)c.b << 8 | c.a);
}
