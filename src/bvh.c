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

typedef struct s_bvh_node
{
	size_t		shape_idx;
	t_bounds3f	bounds;
}	t_bvh_primitive;

typedef struct s_bvh_build_node
{
	t_bounds3f					bounds;
	struct s_bvh_build_node		*children[2];
	int							first_prim_offset;
	uint16_t					n_primitives;
	uint8_t						split_axis;
}	t_bvh_build_node;

typedef struct bvh_build_state
{
	t_state				state;
	t_vec_shape			*ordered_shapes;
	int					total_nodes;
	t_bump_allocator	allocator;
}	t_bvh_build_state;

typedef struct bvh_prim_slice {
	t_bvh_primitive	*buff;
	size_t	len;
} t_bvh_prim_slice;

bool	bounds_check_enclosed(t_bounds3f big, t_bounds3f small)
{
	if (big.min.x > small.min.x || big.min.y > small.min.y
		|| big.min.z > small.min.z || big.max.x < small.max.x
		|| big.max.y < small.max.y || big.max.z < small.max.z)
		return (false);
	return (true);
}

void	bvh_create_leaf_node(t_bvh_build_state *state,
						t_bvh_prim_slice prims,
						t_bvh_build_node *ret)
{
	ret->first_prim_offset = state->ordered_shapes->len;
	for (size_t i = 0; i < prims.len; i++) {
		t_shape shape = state->state.shapes.buff[prims.buff[i].shape_idx];
		t_bounds3f bounds = shape_bounds(&state->state, shape);
		if (!bounds_check_enclosed(ret->bounds, bounds))
			ft_printf("Failing in bvh leaf node\n");
		if (ft_memcmp(&bounds, &prims.buff[i].bounds, sizeof(bounds)))
		{
			ft_printf("Got different bounds\n");
		}
		vec_shape_push(state->ordered_shapes, shape);
	}
	ret->n_primitives = prims.len;
	state->total_nodes++;
}

int	float_cmp(float a, float b)
{
	if (a > b)
		return (1);
	if (a < b)
		return (-1);
	return (0);
}

int	centroid_cmp_dim(t_fvec3 a, t_fvec3 b, uint8_t dim)
{
	if (dim == 0)
		return (float_cmp(a.x, b.x));
	else if (dim == 1)
		return (float_cmp(a.y, b.y));
	return (float_cmp(a.z, b.z));
}

int cmp_bvh_primitives(const void *av, const void *bv, void *arg) {
	t_bvh_primitive *a = av;
	t_bvh_primitive *b = bv;
	int dim = *(int *)arg;
	t_fvec3 a_c = bounds_centroid(a->bounds);
	t_fvec3 b_c = bounds_centroid(b->bounds);
	return (centroid_cmp_dim(a_c, b_c, dim));
}

typedef struct s_sah_splitfn_arg
{
	int			best_split;
	t_bounds3f	bounds;
	int			dim;
}	t_sah_splitfn_arg;

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

t_bvh_build_node	*bvh_build_recursive(t_bvh_build_state *state,
			t_bvh_prim_slice prims);

void	bvh_midpoint_split(t_bvh_build_state *state,
							t_bvh_prim_slice slice, t_bvh_build_node *ret)
{
	int	mid;
	int	dim;

	mid = slice.len / 2;
	dim = bounds_max_dim(ret->bounds);
	my_smoothsort((t_sort_args){.arr = slice.buff,
		.arg = &dim,
		.arr_len = slice.len,
		.el_size = sizeof(*slice.buff),
		.cmp = cmp_bvh_primitives});
	ret->children[0] = bvh_build_recursive(state, (t_bvh_prim_slice){slice.buff, mid});
	ret->children[1] = bvh_build_recursive(state, (t_bvh_prim_slice){slice.buff + mid, slice.len - mid});
	ret->n_primitives = 0;
	ret->split_axis = dim;
	state->total_nodes++;
}

bool split_fn(void *ptr, void *arg_v)
{

	t_bvh_primitive *prim = ptr;
	t_sah_splitfn_arg *arg = arg_v;

	int b = (fvec3_idx(bounds_offset(arg->bounds, bounds_centroid(prim->bounds)), arg->dim) * SAH_BUCKETS);
	return arg->best_split >= b;
}


void bvh_sah_split(t_bvh_build_state *state,
							t_bvh_prim_slice prims, t_bvh_build_node *ret)
{
	int	dim;
	t_bounds3f centroid_bounds = BOUNDS_DEGENERATE;

	size_t bucket_counts[SAH_BUCKETS] = {0};
	float split_costs[SAH_BUCKETS - 1] = {0};

	int split_counts[SAH_BUCKETS - 1] = {0};
	t_bounds3f bucket_bounds[SAH_BUCKETS];

	for (int i = 0; i < SAH_BUCKETS; i++) {
		bucket_bounds[i] = BOUNDS_DEGENERATE;
	}

	state->total_nodes++;
	dim = bounds_max_dim(ret->bounds);
	for (size_t i = 0; i < prims.len; i++)
	{
		centroid_bounds = bounds_extend_pt(
			centroid_bounds,
			bounds_centroid(prims.buff[i].bounds));
	}


	for (size_t i = 0; i < prims.len; i++)
	{
		t_bounds3f curr_bound = prims.buff[i].bounds;
		int bucket = fvec3_idx(bounds_offset(centroid_bounds, bounds_centroid(curr_bound)), dim) * SAH_BUCKETS;
		ft_assert(bucket >= 0);
		if (bucket == SAH_BUCKETS)
			bucket--;
		bucket_counts[bucket]++;
		bucket_bounds[bucket] = bounds_extend_bounds(bucket_bounds[bucket], curr_bound);
	}

	t_bounds3f bound_below = BOUNDS_DEGENERATE;
	int count_below = 0;

	for (size_t i = 0; i < SAH_BUCKETS - 1; i++)
	{
		bound_below = bounds_extend_bounds(bound_below, bucket_bounds[i]);
		count_below += bucket_counts[i];
		split_counts[i] += count_below;
		split_costs[i] += bounds_surphace_area(bound_below) * count_below;
	}

	t_bounds3f bound_above = BOUNDS_DEGENERATE;
	int count_above = 0;

	for (size_t i = SAH_BUCKETS - 1; i > 0; i--)
	{
		bound_above = bounds_extend_bounds(bound_above, bucket_bounds[i]);
		count_above += bucket_counts[i];
		split_costs[i - 1] += bounds_surphace_area(bound_above) * count_above;
	}
	float min_cost = INFINITY;
	int min_split_idx = 0;
	for (int i = 0; i < SAH_BUCKETS - 1; i++)
	{
		if (split_costs[i] < min_cost)
		{
			min_cost = split_costs[i];
			min_split_idx = i;
		}
	}
	min_cost = 1.f / 2.f + min_cost / bounds_surphace_area(ret->bounds);
	float leaf_cost = prims.len;
	if (leaf_cost < min_cost || prims.len < BVH_SAH_MIN_NUM_NODES)
		bvh_create_leaf_node(state, prims, ret);
	else
	{
		t_sah_splitfn_arg splitfn_args = {.bounds = centroid_bounds, .dim = dim, .best_split = min_split_idx};
		t_partition_args pargs = {.arg = &splitfn_args, .data = prims.buff, .len = prims.len, .el_size = sizeof(*prims.buff), .sep = split_fn};
		t_bvh_primitive *p2 = partition(pargs);

		size_t mid = p2 - prims.buff;

		if (mid == 0 || mid == prims.len)
			bvh_create_leaf_node(state, prims, ret);
		else
		{
			ret->children[0] = bvh_build_recursive(state, (t_bvh_prim_slice){prims.buff, mid});
			ret->children[1] =
				bvh_build_recursive(state, (t_bvh_prim_slice){p2, prims.len - mid});
			ret->n_primitives = 0;
			ret->split_axis = dim;
		}
	}
}

t_bvh_build_node* bvh_build_recursive(t_bvh_build_state* state,
									  t_bvh_prim_slice slice) {
	assert(slice.len > 0);
	t_bvh_build_node* ret = bump_alloc(&state->allocator, sizeof(*ret));
	*ret = (t_bvh_build_node){.bounds = BOUNDS_DEGENERATE};

	for (size_t i = 0; i < slice.len; i++) {
		ret->bounds =
			bounds_extend_bounds(ret->bounds, slice.buff[i].bounds);
	}

	if (bounds_surphace_area(ret->bounds) == 0 || slice.len == 1) {
		// Crate leaf node
		bvh_create_leaf_node(state, slice, ret);
		return ret;
	}

	if (0)
		bvh_midpoint_split(state, slice, ret);
	else
		bvh_sah_split(state, slice, ret);

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
	if (state->shapes.len == 0)
		return;
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
		bvh_build_recursive(&bstate, (t_bvh_prim_slice){bvh_primitives, state->shapes.len});

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

t_collision	collide_bvh(t_state* state, t_ray_isector isect)
{
	int stack_offs = 0;
	int stack[64];
	stack[0] = 0;
	t_collision curr_best = {.collided = false};

	t_fvec3 inv_dir = {
		.x = 1 / isect.ray.dir.x, .y = 1 / isect.ray.dir.y, .z = 1 / isect.ray.dir.z};
	int dir_is_neg[3] = {(int)(inv_dir.x > 0), (int)(inv_dir.y < 0),
		(int)(inv_dir.z < 0)};

	while (state->bvh && stack_offs >= 0) {
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
