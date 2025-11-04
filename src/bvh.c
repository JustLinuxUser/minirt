#include "libft/alloc/mmalloc.h"
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
	t_vec_shape			ordered_shapes;
	int					total_nodes;
	t_bump_allocator	allocator;
}	t_bvh_build_state;

typedef struct bvh_prim_slice
{
	t_bvh_primitive	*buff;
	size_t			len;
}	t_bvh_prim_slice;

typedef struct s_sah_buckets
{
	t_bounds3f	bounds[SAH_BUCKETS];
	size_t		counts[SAH_BUCKETS];
	float		split_costs[SAH_BUCKETS - 1];
	int			split_counts[SAH_BUCKETS - 1];
}	t_sah_buckets;

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
	size_t		i;
	t_shape		shape;
	t_bounds3f	bounds;

	ret->first_prim_offset = state->ordered_shapes.len;
	i = 0;
	while (i < prims.len)
	{
		shape = state->state.shapes.buff[prims.buff[i].shape_idx];
		bounds = shape_bounds(&state->state, shape);
		if (!bounds_check_enclosed(ret->bounds, bounds))
			ft_printf("Failing in bvh leaf node\n");
		if (ft_memcmp(&bounds, &prims.buff[i].bounds, sizeof(bounds)))
		{
			ft_printf("Got different bounds\n");
		}
		i++;
		vec_shape_push(&state->ordered_shapes, shape);
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

int	cmp_bvh_primitives(const void *av, const void *bv, void *arg)
{
	t_bvh_primitive	*a;
	t_bvh_primitive	*b;
	int				dim;
	t_fvec3			a_c;
	t_fvec3			b_c;

	a = (t_bvh_primitive *)av;
	b = (t_bvh_primitive *)bv;
	dim = *(int *)arg;
	a_c = bounds_centroid(a->bounds);
	b_c = bounds_centroid(b->bounds);
	return (centroid_cmp_dim(a_c, b_c, dim));
}

typedef struct s_sah_splitfn_arg
{
	int			best_split;
	t_bounds3f	bounds;
	int			dim;
}	t_sah_splitfn_arg;

t_bvh_build_node	*bvh_build_recursive(t_bvh_build_state *state,
			t_bvh_prim_slice prims);

void	bvh_midpoint(t_bvh_build_state *state,
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
	ret->children[0] = bvh_build_recursive(state,
			(t_bvh_prim_slice){slice.buff, mid});
	ret->children[1] = bvh_build_recursive(state,
			(t_bvh_prim_slice){slice.buff + mid, slice.len - mid});
	ret->n_primitives = 0;
	ret->split_axis = dim;
	state->total_nodes++;
}

bool	split_fn(void *ptr, void *arg_v)
{
	t_bvh_primitive		*prim;
	t_sah_splitfn_arg	*arg;
	int					b;

	prim = ptr;
	arg = arg_v;
	b = (fvec3_idx(bounds_offset(arg->bounds, bounds_centroid(prim->bounds)),
				arg->dim) * SAH_BUCKETS);
	return (arg->best_split >= b);
}

void	partition_bvh_sah(t_bvh_build_state *state, t_bvh_prim_slice prims,
			t_bvh_build_node *ret, t_sah_splitfn_arg splitfn_args)
{
	t_partition_args	pargs;
	t_bvh_primitive		*p2;
	size_t				mid;

	pargs = (t_partition_args){.arg = &splitfn_args, .data = prims.buff,
		.len = prims.len, .el_size = sizeof(*prims.buff), .sep = split_fn};
	p2 = partition(pargs);
	mid = p2 - prims.buff;
	if (mid == 0 || mid == prims.len)
		bvh_create_leaf_node(state, prims, ret);
	else
	{
		ret->children[0] = bvh_build_recursive(state,
				(t_bvh_prim_slice){prims.buff, mid});
		ret->children[1] = bvh_build_recursive(state,
				(t_bvh_prim_slice){p2, prims.len - mid});
		ret->n_primitives = 0;
		ret->split_axis = splitfn_args.dim;
	}
}

void sah_bucket_set_bounds_degenerate(t_sah_buckets *buckets)
{
	size_t			i;

	i = 0;
	while (i < SAH_BUCKETS)
		buckets->bounds[i++] = BOUNDS_DEGENERATE;
}

t_bounds3f	get_centroid_bounds(t_bvh_prim_slice prims)
{
	t_bounds3f		centroid_bounds;
	size_t			i;

	centroid_bounds = BOUNDS_DEGENERATE;
	i = 0;
	while (i < prims.len)
		centroid_bounds = bounds_extend_pt(
				centroid_bounds,
				bounds_centroid(prims.buff[i++].bounds));
	return (centroid_bounds);
}

t_sah_buckets	init_centroid_buckets(t_bvh_prim_slice prims,
			t_bounds3f *centroid_bounds, int dim)
{
	t_sah_buckets	buckets;
	size_t			i;
	t_bounds3f		curr_bound;
	int				bucket;

	buckets = (t_sah_buckets){0};
	sah_bucket_set_bounds_degenerate(&buckets);
	*centroid_bounds = get_centroid_bounds(prims);
	i = 0;
	while (i < prims.len)
	{
		curr_bound = prims.buff[i].bounds;
		bucket = fvec3_idx(bounds_offset(*centroid_bounds,
					bounds_centroid(curr_bound)), dim) * SAH_BUCKETS;
		ft_assert(bucket >= 0);
		if (bucket == SAH_BUCKETS)
			bucket--;
		buckets.counts[bucket]++;
		buckets.bounds[bucket]
			= bounds_extend_bounds(buckets.bounds[bucket], curr_bound);
		i++;
	}
	return (buckets);
}

void	sah_bucket_splits(t_sah_buckets *buckets)
{
	t_bounds3f	bound_below;
	int			bl;
	t_bounds3f	bound_above;
	int			ab;
	size_t		i;

	bound_below = BOUNDS_DEGENERATE;
	bl = 0;
	i = 0;
	while (i < SAH_BUCKETS - 1)
	{
		bound_below = bounds_extend_bounds(bound_below, buckets->bounds[i]);
		bl += buckets->counts[i];
		buckets->split_counts[i] += bl;
		buckets->split_costs[i++] += bounds_surphace_area(bound_below) * bl;
	}
	bound_above = BOUNDS_DEGENERATE;
	ab = 0;
	i = SAH_BUCKETS - 1;
	while (i > 0)
	{
		bound_above = bounds_extend_bounds(bound_above, buckets->bounds[i]);
		ab += buckets->counts[i];
		buckets->split_costs[i-- - 1] += bounds_surphace_area(bound_above) * ab;
	}
}

void	sah_bucket_min_cost(t_sah_buckets *buckets, float *min_cost,
		int *min_split_idx, t_bvh_build_node *ret_node)
{
	int	i;

	*min_cost = INFINITY;
	*min_split_idx = 0;
	i = 0;
	while (i < SAH_BUCKETS - 1)
	{
		if (buckets->split_costs[i] < *min_cost)
		{
			*min_cost = buckets->split_costs[i];
			*min_split_idx = i;
		}
		i++;
	}
	*min_cost = 1.f / 2.f + *min_cost / bounds_surphace_area(ret_node->bounds);
}

void	bvh_sah(t_bvh_build_state *state,
							t_bvh_prim_slice prims, t_bvh_build_node *ret)
{
	int				dim;
	t_bounds3f		centroid_bounds;
	t_sah_buckets	buckets;
	float			min_cost;
	int				min_split_idx;

	state->total_nodes++;
	dim = bounds_max_dim(ret->bounds);
	buckets = init_centroid_buckets(prims, &centroid_bounds, dim);
	sah_bucket_splits(&buckets);
	sah_bucket_min_cost(&buckets, &min_cost, &min_split_idx, ret);
	if (prims.len < min_cost || prims.len < BVH_SAH_MIN_NUM_NODES)
		bvh_create_leaf_node(state, prims, ret);
	else
		partition_bvh_sah(state, prims, ret, (t_sah_splitfn_arg)
		{.bounds = centroid_bounds, .dim = dim, .best_split = min_split_idx});
}

t_bvh_build_node	*bvh_build_recursive(t_bvh_build_state* state,
						t_bvh_prim_slice slice)
{
	size_t				i;
	t_bvh_build_node	*ret;

	assert(slice.len > 0);
	ret = bump_alloc(&state->allocator, sizeof(*ret));
	*ret = (t_bvh_build_node){.bounds = BOUNDS_DEGENERATE};
	i = 0;
	while (i < slice.len)
		ret->bounds = bounds_extend_bounds(ret->bounds, slice.buff[i++].bounds);
	if (bounds_surphace_area(ret->bounds) == 0 || slice.len == 1)
	{
		bvh_create_leaf_node(state, slice, ret);
		return (ret);
	}
	if (0)
		bvh_midpoint(state, slice, ret);
	else
		bvh_sah(state, slice, ret);
	return (ret);
}

void	flatten_bvh(t_linear_bvh_nd *linear,
			int *offset, t_bvh_build_node *tree)
{
	int	curr_offset;
	int	start_offset;

	start_offset = 0;
	if (!offset)
		offset = &start_offset;
	curr_offset = (*offset)++;
	linear[curr_offset].bounds = tree->bounds;
	linear[curr_offset].axis = tree->split_axis;
	linear[curr_offset].n_primitives = tree->n_primitives;
	if (tree->n_primitives)
		linear[curr_offset]._union.primitives_offset = tree->first_prim_offset;
	else
	{
		flatten_bvh(linear, offset, tree->children[0]);
		linear[curr_offset]._union.second_child_offset = *offset;
		flatten_bvh(linear, offset, tree->children[1]);
	}
}

t_bvh_primitive *init_bvh_primitives(t_state *state)
{
	t_bvh_primitive	*bvh_primitives;
	size_t			i;

	bvh_primitives = mmalloc(sizeof(*bvh_primitives) * state->shapes.len,
			"Allocating a buffer for the bvh primitives");
	i = 0;
	while (i < state->shapes.len)
	{
		bvh_primitives[i] = (t_bvh_primitive){
			.bounds = shape_bounds(state, state->shapes.buff[i]),
			.shape_idx = i,
		};
		i++;
	}
	return (bvh_primitives);
}

void	build_bvh(t_state *state)
{
	t_bvh_primitive		*bvh_primitives;
	t_bvh_build_state	bstate;
	t_bvh_build_node	*tree;

	if (state->shapes.len == 0)
		return ;
	bvh_primitives = init_bvh_primitives(state);
	bstate = (t_bvh_build_state){.state = *state,
		.ordered_shapes = {0},
		.allocator = {.arena_size = 20000000}
	};
	vec_shape_init(&bstate.ordered_shapes, state->shapes.len);
	tree = bvh_build_recursive(&bstate,
			(t_bvh_prim_slice){bvh_primitives, state->shapes.len});
	free(bvh_primitives);
	state->bvh = mmalloc(sizeof(t_linear_bvh_nd) * bstate.total_nodes,
			"Allocating a buffer for the linear bvh");
	flatten_bvh(state->bvh, 0, tree);
	bump_alloc_free(&bstate.allocator);
	state->shapes = bstate.ordered_shapes;
}

void	sort2f(float *a, float *b)
{
	float	temp;

	if (*a <= *b)
		return ;
	temp = *a;
	*a = *b;
	*b = temp;
}

bool	intersect_bounds(t_bounds3f bounds,
			t_ray r,
			float t_max,
			float *hitt0)
{
	t_fvec2	ts;
	float	inv_ray_dir;
	float	t_near;
	float	t_far;
	int		i;

	ts.x = 0;
	ts.y = t_max;
	i = -1;
	while (++i < 3)
	{
		inv_ray_dir = 1 / fvec3_idx(r.dir, i);
		t_near = (fvec3_idx(bounds.min, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		t_far = (fvec3_idx(bounds.max, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		sort2f(&t_near, &t_far);
		t_far *= 1 + 2 * f32_gamma(3);
		ts.x = fmax(t_near, ts.x);
		ts.y = fmin(t_far, ts.y);
		if (ts.x > ts.y)
			return (false);
	}
	if (hitt0)
		*hitt0 = ts.x;
	return (true);
}

t_collision	collide_bvh(t_state* state, t_ray_isector isect)
{
	int			stack_offs;
	int			stack[64];
	t_collision	curr_best;

	stack_offs = 0;
	stack[0] = 0;
	curr_best = (t_collision){.collided = false};
	t_fvec3 inv_dir = {
		.x = 1 / isect.ray.dir.x, .y = 1 / isect.ray.dir.y, .z = 1 / isect.ray.dir.z};
	int dir_is_neg[3] = {(int)(inv_dir.x > 0), (int)(inv_dir.y < 0),
		(int)(inv_dir.z < 0)};

	while (state->bvh && stack_offs >= 0)
	{
		int curr_nd_idx = stack[stack_offs--];
		t_linear_bvh_nd curr = state->bvh[curr_nd_idx];

		float hit0;

		if (!intersect_bounds(curr.bounds, isect.ray, isect.t_max, &hit0))
			continue;
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
