#include "bvh.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "bounds.h"
#include "float.h"
#include "libft/libft.h"
#include "minirt.h"
#include "shapes.h"

typedef struct s_bvh_node {
    size_t shape_idx;
    t_bounds3f bounds;
} t_bvh_primitive;

typedef struct s_bvh_build_node {
    t_bounds3f bounds;
    struct s_bvh_build_node* children[2];
    int first_prim_offset;
    uint16_t n_primitives;
    uint8_t split_axis;
} t_bvh_build_node;

typedef struct bvh_build_state {
    t_state state;
    t_vec_shape* ordered_shapes;
    int total_nodes;
} t_bvh_build_state;

bool bounds_is_encapsulated(t_bounds3f big, t_bounds3f small) {
    if (big.min.x > small.min.x || big.min.y > small.min.y ||
        big.min.z > small.min.z ||

        big.max.x < small.max.x || big.max.y < small.max.y ||
        big.max.z < small.max.z)
        return (false);
    return (true);
}
bool verify_tree(t_vec_shape* ordered_shapes,
                 t_state* state,
                 t_bvh_build_node* tree);

void bvh_create_leaf_node(t_bvh_build_state* state,
                          t_bvh_primitive* bvh_primitives,
                          size_t bvh_prim_len,
                          t_bvh_build_node* ret) {
    if (bvh_prim_len != 1)
        printf("bvh_len: %zu\n", bvh_prim_len);

    ret->first_prim_offset = state->ordered_shapes->len;
	if (state->ordered_shapes->len + 1> UINT16_MAX) {
		printf("Int overflow, got: %zu\n", state->ordered_shapes->len +1);
	}
    for (size_t i = 0; i < bvh_prim_len; i++) {

        t_shape shape = state->state.shapes.buff[bvh_primitives[i].shape_idx];

        t_bounds3f bounds = shape_bounds(&state->state, shape);
        if (!bounds_is_encapsulated(ret->bounds, bounds))
            printf("Failing in bvh leaf node\n");

		if (ft_memcmp(&bounds, &bvh_primitives[i].bounds, sizeof(bounds))) {
            printf("Got different bounds\n");
		}
        vec_shape_push(state->ordered_shapes, shape);
    }
    ret->n_primitives = bvh_prim_len;
    state->total_nodes++;
}

void sort_bvh_primitives_dim(t_bvh_primitive* bvh_primitives,
                             size_t len,
                             uint8_t dim) {
    t_bvh_primitive temp;

    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = i + 1; j < len; j++) {
            t_fvec3 a = bounds_centroid(bvh_primitives[i].bounds);
            t_fvec3 b = bounds_centroid(bvh_primitives[j].bounds);
            bool swap = false;
            if (dim == 0)
                swap = a.x < b.x;
            else if (dim == 1)
                swap = a.y < b.y;
            else if (dim == 2)
                swap = a.y < b.y;
            if (swap) {
                temp = bvh_primitives[i];
                bvh_primitives[i] = bvh_primitives[j];
                bvh_primitives[j] = temp;
            }
        }
    }
}

t_bvh_build_node* bvh_build_recursive(t_bvh_build_state* state,
                                      t_bvh_primitive* bvh_primitives,
                                      size_t bvh_prim_len) {
    t_bvh_build_node* ret = ft_calloc(1, sizeof(*ret));

    for (size_t i = 0; i < bvh_prim_len; i++) {
        if (i == 0)
            ret->bounds = bvh_primitives[i].bounds;
        ret->bounds =
            bounds_extend_bounds(ret->bounds, bvh_primitives[i].bounds);
    }
    if (bounds_surphace_area(ret->bounds) == 0 || bvh_prim_len == 1) {
        // Crate leaf node
        bvh_create_leaf_node(state, bvh_primitives, bvh_prim_len, ret);
        if (!verify_tree(state->ordered_shapes, &state->state, ret)) {
            printf("Failed during leaf node construciton\n");
            exit(1);
        }
        return ret;
    }
    int dim = bounds_max_dim(ret->bounds);
    int mid = bvh_prim_len / 2;

    sort_bvh_primitives_dim(bvh_primitives, bvh_prim_len, dim);

    ret->children[0] = bvh_build_recursive(state, bvh_primitives, mid);
    ret->children[1] =
        bvh_build_recursive(state, bvh_primitives + mid, bvh_prim_len - mid);
    ret->n_primitives = 0;
    ret->split_axis = dim;
    state->total_nodes++;

    return ret;
}

void flatten_bvh(t_linear_bvh_nd* linear, int* offset, t_bvh_build_node* tree) {
    int curr_offset = (*offset)++;
    linear[curr_offset].bounds = tree->bounds;
    linear[curr_offset].axis = tree->split_axis;
    linear[curr_offset].n_primitives = tree->n_primitives;

    if (tree->n_primitives) {
        linear[curr_offset]._union.primitives_offset = tree->first_prim_offset;
    } else {
        flatten_bvh(linear, offset, tree->children[0]);
        linear[curr_offset]._union.second_child_offset = *offset;
        flatten_bvh(linear, offset, tree->children[1]);
    }
    free(tree);
}

bool verify_tree(t_vec_shape* ordered_shapes,
                 t_state* state,
                 t_bvh_build_node* tree) {
    if (tree->n_primitives) {
        for (int i = 0; i < tree->n_primitives; i++) {
            assert(tree->n_primitives == 1);
            if (tree->first_prim_offset + i > ordered_shapes->len) {
                printf("Segfault incomming, %i\n", tree->first_prim_offset + i);
                return false;
            }
            t_shape shape = ordered_shapes->buff[tree->first_prim_offset + i];
            t_bounds3f bounds = shape_bounds(state, shape);
            if (!bounds_is_encapsulated(tree->bounds, bounds)) {
                printf(
                    "Verification failed, %f %f %f,  %f %f %f\n",
                    tree->bounds.min.x, tree->bounds.min.y, tree->bounds.min.z,

                    tree->bounds.max.x, tree->bounds.max.y, tree->bounds.max.z);

                printf("Verification failed, %f %f %f,  %f %f %f\n",
                       bounds.min.x, bounds.min.y, bounds.min.z,

                       bounds.max.x, bounds.max.y, bounds.max.z);
                return (false);
            }
        }
    } else {
        if (!verify_tree(ordered_shapes, state, tree->children[0]))
		{
			printf("bt 0\n");
            return (false);
		}
        if (!verify_tree(ordered_shapes, state, tree->children[1]))
		{
			printf("bt 1\n");
            return (false);
		}
    }
    return (true);
}

void build_bvh(t_state* state) {
    t_bvh_primitive* bvh_primitives =
        malloc(sizeof(*bvh_primitives) * state->shapes.len);

    for (size_t i = 0; i < state->shapes.len; i++) {
        bvh_primitives[i] = (t_bvh_primitive){
            .bounds = shape_bounds(state, state->shapes.buff[i]),
            .shape_idx = i,
        };
    }

    t_vec_shape ordered_shapes;
    vec_shape_init(&ordered_shapes, state->shapes.len);
    t_bvh_build_state bstate = {.state = *state,
                                .ordered_shapes = &ordered_shapes};
    printf("starting build\n");
    t_bvh_build_node* tree =
        bvh_build_recursive(&bstate, bvh_primitives, state->shapes.len);

    printf("tree built\n");
    printf("verifying tree\n");
    if (!verify_tree(&ordered_shapes, state, tree))
        exit(1);

    printf("tree built\n");
    free(bvh_primitives);

    t_linear_bvh_nd* linear_bvh =
        malloc(sizeof(t_linear_bvh_nd) * bstate.total_nodes);

    int offset = 0;
    flatten_bvh(linear_bvh, &offset, tree);
    printf("tree flattneed\n");
    state->shapes = ordered_shapes;
    state->bvh = linear_bvh;
}

// t_collision intersect_bvh() {
// 	t_collision ret = {0};
//
//
// }

bool intersect_bounds(t_bounds3f bounds,
                      t_ray r,
                      float tMax,
                      float* hitt0,
                      float* hitt1) {
    float t0 = 0, t1 = tMax;
    for (int i = 0; i < 3; ++i) {
        // <<Update interval for ith bounding box slab>>
        float invRayDir = 1 / fvec3_idx(r.dir, i);

        // float tNear = (pMin[i] - o[i]) * invRayDir;
        float tNear =
            (fvec3_idx(bounds.min, i) - fvec3_idx(r.pos, i)) * invRayDir;
        float tFar =
            (fvec3_idx(bounds.max, i) - fvec3_idx(r.pos, i)) * invRayDir;

        if (tNear > tFar) {
            float temp = tNear;
            tNear = tFar;
            tFar = temp;
        }
        tFar *= 1 + 2 * f32_gamma(3);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1)
            return false;
    }
    if (hitt0)
        *hitt0 = t0;
    if (hitt1)
        *hitt1 = t1;
    return true;
}

// TODO: T_MIN, and pass the current t_min
t_collision collide_bvh(t_state* state, t_ray ray, float t_max, int* steps) {
    int stack_offs = 0;
    int stack[64];
    stack[0] = 0;
    t_collision curr_best = {.collided = false};

    t_fvec3 inv_dir = {
        .x = 1 / ray.dir.x, .y = 1 / ray.dir.y, .z = 1 / ray.dir.z};
    int dir_is_neg[3] = {(int)(inv_dir.x > 0), (int)(inv_dir.y < 0),
                         (int)(inv_dir.z < 0)};

    while (stack_offs >= 0) {
        int curr_nd_idx = stack[stack_offs--];
        // if (*steps < stack_offs + 1)
        // 	*steps = stack_offs + 1;
        t_linear_bvh_nd curr = state->bvh[curr_nd_idx];

        float hit0;
        if (!intersect_bounds(curr.bounds, ray, t_max, &hit0, 0)) {
            continue;
        }
        (*steps)++;
        // if (curr_best.collided && hit0 > curr_best.t)
        // 	continue;
        // if ((*steps) > 10)
        // 	exit (0);
        if (curr.n_primitives > 0) {
            for (int i = 0; i < curr.n_primitives; i++) {
                int prim_offs = curr._union.primitives_offset;
                t_collision coll = collide_shape(
                    state, state->shapes.buff[prim_offs + i], ray, t_max);
                if (coll.collided &&
                    (coll.t < curr_best.t || !curr_best.collided)) {
                    curr_best = coll;
                }
            }
        } else {
            if (dir_is_neg[curr.axis]) {
                stack[++stack_offs] = curr_nd_idx + 1;
                stack[++stack_offs] = curr._union.second_child_offset;
            } else {
                stack[++stack_offs] = curr._union.second_child_offset;
                stack[++stack_offs] = curr_nd_idx + 1;
            }
        }
    }
    return (curr_best);
}
