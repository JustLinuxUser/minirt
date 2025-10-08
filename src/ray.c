#include "cie.h"
#include "minirt.h"
#include "samplers.h"
#include "shapes.h"
#include <math.h>
#include <stdlib.h>
//
// t_collision ray_collision(t_state* state, t_ray curr_ray, float t_max, void *ignore_shape) {
// 	t_collision curr_coll = {.collided = false};
//
//     for (size_t obj_idx = 0; obj_idx < state->shapes.len; obj_idx++) {
// 		if (&state->shapes.buff[obj_idx].ptr == ignore_shape)
// 			continue;
// 		t_shape shape = state->shapes.buff[obj_idx];
// 		t_collision coll = collide_shape(state, shape, curr_ray, t_max);
//
// 		if (coll.collided) {
//             if (!curr_coll.collided || coll.t < curr_coll.t) {
// 				curr_coll = coll;
// 				if (isfinite(t_max) && curr_coll.t < t_max)
// 					break;
// 			}
// 		}
// 	}
// 	return curr_coll;
// }

/*NEW CAST REFLECTABLE RAY!!!*/
//#TODO

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

t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, 
										   t_SampledWavelengths lambdas, int iters_left) {
    t_SampledSpectrum L = {0};
    t_SampledSpectrum beta = {1.f};
	t_collision coll;
    int i;
    t_fvec3 p;
    t_fvec3 norm = {0};
	void *ignored_shape = 0;

    //INIT BETA
    i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
        beta.values[i] = 1.f;
     

    while (iters_left-- > 0)
    {
        coll = collide_bvh(state, (t_ray_isector){.ray = ray, .t_max = INFINITY, .t_min = 0.001, .ignore_shape = ignored_shape});
        if (!coll.collided)
            break;
		ignored_shape = coll.shape.ptr;
        //TO BE USED WHEN BSDF IMPLEMENTED:

        //HIT POS AND NORMAL TO HIT
        p = fvec3_add(fvec3_scale(ray.dir, coll.t), ray.pos);
		norm = collision_norm(state, coll, p);
		if (fvec3_dot(norm, ray.dir) > 0)
			norm = fvec3_invert(norm);
        
        /*LIGHT THING*/

        //LIGHT INFORMATION
        float lu = random_generator();
        t_light s_light;
        int index = SampleAliasTable(&state->lights, lu);
        s_light = state->lights.lights[index]; 
        t_fvec3 light = fvec3_sub(s_light.position, p);
        float light_t = sqrtf(fvec3_len_sq(light));
        t_fvec3 norm_light = fvec3_norm(light);
        
        //SHADOW RAY
        t_SampledSpectrum color = {0};
        color = get_surface_color(state, p);
        float dot = 0.f;
        float distance_decrease = 0.f;

		t_ray light_ray = (t_ray){.pos = p, .dir = norm_light};
        coll = collide_bvh(state, (t_ray_isector){.ray = light_ray, .t_max = light_t, .t_min = 0.001, .ignore_shape = ignored_shape});
        if (!coll.collided)
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
                //#TODO: CHANGE FOR LIGHT.VALUES[i]
                L.values[i] += beta.values[i] * color.values[i] * dot * (s_light.intensity * s_light.spec.samples[(int)(lambdas.lambda[i] - CIE_MIN_LAMBDA)] * distance_decrease);
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
        //t_SampledWavelengths wv = get_values_pdf(state, p);
        while (++i < NUM_SPECTRUM_SAMPLES)
        {
            if (lambdas.pdf[i] == 0.f)
                beta.values[i] = 0.f;
            else
                beta.values[i] *= color.values[i] * dot;// / lambdas.pdf[i];
        }
    }
    return L;
}
