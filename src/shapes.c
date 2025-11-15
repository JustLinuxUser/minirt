/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:54:31 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:54:43 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include <float.h>
#include <stdbool.h>
#include "bounds.h"
#include "libft/utils/utils.h"
#include "minirt.h"

t_fvec3	collision_norm(t_state *state, t_collision collision, t_fvec3 pos)
{
	t_triangle_pts	pts;
	t_sphere		*sphere;

	if (collision.shape.type == OBJ_TRIANGLE)
	{
		pts = triangle_points(state, *(t_triangle *)collision.shape.ptr);
		return (fvec3_norm(
				fvec3_cross(fvec3_sub(pts.a, pts.b), fvec3_sub(pts.a, pts.c))));
	}
	else if (collision.shape.type == OBJ_SPHERE)
	{
		sphere = (t_sphere *)collision.shape.ptr;
		if (collision.u == 0)
			return (fvec3_invert(fvec3_norm(fvec3_sub(pos, sphere->p))));
		return (fvec3_norm(fvec3_sub(pos, sphere->p)));
	}
	else if (collision.shape.type == OBJ_CYLINDER)
		return (collision.norm);
	else if (collision.shape.type == OBJ_PLANE)
		return (((t_plane *)collision.shape.ptr)->dir);
	ft_assert("Unreachable" != 0);
	return ((t_fvec3){0});
}

t_densely_sampled_spectrum	*shape_spectrum(t_state *state,
								t_collision collision)
{
	int	idx;

	idx = 0;
	if (collision.shape.type == OBJ_TRIANGLE)
		idx = state->meshes.buff[(*(t_triangle *)collision.shape.ptr).mesh_idx]
			.spectrum_idx;
	else if (collision.shape.type == OBJ_SPHERE)
		idx = (*(t_sphere *)collision.shape.ptr).spectrum_idx;
	else if (collision.shape.type == OBJ_CYLINDER)
		idx = (*(t_cylinder *)collision.shape.ptr).spectrum_idx;
	else if (collision.shape.type == OBJ_PLANE)
		idx = (*(t_plane *)collision.shape.ptr).spectrum_idx;
	else
	{
		ft_assert("Unreachable" != 0);
		return (0);
	}
	return (state->spectrums.buff + idx);
}

inline static t_bounds3f	bounds_cylinder(t_cylinder cylinder)
{
	t_bounds3f	bounds;

	bounds = bounds_degenerate();
	bounds = bounds_extend_pt(bounds, fvec3_add(cylinder.a, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_sub(cylinder.a, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_add(cylinder.b, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_sub(cylinder.b, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	return (bounds);
}

t_bounds3f	shape_bounds(t_state *state, t_shape shape)
{
	t_bounds3f		bounds;
	t_sphere		sp;
	t_triangle_pts	pts;

	bounds = bounds_degenerate();
	if (shape.type == OBJ_TRIANGLE)
	{
		pts = triangle_points(state, *(t_triangle *)shape.ptr);
		bounds = bounds_extend_pt(bounds, pts.a);
		bounds = bounds_extend_pt(bounds, pts.b);
		bounds = bounds_extend_pt(bounds, pts.c);
	}
	else if (shape.type == OBJ_SPHERE)
	{
		sp = *(t_sphere *)shape.ptr;
		bounds.min = fvec3_sub(sp.p,
				(t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
		bounds.max = fvec3_add(sp.p,
				(t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
	}
	else if (shape.type == OBJ_CYLINDER)
		bounds = bounds_cylinder(*(t_cylinder *)shape.ptr);
	else
		ft_assert("Unreachable" == 0);
	return (bounds);
}
