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
    return (t_8bcolor){
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

t_8bcolor xyz_to_rgb(t_fvec3 t)
{
    t_color rgb;
    rgb.x =  3.2406f * t.x - 1.5372f * t.y - 0.4986f * t.z;
    rgb.y = -0.9689f * t.x + 1.8758f * t.y + 0.0415f * t.z;
    rgb.z =  0.0557f * t.x - 0.2040f * t.y + 1.0570f * t.z;

    rgb = clamp_rgb(rgb);

    t_8bcolor out;
    out.r = fmin(linear_to_gamma(rgb.x) * 255.0f, 255.0f);
    out.g = fmin(linear_to_gamma(rgb.y) * 255.0f, 255.0f);
    out.b = fmin(linear_to_gamma(rgb.z) * 255.0f, 255.0f);

    return out;
}

t_fvec3 rgb_to_xyz(t_8bcolor c)
{
	float r = c.r / 255.f;
	float g = c.g / 255.f;
	float b = c.b / 255.f;

	if (r < 0.04045) r /= 12.92;
	else r = powf((r + 0.055)/1.055, 2.4);

	if (g < 0.04045) g /= 12.92;
	else g = powf((g + 0.055)/1.055, 2.4);

	if (b < 0.04045) b /= 12.92;
	else b = powf((b + 0.055)/1.055, 2.4);

    t_color xyz;
	xyz.x =0.4124564 * r + 0.3575761 * g + 0.1804375* b;
	xyz.y =0.2126729 * r + 0.7151522 * g + 0.0721750* b;
	xyz.z =0.0193339 * r + 0.1191920 * g + 0.9503041* b;
	return xyz;
}

uint32_t conv_8bcolor_to_uint32(t_8bcolor c) {
	return ((uint32_t)c.r << 24 | (uint32_t)c.g << 16 | (uint32_t)c.b << 8 | 0xff);
}
