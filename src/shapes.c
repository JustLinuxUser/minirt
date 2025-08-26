#include "shapes.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "bounds.h"
#include "libft/utils/utils.h"
#include "minirt.h"

bool intersect_sphere(t_sphere s, t_ray r, float* t, bool* inside) {
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
    *inside = t1 > 0 && t2 > 0;
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

bool intersect_triangle(t_ray ray,
                        t_fvec3 a,
                        t_fvec3 b,
                        t_fvec3 c,
                        t_triangle_collision* coll) {
    float epsilon = FLT_EPSILON;  // std::numeric_limits<float>::epsilon();

    t_fvec3 edge1 = fvec3_sub(b, a);
    t_fvec3 edge2 = fvec3_sub(c, a);
    t_fvec3 ray_cross_e2 = fvec3_cross(ray.dir, edge2);
    float det = fvec3_dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;  // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    t_fvec3 s = fvec3_sub(ray.pos, a);
    coll->u = inv_det * fvec3_dot(s, ray_cross_e2);

    if ((coll->u < 0 && fabs(coll->u) > epsilon) ||
        (coll->u > 1 && fabs(coll->u - 1) > epsilon))
        return false;

    t_fvec3 s_cross_e1 = fvec3_cross(s, edge1);
    coll->v = inv_det * fvec3_dot(ray.dir, s_cross_e1);

    if ((coll->v < 0 && fabs(coll->v) > epsilon) ||
        (coll->u + coll->v > 1 && fabs(coll->u + coll->v - 1) > epsilon))
        return false;

    // At this stage we can compute t to find out where the intersection point
    // is on the line.
    coll->t = inv_det * fvec3_dot(edge2, s_cross_e1) * 0.99;

    return (coll->t > epsilon);  // ray intersection
}

t_triangle_pts triangle_points(t_state* state, t_triangle triangle) {
    t_mesh* mesh = &state->meshes.buff[triangle.mesh_idx];
    int* vidxs = &mesh->vertex_idxs.buff[triangle.triangle_idx * 3];

    t_triangle_pts ret;
    ret.a = mesh->vertices.buff[vidxs[0]];
    ret.b = mesh->vertices.buff[vidxs[1]];
    ret.c = mesh->vertices.buff[vidxs[2]];
    return (ret);
}

t_collision collide_shape(t_state* state,
                          t_shape shape,
                          t_ray ray,
                          float t_max) {
    t_collision ret = {.shape = shape};
    float t;

    if (shape.type == OBJ_TRIANGLE) {
        t_triangle_pts pts = triangle_points(state, *(t_triangle*)shape.ptr);

        t_triangle_collision coll;
        if (intersect_triangle(ray, pts.b, pts.c, pts.a, &coll) &&
            coll.t < t_max) {
            ret.collided = true;
            ret.t = coll.t;
            ret.u = coll.u;
            ret.v = coll.v;
        }
        return ret;
    } else if (shape.type == OBJ_SPHERE) {
        t_sphere* sphere = (t_sphere*)shape.ptr;
        bool inside;
        if (intersect_sphere(*sphere, ray, &t, &inside) && t < t_max) {
            ret.collided = true;
            ret.t = t;
            ret.u = inside;
        }
        return ret;
    }

    ft_assert("Unreachable" != 0);
    return ret;
}

t_fvec3 collision_norm(t_state* state, t_collision collision, t_fvec3 pos) {
    if (collision.shape.type == OBJ_TRIANGLE) {
        t_triangle_pts pts =
            triangle_points(state, *(t_triangle*)collision.shape.ptr);

        // return (fvec3_norm(fvec3_cross(fvec3_sub(a, c), fvec3_sub(a, b))));
        return (fvec3_norm(
            fvec3_cross(fvec3_sub(pts.a, pts.b), fvec3_sub(pts.a, pts.c))));
    } else if (collision.shape.type == OBJ_SPHERE) {
        t_sphere* sphere = (t_sphere*)collision.shape.ptr;
        if (collision.u == 0)
            return (fvec3_invert(fvec3_norm(fvec3_sub(pos, sphere->p))));
        return (fvec3_norm(fvec3_sub(pos, sphere->p)));
    }
    ft_assert("Unreachable" != 0);
    return (t_fvec3){0};
}

t_bounds3f shape_bounds(t_state* state, t_shape shape) {
    t_bounds3f bounds = {0};
    if (shape.type == OBJ_TRIANGLE) {
        t_triangle_pts pts = triangle_points(state, *(t_triangle*)shape.ptr);
        bounds.min = pts.a;
        bounds.max = pts.a;
        bounds = bounds_extend_pt(bounds, pts.b);
        bounds = bounds_extend_pt(bounds, pts.c);
    } else if (shape.type == OBJ_SPHERE) {
        t_sphere sp = *(t_sphere*)shape.ptr;
        bounds.min =
            fvec3_sub(sp.p, (t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
        bounds.max =
            fvec3_add(sp.p, (t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
    } else {
        ft_assert("TODO" == 0);
    }
    return (bounds);
}
