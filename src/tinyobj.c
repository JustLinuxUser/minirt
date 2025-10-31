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

bool load_triangles(t_state* state, char* path, t_fvec3 pos, float scale, t_fvec2 rotation, int spectrum_idx, bool forward_z) {

    t_mesh mesh = {.spectrum_idx = spectrum_idx};

	t_obj_parser parser;

	if (!parse_obj(path, &parser)) {
		ft_eprintf("Failed to parse the obj file: %s\n", path);
		return (false);
	}

	mesh.vertex_idxs = parser.faces;
	mesh.vertices = parser.vertices;

	// printf("faces: %zu, buff: %p\n", parser.faces.len, parser.faces.buff);
	// printf("vertices %zu, buff: %p\n", parser.vertices.len, parser.vertices.buff);

	t_fvec3 sum = {0};
	// printf("vertecies: \n");
	for (size_t i = 0; i < mesh.vertices.len; i++) {
		t_fvec3 vert;
		vert = mesh.vertices.buff[i];
		t_fvec3 tmp;
		tmp = vert;
		if (forward_z) {
			tmp.z = vert.y;
			tmp.y = vert.z;
		}
		mesh.vertices.buff[i] = tmp;
		vert = tmp;
		sum = fvec3_add(vert, sum);
	}
	t_fvec3 avg = fvec3_scale(sum, 1. / mesh.vertices.len);

	for (size_t i = 0; i < mesh.vertex_idxs.len / 3; i++) {
		t_triangle t;
		t.mesh_idx = state->meshes.len;
		t.triangle_idx = i;
		vec_triangle_push(&state->triangles, t);
	}

	for (size_t i = 0; i < mesh.vertices.len; i++) {
		t_fvec3 vert;
		vert = mesh.vertices.buff[i];
		vert = fvec3_sub(vert, avg);
		vert = vec3_rotate_yaw_pitch(vert, rotation.x, rotation.y);
		vert = fvec3_scale(vert, scale);
		vert = fvec3_add(vert, pos);
		mesh.vertices.buff[i] = vert;
	}
	vec_mesh_push(&state->meshes, mesh);
	return true;
}
