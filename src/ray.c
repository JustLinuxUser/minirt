#include "cie.h"
#include "minirt.h"
#include "mymath.h"
#include "samplers.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "shapes.h"

//DELETE
#include <stdio.h>
#include <string.h>

t_collision ray_collision(t_state* state, t_ray curr_ray, float t_max) {
	t_collision curr_coll = {.collided = false};

    for (size_t obj_idx = 0; obj_idx < state->shapes.len; obj_idx++) {
		t_shape shape = state->shapes.buff[obj_idx];
		t_collision coll = collide_shape(state, shape, curr_ray, t_max);

		if (coll.collided) {
            if (!curr_coll.collided || coll.t < curr_coll.t) {
				curr_coll = coll;
				if (isfinite(t_max) && curr_coll.t < t_max)
					break;
			}
		}
	}
	return curr_coll;
}

//EASY GET SURFACE COLOR AS BSDF
t_SampledSpectrum get_surface_color(t_state* state, t_fvec3 p)
{
    t_SampledSpectrum color;
    int i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
       color.values[i] = 1.0f; 
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
	t_collision coll;
	t_collision light_coll;
    int i;
    t_fvec3 p;
    t_fvec3 norm = {0};

    //INIT BETA
    i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
        beta.values[i] = 1.f;
     

    while (iters_left-- > 0)
    {
        // coll = ray_collision(state, ray, INFINITY);
		int steps = 0;
		coll = collide_bvh(state, ray, INFINITY, &steps);
		// coll = ray_collision(state, ray, INFINITY);
		// if (rand() % 50 == 0) {
		// 	t_collision coll2 = ray_collision(state, ray, INFINITY);
		// 	if (coll.shape.ptr != coll2.shape.ptr) {
		// 		L.values[0] = 1000;
		// 		L.values[1] = 1000;
		// 		L.values[2] = 1000;
		// 		L.values[3] = 1000;
		// 	}
		// }
		i = -1;
		while (++i < NUM_SPECTRUM_SAMPLES)
		{
			// L.values[i] += steps * 0.001;
		}
        if (!coll.collided)
		{
            break;
		}
		// if (coll.shape.ptr != coll2.shape.ptr)
		// 	printf("Different shapes!!!!!!!");
		// if (coll2.collided)
		// 	printf("Something\n");
        //TO BE USED WHEN BSDF IMPLEMENTED:
        // obj o = state->objs[state->last_collided_idx];

        //HIT POS AND NORMAL TO HIT
        p = fvec3_add(fvec3_scale(ray.dir, coll.t), ray.pos);

		norm = collision_norm(state, coll, p);
		if (fvec3_dot(norm, ray.dir) > 0)
			norm = fvec3_invert(norm);
        /*LIGHT THING*/

        //LIGHT INFORMATION
        t_fvec3 light = fvec3_sub(state->light_pos, p);
        float light_t = sqrtf(fvec3_len_sq(light));
        t_fvec3 norm_light = fvec3_norm(light);
        
        //SHADOW RAY
        t_SampledSpectrum color = {0};
        color = get_surface_color(state, p);
		if (coll.shape.type == OBJ_SPHERE)
			color.values[0] = 0.5;
        float dot = 0.f;
        float distance_decrease = 0.f;

		// light_coll = ray_collision(state, (t_ray){.pos = p, .dir = norm_light}, light_t);
		light_coll = collide_bvh(state, (t_ray){.pos = p, .dir = norm_light}, light_t, &steps);
        if (!light_coll.collided)
        {
			t_SampledSpectrum light = {.values = {
				0.87, 0.69, 0.52, 0.99
			}};
            //GET COSINE (AS BEFORE)
            dot = fmax(fvec3_dot(norm, norm_light), 0);
            //HOW MUCH LIGHT INTENSITY (cos AND distance)
            distance_decrease = 1.0f / (light_t * light_t);
            //APPLY BSDF
			i = -1;
			while (++i < NUM_SPECTRUM_SAMPLES)
			{
				//printf("Update L[%d]: %f, %f, %f, %f\n", i, beta.values[i], color.values[i],  dot, distance_decrease);
				L.values[i] += beta.values[i] * color.values[i] * dot * (5000.f * light.values[i] * distance_decrease);
			}
		}
        // }        

        /*LAMBERTIAN SURFACE (BASE CASE)*/
        t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm)};
        ray = new_ray;
        // ray.pos = p;
        // ray.dir = rand_halfsphere(norm);

        dot = fmax(fvec3_dot(norm, new_ray.dir), 0);

        //UPDATE BETA NOT CORRECT AT THE MOMENT
        i = -1;
        t_SampledWavelengths wv = SampleUniform(0.5, CIE_MIN_LAMBDA, CIE_MAX_LAMBDA);
        while (++i < NUM_SPECTRUM_SAMPLES)
        {
            if (wv.pdf[i] == 0.f)
                beta.values[i] = 0.f;
            else
                beta.values[i] *= color.values[i] * dot ;/// wv.pdf[i];
			if (beta.values[i] > 1.1) {
				sampled_spectrum_scale(L, 0);
				L.values[0] = 1.;
			}
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
