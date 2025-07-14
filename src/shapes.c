#include "minirt.h"
#include <math.h>
#include <stdbool.h>

bool intersect_sphere(sphere s, t_ray r, float* t) {
    t_fvec3 L = fvec3_sub(s.p, r.pos);

    // if tc is less then 0, we are looking away from the center (mb we are
    // inside the sphere)
    float tc = fvec3_dot(L, fvec3_norm(r.dir));
    // printf("tc: %f\n", tc);
    // if (tc < 0.0) {
    //     return false;
    // }

    // TODO: Optimize the sqrt
    float d = fvec3_dot(L, L) - tc * tc;
    if (d > s.r * s.r || d < 0) {
        return (false);
    }
    float tlc = sqrt(s.r * s.r - d);
    float t1 = tc + tlc;
    float t2 = tc - tlc;

    if ((t1 < t2 && t1 > 0) || t2 < 0)
        *t = t1;
    else
        *t = t2;
    // *p = fvec3_add(Vector3Scale(r.direction, closest_t), r.position);
    // return (true);
    *t *= 0.99;
    return (*t > 0.1);
}

int intersect_plane(t_ray plane, t_ray r, float* t) {
    float denom = fvec3_dot(r.dir, plane.dir);
    // close enough to parallel, to be considered paralell
    if (fabs(denom) < 1e-7) {
        return false;
    }
    *t = fvec3_dot(fvec3_sub(plane.pos, r.pos), plane.dir) / denom;
    if (*t < 1e-6)
        return false;
    return (true);
}
