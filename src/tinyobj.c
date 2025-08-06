#include "minirt.h"
#include "vecs/vec_mesh.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader_c.h"

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

static void get_file_data(void* ctx,
                          const char* filename,
                          const int is_mtl,
                          const char* obj_filename,
                          char** data,
                          size_t* len) {
    // NOTE: If you allocate the buffer with malloc(),
    // You can define your own memory management struct and pass it through
    // `ctx` to store the pointer and free memories at clean up stage(when you
    // quit an app) This example uses mmap(), so no free() required.
    (void)ctx;

    if (!filename) {
        fprintf(stderr, "null filename\n");
        (*data) = NULL;
        (*len) = 0;
        return;
    }

    size_t data_len = 0;

    *data = mmap_file(&data_len, filename);
    (*len) = data_len;
}

void load_triangles(t_state *state)
{
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;

	t_mesh mesh = {0};


    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials, "objs/cube.obj", get_file_data,
                                NULL, flags);
    if (ret != 0) {
        printf("Parsing failed\n");
        return;
    }

	// attrib.num_vertices;
	// attrib.vertices
	
    // printf("num_shapes: %zu\n", num_shapes);
	for (size_t i = 0; i < attrib.num_faces; i++) {
		vec_int_push(&mesh.vertex_idxs, attrib.faces[i].v_idx);
	}

	for (size_t i = 0; i < attrib.num_vertices; i++) {
		vec_fvec3_push(&mesh.vertices, *(t_fvec3 *)&attrib.vertices[i * 3]);
	}

	for (size_t i = 0; i < mesh.vertex_idxs.len / 3; i++) {
		t_triangle t;
		t.mesh_idx = state->meshes.len;
		t.triangle_idx = i;
		vec_triangle_push(&state->triangles, t);
	}
	vec_mesh_push(&state->meshes, mesh);

	tinyobj_attrib_free(&attrib);
	tinyobj_materials_free(materials, num_materials);
	tinyobj_shapes_free(shapes, num_shapes);

  //   for (size_t i = 0; i < shapes[0].length; i++) {
  //       tinyobj_vertex_index_t* start =
  //           &attrib.faces[shapes[0].face_offset + i * 3];
  //       printf("Triangle\n");
		//
		// vec_int_push(&mesh.vertex_idxs, start[0].v_idx);
		// vec_int_push(&mesh.vertex_idxs, start[1].v_idx);
		// vec_int_push(&mesh.vertex_idxs, start[2].v_idx);
		//
  //       printf("pt: %f %f %f\n", attrib.vertices[start[0].v_idx],
  //              attrib.vertices[start[0].v_idx + 1],
  //              attrib.vertices[start[0].v_idx + 2]);
  //       printf("pt: %f %f %f\n", attrib.vertices[start[1].v_idx],
  //              attrib.vertices[start[1].v_idx + 1],
  //              attrib.vertices[start[1].v_idx + 2]);
  //       printf("pt: %f %f %f\n", attrib.vertices[start[2].v_idx],
  //              attrib.vertices[start[2].v_idx + 1],
  //              attrib.vertices[start[2].v_idx + 2]);
  //   }
    // fflush(stdout);
    // fflush(stderr);
    // _exit(0);
}
