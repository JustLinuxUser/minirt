#include "minirt.h"
#include "samplers.h"
#include <math.h>
#include <stdlib.h>

//DELETE
#include <stdio.h>

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

/*NEW CAST REFLECTABLE RAY!!!*/

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

/*NEW CAST REFLECTABLE RAY!!!*/
//#TODO
typedef struct t_RGBSigmoidalPolyn {
    float c0, c1, c2;
} t_RGBSigmoidalPolyn;

//EASY GET SURFACE COLOR AS BSDF
t_SampledSpectrum get_surface_color(t_state* state, t_fvec3 p)
{
    /*REVERSE PROCESS -> MATRIX*/
    p.x = 95.f;
    p.y = 100.f;
    p.z = 100.f;

    t_SampledSpectrum color;
    int i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
       //color.values[i] = 0.5f * p.x + 0.5 * p.y; 
       color.values[i] = 0.9f;

    return color;
}

t_SampledWavelengths get_values_pdf(t_state* state, t_fvec3 p)
{
    t_SampledWavelengths l;
    int i = -1;
    while(++i < NUM_SPECTRUM_SAMPLES)
    {
        l.lambda[i] = 0.8f;
        l.pdf[i] = 1.0f;
    }
    return l; 
}

t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, int iters_left) {
    t_SampledSpectrum L = {0};
    t_SampledSpectrum beta = {1.f};
    float t;
    int i;
    t_fvec3 p;
    t_fvec3 norm = {0};

    //INIT BETA
    i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
        beta.values[i] = 1.f;
     

    iters_left = 6;
    while (iters_left-- > 0)
    {
        t = cast_ray(state, ray, 0);
        if (!t)
            break;
        //TO BE USED WHEN BSDF IMPLEMENTED:
        obj o = state->objs[state->last_collided_idx];

        //HIT POS AND NORMAL TO HIT
        p = fvec3_add(fvec3_scale(ray.dir, t), ray.pos);
        if (o.type == OBJ_SPHERE)
            norm = fvec3_norm(fvec3_sub(p, o.obj.sphere.p));
        else if (o.type == OBJ_PLANE)
            norm = o.obj.plane.dir;
        
        /*LIGHT THING*/

        //LIGHT INFORMATION
        t_fvec3 light = fvec3_sub(state->light_pos, p);
        float light_t = sqrtf(fvec3_len_sq(light));
        t_fvec3 norm_light = fvec3_norm(light);
        
        //SHADOW RAY
        t_SampledSpectrum color = {0};
        color = get_surface_color(state, p);
        float dot = 0.f;
        float distance_decrease = 0.f;
        float got_t =
            cast_ray(state, (t_ray){.pos = p, .dir = norm_light}, light_t);
        if (got_t)
        {
            //GET COSINE (AS BEFORE)
            dot = fmax(fvec3_dot(norm, norm_light), 0);
            //HOW MUCH LIGHT INTENSITY (cos AND distance)
            distance_decrease = 1.0f / (light_t * light_t);
            //APPLY BSDF
            i = -1;
            while (++i < NUM_SPECTRUM_SAMPLES)
            {
                //printf("Update L[%d]: %f, %f, %f, %f\n", i, beta.values[i], color.values[i],  dot, distance_decrease);
                L.values[i] += beta.values[i] * color.values[i] * color.values[i] * dot * (5000.f * distance_decrease);
            }
        }
        
        /*LAMBERTIAN SURFACE (BASE CASE)*/
        t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm)};
        ray = new_ray;
        // ray.pos = p;
        // ray.dir = rand_halfsphere(norm);

        dot = fmax(fvec3_dot(norm, new_ray.dir), 0);

        //UPDATE BETA NOT CORRECT AT THE MOMENT
        i = -1;
        t_SampledWavelengths wv = get_values_pdf(state, p);
        while (++i < NUM_SPECTRUM_SAMPLES)
        {
            if (wv.pdf[i] == 0.f)
                beta.values[i] = 0.f;
            else
                beta.values[i] *= color.values[i] * dot / wv.pdf[i];
        }
    }
    //printf("Final L: %f, %f, %f\n", L.values[0], L.values[1], L.values[2]);
    return L;
    // float t;
    // t_fvec3 p;
    // t_SampledSpectrum ret;

    // t = cast_ray(state, ray, 0);
    // if (!t)
    // {
    //     //check collision in infinity
    //     // return smthin else
    //     return (t_SampledSpectrum){};
    // }
    // else
    // {
    //     obj o = state->objs[state->last_collided_idx];
        
    //     //emissive surfaces
        
    //     /*WHAT IS ITERS FOR?*/
    //     iters_left--;
    //     if (iters_left <= 0)
    //         return ;
        
    //     ///BSDF and skip over medium boundaries

    //     /// sample illumination

    //     /// outgoing dir at intersection to continue path
    //     int sampleBSDF = 1;
    //     if (sampleBSDF)
    //     {
    //         /// sample BSDF for new dir
    //     }
    //     else
    //     {
    //         /// Uniformly sample
    //         float pdf;
    //         t_fvec3 p;
    //         /// var for bsdf flags
    //         if (!pdf) //Wrong. Need some flags
    //         {
    //             //Reflexive and transmissive
    //         }
    //         else 
    //         {
    //             //Else
    //         }

    //     }
    // }
    // return ret;
}