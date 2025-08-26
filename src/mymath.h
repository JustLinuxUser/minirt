#ifndef MYMATH_H
# define MYMATH_H

typedef struct fvec2 {
	float x;
	float y;
} t_fvec2;

typedef struct t_fvec3 {
	float x;
	float y;
	float z;
} t_fvec3;

typedef struct t_ray {
	t_fvec3 dir;
	t_fvec3 pos;
} t_ray;

t_fvec3 fvec3_sub(t_fvec3 a, t_fvec3 b);
t_fvec3 fvec3_add(t_fvec3 a, t_fvec3 b);
t_fvec3 fvec3_mul(t_fvec3 a, t_fvec3 b);
float fvec3_dot(t_fvec3 a, t_fvec3 b);

t_fvec3 fvec3_norm(t_fvec3 v);
float fvec3_len_sq(t_fvec3 v);
t_fvec3 fvec3_invert(t_fvec3 v);
t_fvec3 fvec3_scale(t_fvec3 v, float scalar);
t_fvec3 fvec3_reflect(t_fvec3 v, t_fvec3 norm);
t_fvec3 fvec3_lerp(t_fvec3 v1, t_fvec3 v2, float prop);
t_fvec3 vec3_rotate_pitch_yaw(t_fvec3 v, float pitch, float yaw);
t_fvec3 fvec3_cross(t_fvec3 v1, t_fvec3 v2);
float fvec3_idx(t_fvec3 v, int idx);
#endif
