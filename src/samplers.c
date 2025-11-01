#include "samplers.h"
#include "minirt.h"

#include <float.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

t_fvec3 rand_direction(uint64_t *rand_state)
{
    t_fvec3 ret;

    while (1) {
        ret.x = rand_float(rand_state) * 2. - 1.;
        ret.y = rand_float(rand_state) * 2. - 1.;
        ret.z = rand_float(rand_state) * 2. - 1.;
        float lensq = fvec3_len_sq(ret);
        if (lensq > FLT_EPSILON && lensq < 1)
            break;
    }
    return (fvec3_norm(ret));
}

t_fvec3 rand_halfsphere(t_fvec3 norm, uint64_t *rand_state) {
    t_fvec3 ret = rand_direction(rand_state);

    if (fvec3_dot(ret, norm) < 0) {
        ret = fvec3_invert(ret);
    }
    return (ret);
}

bool sample_stratified(t_sampler_state* state, t_fvec2* sample) {
    sample->x = (float)state->last_x / state->stratified_x;
    sample->y = (float)state->last_y / state->stratified_y;
    state->last_x++;
    if (state->last_x == state->stratified_x) {
        state->last_x = 0;
        state->last_y++;
    }
    if (state->last_y > state->stratified_y) {
        return false;
    }
    return true;
}
