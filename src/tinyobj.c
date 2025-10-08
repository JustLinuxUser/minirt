#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libft/utils/utils.h"
#include "minirt.h"
#include "obj_file/obj_parser.h"

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
void load_triangles(t_state* state, char* path, t_fvec3 pos, float scale) {

    t_mesh mesh = {0};

	t_obj_parser parser;

	if (!get_obj(path, &parser)) {
		printf("parsing failed\n");
		ft_assert("TODO: handle parsing errors better" == 0);
		return;
	}

	mesh.vertex_idxs = parser.faces;
	mesh.vertices = parser.vertices;

	printf("faces: %zu, buff: %p\n", parser.faces.len, parser.faces.buff);
	printf("vertices %zu, buff: %p\n", parser.vertices.len, parser.vertices.buff);

	t_fvec3 sum = {0};
	// printf("vertecies: \n");
	for (size_t i = 0; i < mesh.vertices.len; i++) {
		t_fvec3 vert;
		vert = mesh.vertices.buff[i];
		sum = fvec3_add(vert, sum);
		// printf("%f %f %f   ", vert.x, vert.y, vert.z);
	}
	t_fvec3 avg = fvec3_scale(sum, 1. / mesh.vertices.len);

	printf("avg: %f %f %f\n", avg.x, avg.y, avg.z);
	for (size_t i = 0; i < mesh.vertex_idxs.len / 3; i++) {
		t_triangle t;
		t.mesh_idx = state->meshes.len;
		t.triangle_idx = i;
		vec_triangle_push(&state->triangles, t);
	}

	for (size_t i = 0; i < mesh.vertices.len; i++) {
		t_fvec3 vert;
		vert = mesh.vertices.buff[i];
		printf("%f %f %f\n", vert.x, vert.y, vert.z);
		vert = fvec3_sub(vert, avg);
		vert = fvec3_scale(vert, scale);
		vert = fvec3_add(vert, pos);
		mesh.vertices.buff[i] = vert;
	}
	vec_mesh_push(&state->meshes, mesh);


	// ft_assert(mesh1.vertex_idxs.len == mesh2.vertex_idxs.len);
	// ft_assert(mesh1.vertices.len == mesh2.vertices.len);
	// for (size_t i = 0; i < mesh1.vertices.len; i++) {
	// 	printf("x %f\n", mesh1.vertices.buff[i].x - mesh2.vertices.buff[i].x);
	// 	printf("y %f\n", mesh1.vertices.buff[i].y - mesh2.vertices.buff[i].y);
	// 	printf("z %f\n", mesh1.vertices.buff[i].z - mesh2.vertices.buff[i].z);
	// }
	//
	// for (size_t i = 0; i < mesh1.vertex_idxs.len; i++) {
	// 	printf("%i, %i\n", mesh1.vertex_idxs.buff[i], mesh2.vertex_idxs.buff[i]);
	// }
	// exit(1);
}
