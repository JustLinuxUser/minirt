#include "minirt.h"
#include <math.h>
#include <raylib.h>

//DELETE
#include <stdio.h>

// TODO: Delete this, stop using Color
t_color RGBToColor(Color c) {
    return (t_color){.x = c.r / 255.0, .y = c.g / 255.0, .z = c.b / 255.0};
}

float linear_to_gamma(float c) {
    if (c < 0)
        return 0;
    return (sqrt(c));
}
Color ColortoRGB(t_color c) {
    return (Color){.a = 255,
                   .r = fmin(linear_to_gamma(c.x) * 255, 255),
                   .g = fmin(linear_to_gamma(c.y) * 255, 255),
                   .b = fmin(linear_to_gamma(c.z) * 255, 255)};
}

Color SpectrumToRGB(t_SampledSpectrum t)
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
    // t_SampledWavelengths lambda = SampleUniform(0.5f, 360.f, 830.f);
    //RANGE
    t_SampledWavelengths lambda = SampleUniform(0.125, 380, 780);
    t_ColorRGB rgb = spectrum_to_rgb(t, lambda);

    rgb = clamp_rgb(rgb);

    Color out;
    out.a = 255;
    out.r = fmin(linear_to_gamma(rgb.r) * 255.0f, 255.0f);
    out.g = fmin(linear_to_gamma(rgb.g) * 255.0f, 255.0f);
    out.b = fmin(linear_to_gamma(rgb.b) * 255.0f, 255.0f);

    return out;
}