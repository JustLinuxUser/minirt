#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libft/utils/utils.h"
#include "minirt.h"
#include "vecs/vec_mesh.h"

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

void load_triangles(t_state* state) {
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;

    t_mesh mesh = {0};

    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials,
								// "objs/cube2.obj",
								"objs/vrmovil/VR-Mobil.obj",
								// "objs/Matteuccia_Struthiopteris_OBJ/matteucia_struthiopteris_1.obj",
								// "objs/skull/12140_Skull_v3_L2.obj",
								// "objs/skull/skull.obj",
                                get_file_data, NULL, flags);
    if (ret != 0) {
        printf("Parsing failed\n");
        return;
    }

	printf("num verts: %u\n", attrib.num_vertices);
	printf("num shapes: %zu\n", num_shapes);
	printf("num faces: %u\n", attrib.num_faces);

    // attrib.num_vertices;
    // attrib.vertices

    // printf("num_shapes: %zu\n", num_shapes);
    for (size_t i = 0; i < attrib.num_faces; i++) {
			vec_int_push(&mesh.vertex_idxs, attrib.faces[i].v_idx);

		if (attrib.face_num_verts[i] != 3)
			continue;
    }

	// for (size_t i = 0; i < num_shapes; i++) {
	// 	tinyobj_shape_t shape = shapes[i];
	// 	printf("Shape %zu, offset: %u, length: %u\n", i, shape.face_offset, shape.length);
	//
	// 	for (size_t j = 0; j < shape.length; j++) {
	// 		size_t face_idx = shape.face_offset + j;
	// 		// if (attrib.face_num_verts[face_idx] != 3) {
	// 		// 	printf("FUCK!!!!!!!!!!!!!!!!!!!!,%zu  %i\n",face_idx, attrib.face_num_verts[face_idx]);
	// 		// }
	// 		ft_assert(attrib.face_num_verts[face_idx] == 3);
	//
	// 		t_triangle t;
	// 		t.mesh_idx = state->meshes.len;
	// 		t.triangle_idx = face_idx;
	// 		vec_triangle_push(&state->triangles, t);
	//
	// 	}
	// }

	t_fvec3 sum ={0};
    for (size_t i = 0; i < attrib.num_vertices; i++) {
        t_fvec3 vert;
        vert = *(t_fvec3*)&attrib.vertices[i * 3];
		sum = fvec3_add(vert, sum);
	}
	t_fvec3 avg = fvec3_scale(sum, 1./ attrib.num_vertices);

    // for (size_t i = 0; i < attrib.num_vertices; i++) {
    //     t_fvec3 vert;
    //     vert = vec3_rotate_pitch_yaw(*(t_fvec3*)&attrib.vertices[i * 3], 0, PI / 13);
    //
    //     // vert.z = -vert.z;
    //     vert = fvec3_scale(vert, 30);
    //     vert = fvec3_add(vert, (t_fvec3){.z = 150});
    //     vec_fvec3_push(&mesh.vertices, vert);
    // }

    for (size_t i = 0; i < mesh.vertex_idxs.len / 3; i++) {
        t_triangle t;
        t.mesh_idx = state->meshes.len;
        t.triangle_idx = i;
        vec_triangle_push(&state->triangles, t);
    }
    // vec_mesh_push(&state->meshes, mesh);
    for (size_t i = 0; i < attrib.num_vertices; i++) {
        t_fvec3 vert;
		vert = *(t_fvec3*)&attrib.vertices[i * 3];
		vert = fvec3_sub(vert, avg);
        vert = vec3_rotate_pitch_yaw(vert, 0, PI / 13);

        // vert.z = -vert.z;
        vert = fvec3_scale(vert, 30);
        vert = fvec3_add(vert, (t_fvec3){.z = 150});
        vec_fvec3_push(&mesh.vertices, vert);
    }

    // for (size_t i = 0; i < mesh.vertex_idxs.len / 3; i++) {
    // }
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
