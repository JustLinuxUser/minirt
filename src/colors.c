#include "minirt.h"
#include <math.h>
#include <raylib.h>

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

