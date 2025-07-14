#include "minirt.h"
#include "mymath.h"

t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample) {
    t_fvec3 dir = {
        .x = (px.x + sample.x - state->screen_width / 2.) * state->proj_coef,
        .y = (px.y + sample.y - state->screen_height / 2.) * state->proj_coef,
        .z = state->screen_dist};
    return fvec3_norm(dir);
}
