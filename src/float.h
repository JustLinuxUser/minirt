
#ifndef FLOAT_H
# define FLOAT_H
#include <math.h>
#define MACHINE_EPSILON (0x1p-24)

inline static float f32_gamma(int n) {
    return (n * MACHINE_EPSILON) / (1 - n * MACHINE_EPSILON);
}
#endif
