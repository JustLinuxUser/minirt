/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tinyobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:17:28 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 16:17:28 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "minirt.h"
#include "mymath.h"
#include "obj_file/obj_parser.h"
#include "libft/ft_printf/ft_printf.h"
#include "obj_loader.h"

void	mesh_swap_yz(t_mesh *mesh)
{
	t_fvec3	vert;
	t_fvec3	tmp;
	size_t	i;

	i = 0;
	while (i < mesh->vertices.len)
	{
		vert = mesh->vertices.buff[i];
		tmp = vert;
		tmp.z = vert.y;
		tmp.y = vert.z;
		mesh->vertices.buff[i] = tmp;
		i++;
	}
}

t_fvec3	mesh_avg(t_mesh *mesh)
{
	t_fvec3	sum;
	size_t	i;

	i = 0;
	sum = (t_fvec3){0};
	while (i < mesh->vertices.len)
	{
		sum = fvec3_add(mesh->vertices.buff[i], sum);
		i++;
	}
	return (fvec3_scale(sum, 1. / mesh->vertices.len));
}

void	mesh_transform(t_mesh *mesh, t_obj_spec s)
{
	t_fvec3	avg;
	size_t	i;
	t_fvec3	vert;

	if (s.forward_z)
		mesh_swap_yz(mesh);
	avg = mesh_avg(mesh);
	i = 0;
	while (i < mesh->vertices.len)
	{
		vert = mesh->vertices.buff[i];
		vert = fvec3_sub(vert, avg);
		vert = vec3_rotate_yaw_pitch(vert, s.rotation.x, s.rotation.y);
		vert = fvec3_scale(vert, s.scale);
		vert = fvec3_add(vert, s.pos);
		mesh->vertices.buff[i] = vert;
		i++;
	}
}

bool	load_triangles(t_state *state, t_obj_spec s)
{
	t_mesh			mesh;
	t_obj_parser	parser;
	size_t			i;
	t_triangle		t;

	mesh = (t_mesh){.spectrum_idx = s.spectrum_idx};
	if (!parse_obj(s.path, &parser))
	{
		ft_eprintf("Failed to parse the obj file: %s\n", s.path);
		return (false);
	}
	mesh.vertex_idxs = parser.faces;
	mesh.vertices = parser.vertices;
	mesh_transform(&mesh, s);
	i = 0;
	while (i < mesh.vertex_idxs.len / 3)
	{
		t.mesh_idx = state->meshes.len;
		t.triangle_idx = i;
		vec_triangle_push(&state->triangles, t);
		i++;
	}
	vec_mesh_push(&state->meshes, mesh);
	return (true);
}
