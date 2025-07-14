#pragma once

#include "mymath.h"
#include <stdbool.h>
typedef struct sampler_state {
    int stratified_x;
    int stratified_y;

    int last_x;
    int last_y;
} t_sampler_state;
bool sample_stratified(t_sampler_state* state, t_fvec2* sample);
t_fvec3 rand_halfsphere(t_fvec3 norm);
