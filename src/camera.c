#include "minirt.h"
#include "mymath.h"

t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample) {
    t_fvec3 dir = {
        .x = (px.x + sample.x - state->screen_width / 2.) * state->proj_coef,
        .y = state->screen_dist,
        .z = (px.y + sample.y - state->screen_height / 2.) * state->proj_coef};
    return fvec3_norm(dir);
}
