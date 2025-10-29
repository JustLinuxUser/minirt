#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libft/utils/utils.h"
#include "minirt.h"
#include "mymath.h"
#include "obj_file/obj_parser.h"
#include "libft/libft.h"

static char* mmap_file(size_t* len, const char* filename) {
    struct stat sb;
    char* p;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return NULL;
    }

    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "%s is not a file\n", filename);
        return NULL;
    }

    p = (char*)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    if (close(fd) == -1) {
        perror("close");
        return NULL;
    }

    (*len) = sb.st_size;

    return p;
}
void load_triangles(t_state* state, char* path, t_fvec3 pos, float scale, t_fvec2 rotation, int spectrum_idx) {

    t_mesh mesh = {.spectrum_idx = spectrum_idx};

	t_obj_parser parser;

	if (!get_obj(path, &parser)) {
		ft_printf("parsing failed\n");
		ft_assert("TODO: handle parsing errors better" == 0);
		return;
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
		tmp.z = vert.y;
		tmp.y = vert.z;
		mesh.vertices.buff[i] = tmp;
		vert = tmp;
		sum = fvec3_add(vert, sum);
		// printf("%f %f %f   ", vert.x, vert.y, vert.z);
	}
	t_fvec3 avg = fvec3_scale(sum, 1. / mesh.vertices.len);

	ft_printf("avg: %f %f %f\n", avg.x, avg.y, avg.z);
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
}
