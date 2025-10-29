#include "ray.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "bounds.h"
#include "float.h"
#include "libft/libft.h"
#include "minirt.h"
#include "shapes.h"
#include "libft/alloc/alloc.h"
#include "libft/dsa/smoothsort.h"
#include "libft/dsa/partition.h"

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
	t_bump_allocator allocator;
} t_bvh_build_state;

bool bounds_is_encapsulated(t_bounds3f big, t_bounds3f small) {
    if (big.min.x > small.min.x || big.min.y > small.min.y ||
        big.min.z > small.min.z ||

        big.max.x < small.max.x || big.max.y < small.max.y ||
        big.max.z < small.max.z)
        return (false);
    return (true);
}

void bvh_create_leaf_node(t_bvh_build_state* state,
                          t_bvh_primitive* bvh_primitives,
                          size_t bvh_prim_len,
                          t_bvh_build_node* ret) {
    // if (bvh_prim_len != 1)
        // ft_printf("bvh_len: %zu\n", bvh_prim_len);

    ret->first_prim_offset = state->ordered_shapes->len;
    for (size_t i = 0; i < bvh_prim_len; i++) {

        t_shape shape = state->state.shapes.buff[bvh_primitives[i].shape_idx];

        t_bounds3f bounds = shape_bounds(&state->state, shape);
        if (!bounds_is_encapsulated(ret->bounds, bounds))
            ft_printf("Failing in bvh leaf node\n");

		if (ft_memcmp(&bounds, &bvh_primitives[i].bounds, sizeof(bounds))) {
            ft_printf("Got different bounds\n");
		}
        vec_shape_push(state->ordered_shapes, shape);
    }
    ret->n_primitives = bvh_prim_len;
    state->total_nodes++;
}

int float_cmp(float a, float b) {
	if (a > b)
		return 1;
	if (a < b)
		return -1;
	return 0;
}
int centroid_cmp_dim(t_fvec3 a, t_fvec3 b, uint8_t dim) {
	// printf("dim: %i\n", dim);
	if (dim == 0)
		return float_cmp(a.x, b.x);
	else if (dim == 1)
		return float_cmp(a.y, b.y);
	return float_cmp(a.z, b.z);
}

int cmp_bvh_primitives(const void *av, const void *bv, void *arg) {
	t_bvh_primitive *a = av;
	t_bvh_primitive *b = bv;
	int dim = *(int *)arg;
	t_fvec3 a_c = bounds_centroid(a->bounds);
	t_fvec3 b_c = bounds_centroid(b->bounds);
	return (centroid_cmp_dim(a_c, b_c, dim));
}

// void sort_bvh_primitives_dim(t_bvh_primitive* bvh_primitives,
//                              size_t len,
//                              uint8_t dim) {
//     t_bvh_primitive temp;
//
//     for (size_t i = 0; i < len - 1; i++) {
//         for (size_t j = i + 1; j < len; j++) {
//             t_fvec3 a = bounds_centroid(bvh_primitives[i].bounds);
//             t_fvec3 b = bounds_centroid(bvh_primitives[j].bounds);
//             bool swap = centroid_cmp_dim(a, b, dim) > 0;
//             if (swap) {
// 				printf("swapping, %zu, %zu\n", i, j);
// 				exit(1);
//                 temp = bvh_primitives[i];
//                 bvh_primitives[i] = bvh_primitives[j];
//                 bvh_primitives[j] = temp;
//             }
//         }
//     }
// }
//
//
// void sort_bvh_primitives_dim2(t_bvh_primitive* bvh_primitives,
//                              size_t len,
//                              uint8_t dim) {
//     t_bvh_primitive temp;
//
//     for (size_t i = 1; i < len; i++) {
//         for (size_t j = i; j > 0; j--) {
//             t_fvec3 a = bounds_centroid(bvh_primitives[j].bounds);
//             t_fvec3 b = bounds_centroid(bvh_primitives[j - 1].bounds);
//
//             if (centroid_cmp_dim(a, b, dim) < 0) {
// 				// printf("swapping\n");
//                 temp = bvh_primitives[j];
//                 bvh_primitives[j] = bvh_primitives[j - 1];
//                 bvh_primitives[j - 1] = temp;
//             } else  {
// 				break;
// 			}
//         }
//     }
// }

#define SAH_BUCKETS 50
#define SAH_SPLITS (SAH_BUCKETS - 1)

typedef struct s_sah_splitfn_arg {
    int best_split;
    t_bounds3f bounds;
    int dim;
} t_sah_splitfn_arg;

void print_pt(t_fvec3 pt) {
	ft_printf("x: %f\n", pt.x);
	ft_printf("y: %f\n", pt.y);
	ft_printf("z: %f\n", pt.z);
}

void print_bounds(t_bounds3f bounds) {
	ft_printf("bounds_min: \n");
	print_pt(bounds.min);

	ft_printf("\nbounds_max: \n");
	print_pt(bounds.max);
}

bool split_fn(void *ptr, void *arg_v) {

	t_bvh_primitive *prim = ptr;
	t_sah_splitfn_arg *arg = arg_v;

	int b = (fvec3_idx(bounds_offset(arg->bounds, bounds_centroid(prim->bounds)), arg->dim) * SAH_BUCKETS);
	return arg->best_split >= b;
}

t_bvh_build_node* bvh_build_recursive(t_bvh_build_state* state,
                                      t_bvh_primitive* bvh_primitives,
                                      size_t bvh_prim_len) {
	assert(bvh_prim_len > 0);
    t_bvh_build_node* ret = bump_alloc(&state->allocator, sizeof(*ret));
	*ret = (t_bvh_build_node){.bounds = BOUNDS_DEGENERATE};

    for (size_t i = 0; i < bvh_prim_len; i++) {
        ret->bounds =
            bounds_extend_bounds(ret->bounds, bvh_primitives[i].bounds);
    }

    if (bounds_surphace_area(ret->bounds) == 0 || bvh_prim_len == 1) {
        // Crate leaf node
        bvh_create_leaf_node(state, bvh_primitives, bvh_prim_len, ret);
        return ret;
    }

    int dim = bounds_max_dim(ret->bounds);
	if (0) {
		int mid = bvh_prim_len / 2;

		// sort_bvh_primitives_dim2(bvh_primitives, bvh_prim_len, dim);
		my_smoothsort((t_sort_args){.arr = bvh_primitives,
			.arg = &dim,
			.arr_len = bvh_prim_len,
			.el_size = sizeof(*bvh_primitives),
			.cmp = cmp_bvh_primitives});
		// sort_bvh_primitives_dim(bvh_primitives, bvh_prim_len, dim);

		ret->children[0] = bvh_build_recursive(state, bvh_primitives, mid);
		ret->children[1] =
			bvh_build_recursive(state, bvh_primitives + mid, bvh_prim_len - mid);
		ret->n_primitives = 0;
		ret->split_axis = dim;
		state->total_nodes++;
	} else {
		state->total_nodes++;

		t_bounds3f centroid_bounds = BOUNDS_DEGENERATE;
		for (size_t i = 0; i < bvh_prim_len; i++) {
			centroid_bounds = bounds_extend_pt(
				centroid_bounds,
				bounds_centroid(bvh_primitives[i].bounds));
		}
		// 2
		// 10
		//
		// 5 - 5
		// (2 * 4) / 5
		t_bounds3f bucket_bounds[SAH_BUCKETS];
		for (int i = 0; i < SAH_BUCKETS; i++) {
			bucket_bounds[i] = BOUNDS_DEGENERATE;
		}
		size_t bucket_counts[SAH_BUCKETS] = {0};


		for (size_t i = 0; i < bvh_prim_len; i++)
		{
			t_bounds3f curr_bound = bvh_primitives[i].bounds;
			int bucket = fvec3_idx(bounds_offset(centroid_bounds, bounds_centroid(curr_bound)), dim) * SAH_BUCKETS;
			if (bucket < 0) {
				// ft_printf("bucket: %i\n", bucket);
				// ft_printf("dim: %i\n", dim);
				// ft_printf("num_primitives: %zu\n", bvh_prim_len);
				// ft_printf("float: %f\n", fvec3_idx(bounds_offset(centroid_bounds, bounds_centroid(curr_bound)), dim));

				// print_bounds(curr_bound);
				// ft_printf("\n");
				// print_bounds(centroid_bounds);
			}

			if (bucket == SAH_BUCKETS)
				bucket--;
			bucket_counts[bucket]++;
			bucket_bounds[bucket] = bounds_extend_bounds(bucket_bounds[bucket], curr_bound);
		}

		float split_costs[SAH_SPLITS] = {0};
		int split_counts[SAH_SPLITS] = {0};

		t_bounds3f bound_below = BOUNDS_DEGENERATE;
		int count_below = 0;

		for (size_t i = 0; i < SAH_SPLITS; i++) {
			bound_below = bounds_extend_bounds(bound_below, bucket_bounds[i]);
			count_below += bucket_counts[i];
			split_counts[i] += count_below;
			split_costs[i] += bounds_surphace_area(bound_below) * count_below;
		}

		t_bounds3f bound_above = BOUNDS_DEGENERATE;
		int count_above = 0;

		for (size_t i = SAH_SPLITS; i > 0; i--) {
			bound_above = bounds_extend_bounds(bound_above, bucket_bounds[i]);
			count_above += bucket_counts[i];
			split_costs[i - 1] += bounds_surphace_area(bound_above) * count_above;
		}
		float min_cost = INFINITY;
		int min_split_idx = 0;
		for (int i = 0; i < SAH_SPLITS; i++) {
			if (split_costs[i] < min_cost) {
				min_cost = split_costs[i];
				min_split_idx = i;
			}
		}

		min_cost = 1.f / 4.f + min_cost / bounds_surphace_area(ret->bounds);
		// min_cost = 1;
		// min_split_idx = SAH_SPLITS / 2;
		float leaf_cost = bvh_prim_len;
		// printf("leaf_cost: %f, min_cost: %f\n", leaf_cost, min_cost);
		if ((leaf_cost < min_cost || bvh_prim_len < 3)) {
			// ft_printf("child: %zu\n", bvh_prim_len);
			// ft_printf("child: len: %zu, min_split_idx: %i, split_cost: %f, split_count: %i\n", bvh_prim_len, min_split_idx, min_cost, split_counts[min_split_idx]);
			// Crate leaf node
			bvh_create_leaf_node(state, bvh_primitives, bvh_prim_len, ret);
			return ret;
		} else {
			t_sah_splitfn_arg splitfn_args = {.bounds = centroid_bounds, .dim = dim, .best_split = min_split_idx};
			t_partition_args pargs = {.arg = &splitfn_args, .data = bvh_primitives, .len = bvh_prim_len, .el_size = sizeof(*bvh_primitives), .sep = split_fn};
			t_bvh_primitive *p2 = partition(pargs);

			size_t mid = p2 - bvh_primitives;

			if (mid == 0 || mid == bvh_prim_len) {
				bvh_create_leaf_node(state, bvh_primitives, bvh_prim_len, ret);
			} else {
				// ft_printf("mid: %zu, len: %zu, min_split_idx: %i, split_cost: %f, split_count: %i\n", mid, bvh_prim_len, min_split_idx, min_cost, split_counts[min_split_idx]);
				ret->children[0] = bvh_build_recursive(state, bvh_primitives, mid);
				ret->children[1] =
					bvh_build_recursive(state, p2, bvh_prim_len - mid);
				ret->n_primitives = 0;
				ret->split_axis = dim;
			}
		}
	}

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
                                .ordered_shapes = &ordered_shapes,
		.allocator = {.arena_size = 20000000}
	};

    ft_printf("starting build\n");
    t_bvh_build_node* tree =
        bvh_build_recursive(&bstate, bvh_primitives, state->shapes.len);

    ft_printf("tree built\n");
    ft_printf("verifying tree\n");

    ft_printf("tree built\n");
    free(bvh_primitives);
	// exit(0);

    ft_printf("tree_size: %i\n", bstate.total_nodes);

    t_linear_bvh_nd* linear_bvh =
        malloc(sizeof(t_linear_bvh_nd) * bstate.total_nodes);

    int offset = 0;
    flatten_bvh(linear_bvh, &offset, tree);
	// printf("addler: %u\n", adler32((unsigned char *)linear_bvh, sizeof(t_linear_bvh_nd) * bstate.total_nodes));
	bump_alloc_free(&bstate.allocator);
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
        // Update interval for ith bounding box slab
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

t_collision collide_bvh(t_state* state, t_ray_isector isect) {
    int stack_offs = 0;
    int stack[64];
    stack[0] = 0;
    t_collision curr_best = {.collided = false};

    t_fvec3 inv_dir = {
        .x = 1 / isect.ray.dir.x, .y = 1 / isect.ray.dir.y, .z = 1 / isect.ray.dir.z};
    int dir_is_neg[3] = {(int)(inv_dir.x > 0), (int)(inv_dir.y < 0),
                         (int)(inv_dir.z < 0)};

    while (stack_offs >= 0) {
        int curr_nd_idx = stack[stack_offs--];
        t_linear_bvh_nd curr = state->bvh[curr_nd_idx];

        float hit0;

        if (!intersect_bounds(curr.bounds, isect.ray, isect.t_max, &hit0, 0)) {
            continue;
        }
        if (curr.n_primitives > 0) {
            for (int i = 0; i < curr.n_primitives; i++) {
                int prim_offs = curr._union.primitives_offset;
                t_collision coll = collide_shape(
                    state, state->shapes.buff[prim_offs + i], isect);
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

	for (size_t i = 0; i < state->unbounded_shapes.len; i++) {
		t_collision coll = collide_shape(
			state, state->unbounded_shapes.buff[i], isect);
		if (coll.collided && (coll.t < curr_best.t || !curr_best.collided)) {
			curr_best = coll;
		}
	}
    return (curr_best);
}
