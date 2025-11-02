#include "cie.h"
#include "minirt.h"
#include "samplers.h"
#include "shapes.h"
#include "spectrum.h"
#include <math.h>
#include <stdlib.h>
//
t_collision collide_ray_slow(t_state* state, t_ray_isector isect) {
	t_collision curr_coll = {.collided = false};

    for (size_t obj_idx = 0; obj_idx < state->shapes.len; obj_idx++) {
		if (&state->shapes.buff[obj_idx].ptr == isect.ignore_shape)
			continue;
		t_shape shape = state->shapes.buff[obj_idx];
		t_collision coll = collide_shape(state, shape, isect);

		if (coll.collided) {
            if (!curr_coll.collided || coll.t < curr_coll.t) {
				curr_coll = coll;
				if (isfinite(isect.t_max) && curr_coll.t < isect.t_max)
					break;
			}
		}
	}
	return curr_coll;
}

t_SampledSpectrum sample_densely_sampled_spectrum(const t_densely_sampled_spectrum *spec, t_SampledWavelengths lambdas) {
	int	i;
	t_SampledSpectrum	ret;

	i = -1;
	ret = (t_SampledSpectrum){0};
	while (++i < NUM_SPECTRUM_SAMPLES)
	{
		ret.values[i] = spec->samples[(int)(lambdas.lambda[i] - CIE_MIN_LAMBDA)];
	}
	return ret;
}

//EASY GET SURFACE COLOR AS BSDF
t_SampledSpectrum get_surface_color(t_state* state, t_SampledWavelengths lambdas, t_collision coll)
{
	return sample_densely_sampled_spectrum(shape_spectrum(state, coll), lambdas);
}

t_SampledSpectrum sample_sky(t_state *state, t_SampledWavelengths lambdas) {
	t_SampledSpectrum	spec;
	t_light l;
	spec = (t_SampledSpectrum){0};

	if (state->sky_light_idx != -1)
	{
		l = state->lights.lights.buff[state->sky_light_idx];
		spec = sample_densely_sampled_spectrum(state->spectrums.buff + l.spec_idx, lambdas);
		spec = sampled_spectrum_scale(spec, l.intensity / 10);
	}
	return (spec);
}

t_light sample_effective_light(t_state *state, t_ray_isector *isector, uint64_t *rand_state, t_fvec3 norm)
{
	t_light light;
	t_collision light_coll;
	int light_idx;


	light = (t_light){0};
	light_idx = sample_alias_table(&state->lights, rand_float(rand_state));
	if (light_idx == -1)
		return light;
	light = state->lights.lights.buff[light_idx]; 

	float light_t = INFINITY;

	t_fvec3 light_dir;

	light.intensity = light.intensity / state->lights.pdfs.buff[light_idx];
	if (light.t == POINT_LIGHT) {
		light_dir = fvec3_sub(light.position, isector->ray.pos);
		light_t = sqrtf(fvec3_len_sq(light_dir));
		light.intensity /= light_t * light_t;
	} else if (light.t == DISTANT_LIGHT) {
		light.intensity /= 10.0;
		light_dir = light.position;
	} else if (light.t == AMBIANT_LIGHT) {
		light.intensity /= 10.0;
		light.intensity /= 2.0; // because a halfsphere
		light_dir = rand_halfsphere(norm, rand_state);
	} else {
		ft_assert("Unreachable" == 0);
		return light.t = LIGHT_NONE, light;
	}

	isector->ray.dir = fvec3_norm(light_dir);
	isector->t_max = light_t;

	light_coll = collide_bvh(state, *isector);
	if (light_coll.collided)
		light.t = LIGHT_NONE;
	return light;
}

t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, 
										   t_SampledWavelengths lambdas, int iters_left, uint64_t *rand_state) {
    t_SampledSpectrum L = {0};
    t_SampledSpectrum beta = {1.f};
	t_collision coll;
	t_collision light_coll;
    int i;
    int iter = -1;
    t_fvec3 p;
    t_fvec3 norm = {0};
	void *ignored_shape = 0;

    //INIT BETA
    i = -1;
    while (++i < NUM_SPECTRUM_SAMPLES)
        beta.values[i] = 1.f;
     

    while (iters_left-- > 0)
    {
		iter++;
        coll = collide_bvh(state, (t_ray_isector){.ray = ray, .t_max = INFINITY, .t_min = 0.01, .ignore_shape = ignored_shape});
		// if (rand() % 100 == 0) {
		// 	coll2 = collide_ray_slow(state, (t_ray_isector){.ray = ray, .t_max = INFINITY, .t_min = 0.01, .ignore_shape = ignored_shape});
		// 	if (coll.collided != coll2.collided || coll2.shape.ptr != coll.shape.ptr) {
		// 		L = sampled_spectrum_add(L, sampled_spectrum_scale(sample_densely_sampled_spectrum(&SPECTRUM_ONES, lambdas), 100));
		// 	}
		// }

	
		//APPLY BSDF
        if (!coll.collided) {
			if (iter == 0) {
				t_SampledSpectrum sky_spec = sample_sky(state, lambdas);
				i = -1;
				while (++i < NUM_SPECTRUM_SAMPLES)
					L.values[i] += beta.values[i] * sky_spec.values[i];
			}
			break;
		}

		ignored_shape = coll.shape.ptr;
        //TO BE USED WHEN BSDF IMPLEMENTED:

        //HIT POS AND NORMAL TO HIT
        p = fvec3_add(fvec3_scale(ray.dir, coll.t), ray.pos);
		norm = collision_norm(state, coll, p);
		if (fvec3_dot(norm, ray.dir) > 0)
			norm = fvec3_invert(norm);
        
        /*LIGHT THING*/

        //LIGHT INFORMATION
        float lu = rand_float(rand_state);

		t_SampledSpectrum color = {0};
		color = get_surface_color(state, lambdas, coll);

		//SHADOW RAY
		t_SampledSpectrum light_spec = {0};
		t_ray_isector isector = {.ray = {.pos = p}, .t_min = 0.01, .ignore_shape = ignored_shape};
        t_light light = sample_effective_light(state, &isector, rand_state, norm);

		if (light.t != LIGHT_NONE) {
			float dot = fmax(fvec3_dot(norm, isector.ray.dir), 0);
			light_spec = sampled_spectrum_scale(sample_densely_sampled_spectrum(&state->spectrums.buff[light.spec_idx], lambdas), light.intensity * dot);
		}

		t_SampledSpectrum ambiant_spec = sample_densely_sampled_spectrum(&state->ambiant_light_spec, lambdas);
	
		//APPLY BSDF
		i = -1;
		while (++i < NUM_SPECTRUM_SAMPLES)
		{
			L.values[i] += beta.values[i] * color.values[i] * (light_spec.values[i] + ambiant_spec.values[i] * fabs(fvec3_dot(norm, ray.dir)));
		}

        /*LAMBERTIAN SURFACE (BASE CASE)*/
        t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm, rand_state)};
        ray = new_ray;
        // ray.pos = p;
        // ray.dir = rand_halfsphere(norm);

        float dot = fmax(fvec3_dot(norm, new_ray.dir), 0);

        //UPDATE BETA NOT CORRECT AT THE MOMENT
        i = -1;
		// TODO: figure out the PDF
        while (++i < NUM_SPECTRUM_SAMPLES)
        {
            if (lambdas.pdf[i] == 0.f)
                beta.values[i] = 0.f;
            else
                beta.values[i] *= color.values[i] * dot;
			// / lambdas.pdf[i];
        }
    }
    return L;
}
