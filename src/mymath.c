#include "mymath.h"
#include <math.h>
#include <raymath.h>

t_fvec3 fvec3_sub(t_fvec3 a, t_fvec3 b) {
    return (t_fvec3){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

float fvec3_dot(t_fvec3 a, t_fvec3 b) {
	// error probably < gamma5
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_fvec3 fvec3_norm(t_fvec3 v) {
    t_fvec3 result = v;

    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float ilength = 1.0f / length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }
    return result;
}

float fvec3_len_sq(t_fvec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

t_fvec3 fvec3_invert(t_fvec3 v) {
    return (t_fvec3){.x = -v.x, .y = -v.y, .z = -v.z};
}

t_fvec3 fvec3_add(t_fvec3 a, t_fvec3 b) {
    return (t_fvec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}
t_fvec3 fvec3_mul(t_fvec3 a, t_fvec3 b) {
    return (t_fvec3){
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
    };
}

t_fvec3 fvec3_scale(t_fvec3 v, float scalar) {
    return (t_fvec3){
        .x = v.x * scalar,
        .y = v.y * scalar,
        .z = v.z * scalar,
    };
}

t_fvec3 fvec3_reflect(t_fvec3 v, t_fvec3 norm) {
    return fvec3_sub(v, fvec3_scale(norm, 2 * fvec3_dot(norm, v)));
}

t_fvec3 fvec3_lerp(t_fvec3 v1, t_fvec3 v2, float prop) {
    return (t_fvec3){
        .x = v1.x * prop + v2.x * (1 - prop),
        .y = v1.y * prop + v2.y * (1 - prop),
        .z = v1.z * prop + v2.z * (1 - prop),
    };
}

t_fvec3 fvec3_cross(t_fvec3 v1, t_fvec3 v2) {
    t_fvec3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };

    return result;
}

t_fvec2 vec2_rotate(t_fvec2 v, float angle) {
	float s;
	float c;
	t_fvec2 ret;
	
	s = sin(angle);
	c = cos(angle);
	ret.x = v.x * c + v.y * s;
	ret.y = v.x * s - v.y * c;
	return (ret);
}

// z        
// | /y     
// |/       
// +---- x  

t_fvec3 vec3_rotate_yaw_pitch(t_fvec3 v, float yaw, float pitch)
{
	 // Vector3 ret = Vector3RotateByQuaternion(*(Vector3*)(void*)&v,
  //                                           QuaternionFromEuler(pitch, yaw, 0));
  //   return (*(t_fvec3*)(void*)&ret);
	t_fvec2 zy;
	t_fvec2 xy;

	xy.x = v.x;
	xy.y = v.y;
	xy = vec2_rotate(xy, yaw);
	v.x = xy.x;
	v.y = xy.y;

	zy.x = v.z;
	zy.y = v.y;
	zy = vec2_rotate(zy, pitch);

	v.z = zy.x;
	v.y = zy.y;
    return(v);
}

float fvec3_idx(t_fvec3 v, int idx)
{
	if (idx == 0)
		return v.x;
	if (idx == 1)
		return v.y;
	if (idx == 2)
		return v.z;
	return 0;
}
