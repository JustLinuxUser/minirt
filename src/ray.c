#include "minirt.h"
#include "samplers.h"
#include <math.h>
#include <stdlib.h>

float cast_ray(t_state* state, t_ray curr_ray, float t_cutoff) {
    float t;

    int curr_collided_idx = -1;
    float curr_t = INFINITY;

    for (int obj_idx = 0; obj_idx < state->obj_count; obj_idx++) {
        obj o = state->objs[obj_idx];

        if (/* obj_idx == state->last_collided_idx || */ (t_cutoff && o.skip))
            continue;

        if (o.type == OBJ_SPHERE &&
            intersect_sphere(o.obj.sphere, curr_ray, &t)) {
            if (t < curr_t) {
                curr_t = t;
                curr_collided_idx = obj_idx;
            }
        }
        if (o.type == OBJ_PLANE && intersect_plane(o.obj.plane, curr_ray, &t)) {
            if (t < curr_t) {
                curr_t = t * 0.999;
                curr_collided_idx = obj_idx;
            }
        }
        if (t_cutoff != 0 && curr_t < t_cutoff)
            break;
    }
    if (t_cutoff != 0) {
        if (curr_t < t_cutoff) {
            return (0);
        } else {
            return (curr_t);
        }
    } else if (curr_collided_idx != -1) {
        state->last_collided_idx = curr_collided_idx;
        return (curr_t);
    }
    return (0);
}

t_color cast_reflectable_ray(t_state* state, t_ray ray, int iters_left) {
    float t;
    t_color ret = {};
    t_color obj_color;
    t_fvec3 p;

    t = cast_ray(state, ray, 0);

    if (t != 0) {
        obj o = state->objs[state->last_collided_idx];
        t_fvec3 norm;

        if (o.type == OBJ_SPHERE) {
            p = fvec3_add(fvec3_scale(ray.dir, t), ray.pos);
            if ((abs((int)p.x / 4) % 2 == 0) ^ (p.x > 0) ^ (p.z > 0) ^
                (abs((int)p.z / 4) % 2 == 0))
                obj_color = o.obj.sphere.color;
            else
                obj_color = DARKDARKGRAY;
            norm = fvec3_norm(fvec3_sub(p, o.obj.sphere.p));
        }
        if (o.type == OBJ_PLANE) {
            p = fvec3_add(fvec3_scale(ray.dir, t), ray.pos);
            if ((abs((int)p.x / 10) % 2 == 0) ^ (p.x > 0) ^ (p.z > 0) ^
                (abs((int)p.z / 10) % 2 == 0))
                obj_color = DARKDARKGRAY;
            else
                obj_color = RGBToColor(WHITE);
            norm = o.obj.plane.dir;
        }
        t_fvec3 light = fvec3_sub(state->light_pos, p);
        float light_t = sqrtf(fvec3_len_sq(light));
        t_fvec3 norm_light = fvec3_norm(light);
        float brightness = 0;

        state->debug = 0;
        float got_t =
            cast_ray(state, (t_ray){.pos = p, .dir = norm_light}, light_t);
        if (got_t) {
            float dot = fmax(fvec3_dot(norm, norm_light), 0);
            brightness = ((15000 * dot) / (light_t * light_t));
        }
        ret = fvec3_add(fvec3_scale(obj_color, brightness), ret);
        if (iters_left > 0) {
            t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm)};
            if (state->last_collided_idx == 1) {
                new_ray.dir = fvec3_norm(fvec3_reflect(ray.dir, norm));
                // return (t_color){.x=1};
            }
            t_color ray_color =
                cast_reflectable_ray(state, new_ray, iters_left - 1);

            t_color ray_color_scaled =
                fvec3_scale(ray_color, fmax(fvec3_dot(new_ray.dir, norm), 0));
            ret = fvec3_add(fvec3_mul(ray_color_scaled, obj_color), ret);
        }
        return (ret);
    } else {
        return (t_color){};
    }
}
