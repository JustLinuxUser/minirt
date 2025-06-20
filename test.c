#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

// TODOS:
// - Generics
// 		- don't need em
// - BRDF
// - Bump maps
// - Optimize the intersection functions
// - Fix the camera rotations
// - Parsers
// - Cast ray function
//    - With a fast mode, that just checks for intersections
// - Env map (HDRi / equivalent)
// - multithreadeing (compile time configurable)
// - bounding volume hierarchies
//
//

typedef struct sphere {
    Vector3 p;
    float r;
    Color color;
} sphere;

enum obj_type {
    OBJ_SPHERE,
    OBJ_PLANE,
};

typedef union obj_u {
    sphere sphere;
    Ray plane;
} obj_u;

typedef struct obj {
    enum obj_type type;
    obj_u obj;
} obj;

bool intersect_sphere(sphere s, Ray r, float* t) {
    Vector3 L = Vector3Subtract(s.p, r.position);

    // if tc is less then 0, we are looking away from the center (mb we are
    // inside the sphere)
    float tc = Vector3DotProduct(L, Vector3Normalize(r.direction));
    // printf("tc: %f\n", tc);
    // if (tc < 0.0) {
    //     return false;
    // }

    // TODO: Optimize the sqrt
    float d = Vector3DotProduct(L, L) - tc * tc;
    if (d > s.r * s.r || d < 0) {
        return (false);
    }
    float tlc = sqrt(s.r * s.r - d);
    float t1 = tc + tlc;
    float t2 = tc - tlc;

	if ((t1 < t2 && t1 > 0) || t2 < 0)
		*t = t1;
	else
		*t = t2;
    // *p = Vector3Add(Vector3Scale(r.direction, closest_t), r.position);
	// return (true);
	*t *= 0.99;
    return (*t > 0.1);
}

int intersect_plane(Ray plane, Ray r, float* t) {
    float denom = Vector3DotProduct(r.direction, plane.direction);
	// close to parallel, to be considered paralell
    if (fabs(denom) < 1e-7) {
        return false;
        return (2);
    }
    *t = Vector3DotProduct(Vector3Subtract(plane.position, r.position),
                           plane.direction) /
         denom;
    if (*t < 1e-6)
        return false;
    // *p = Vector3Add(Vector3Scale(r.direction, t), r.position);
    return (true);
}

Vector3 vec3_rotate_pitch_yaw(Vector3 v, float pitch, float yaw) {
    return (Vector3RotateByQuaternion(v, QuaternionFromEuler(pitch, yaw, 0)));
}

typedef struct t_state {
} t_state;

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Ray(*proj_vectors)[screenWidth] =
        malloc(screenWidth * screenHeight * sizeof(Ray));

    const float screen_dist = 1;

    const float fov_x = PI / 2.3;
    const float proj_plane_width = 2 * tan(fov_x / 2) * screen_dist;
    const float proj_coef = proj_plane_width / screenWidth;

    float cam_pitch = 0;
    float cam_yaw = 0;

    Ray cam = {.position = {.y = 0}};

    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            // calculate the zoom ratio
            Vector3 pos = {.x = ((float)x - screenWidth / 2.) * proj_coef,
                           .y = ((float)y - screenHeight / 2.) * proj_coef,
                           .z = screen_dist};
            proj_vectors[y][x] = (Ray){.direction = Vector3Normalize(pos)};
        }
    }

    obj objs[] = {
        {.type = OBJ_SPHERE,
         .obj = {.sphere = {.r = 10,
                            .p = {.x = 0, .y = 0, .z = 100},
                            .color = GREEN}}},
        {.type = OBJ_SPHERE,
         .obj = {.sphere = {.r = 30,
                            .p = {.x = 20, .y = 20, .z = 150},
                            .color = BLUE}}},
        {.type = OBJ_SPHERE,
         .obj = {.sphere = {.r = 20,
                            .p = {.x = 30, .y = -25, .z = 120},
                            .color = GREEN}}},

        // {.type = OBJ_PLANE,
        //  .obj = {.plane = {.position = {.x = 0, .y = 0, .z = -20},
        //                    .direction = Vector3Normalize(
        //                        (Vector3){.x = 0, .y = 0.6, .z = -0.3})}}},
        {.type = OBJ_PLANE,
         .obj = {.plane = {.position = {.x = 0, .y = -50, .z = 0},
                           .direction = Vector3Normalize(
                               (Vector3){.x = 0, .y = 1, .z = 0})}}},
        // {.type = OBJ_PLANE,
        //  .obj = {.plane = {.position = {.x = 0, .y = 0, .z = -20},
        //                    .direction = Vector3Normalize((Vector3){
        //                        .x = 0, .y = 0.3, .z = 0.3})}}}
    };
    // sphere s[] = {{.r = 10, .p = {.x = 0, .y = 0, .z = 100}, .color = RED},
    //               {.r = 30, .p = {.x = 20, .y = 20, .z = 150}, .color =
    //               BLUE},
    //               {.r = 20, .p = {.x = 30, .y = 25, .z = 120}, .color =
    //               GREEN}};

    // Ray r = {.position = {},
    //          .direction = Vector3Normalize((Vector3){.z = 1., .y = 0.1})};

    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - basic window");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_Q)) {
            cam.position.y -= 10;
        }
        if (IsKeyDown(KEY_E)) {
            cam.position.y += 10;
        }
        if (IsKeyDown(KEY_W)) {
            cam.position = Vector3Add(
                cam.position,
                vec3_rotate_pitch_yaw((Vector3){.z = 1}, 0, cam_yaw));
        }
        if (IsKeyDown(KEY_S)) {
            cam.position = Vector3Add(
                cam.position,
                vec3_rotate_pitch_yaw((Vector3){.z = 1}, 0, PI + cam_yaw));
        }
        if (IsKeyDown(KEY_A)) {
            cam.position = Vector3Add(
                cam.position,
                vec3_rotate_pitch_yaw((Vector3){.z = 1}, 0, -PI / 2 + cam_yaw));
        }
        if (IsKeyDown(KEY_D)) {
            cam.position = Vector3Add(
                cam.position,
                vec3_rotate_pitch_yaw((Vector3){.z = 1}, 0, PI / 2 + cam_yaw));
        }
        if (IsKeyDown(KEY_RIGHT)) {
            cam_yaw += 0.1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            cam_yaw -= 0.1;
        }
        if (IsKeyDown(KEY_UP)) {
            cam_pitch += 0.1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            cam_pitch -= 0.1;
        }
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                Ray curr_ray = proj_vectors[y][x];
                curr_ray.direction = vec3_rotate_pitch_yaw(curr_ray.direction,
                                                           cam_pitch, cam_yaw);
                curr_ray.position = Vector3Add(curr_ray.position, cam.position);
                Color curr_color = BLACK;
                Color res_color = BLACK;

                Vector3 p;
                float t;
                Vector3 col_p;
                Ray curr_refl;
				// int plane_collisions = 0;
				int debug = false;

				int last_collided_idx = -1;
				int curr_collided_idx = -1;
                for (int col = 0; col < 20; col++) {
                    float curr_t = INFINITY;
                    int intersected = false;

                    for (int obj_idx = 0;
                         obj_idx < sizeof(objs) / sizeof(*objs); obj_idx++) {
                        obj o = objs[obj_idx];

                        if (o.type == OBJ_SPHERE &&
                            intersect_sphere(o.obj.sphere, curr_ray, &t)) {
                            if (t > 0 || obj_idx != last_collided_idx) {
                                if (t < curr_t) {
									p = Vector3Add(
										Vector3Scale(curr_ray.direction, t),
                                    curr_ray.position);
                                    curr_t = t;
                                    if ((abs((int)p.x / 4) % 2 == 0) ^
                                        (p.x > 0) ^ (p.z > 0) ^
                                        (abs((int)p.z / 4) % 2 == 0))
                                        curr_color = o.obj.sphere.color;
                                    else
                                        curr_color = DARKBLUE;
                                    col_p = p;
                                    curr_refl = (Ray){
                                        .position = p,
                                        .direction = Vector3Reflect(
                                            curr_ray.direction,
                                            Vector3Normalize(Vector3Subtract(
                                                p, o.obj.sphere.p)))};
                                    intersected = true;
									curr_collided_idx = obj_idx;
                                }
                            }
                        }
                        if (o.type == OBJ_PLANE &&
                            intersect_plane(o.obj.plane, curr_ray, &t)) {
                            if (obj_idx != last_collided_idx) {
                                if (t < curr_t) {
									p = Vector3Add(
										Vector3Scale(curr_ray.direction, t),
                                    curr_ray.position);
                                    curr_t = t;
                                    if ((abs((int)p.x / 10) % 2 == 0) ^
                                        (p.x > 0) ^ (p.z > 0) ^
                                        (abs((int)p.z / 10) % 2 == 0))
                                        curr_color = BLACK;
                                    else
                                        curr_color = DARKGRAY;
                                    col_p = p;
                                    curr_refl =
                                        (Ray){.position = p,
                                              .direction = Vector3Reflect(
                                                  curr_ray.direction,
                                                  o.obj.plane.direction)};
                                    intersected = true;
									curr_collided_idx = obj_idx;
                                }
                            }
                        }
                    }
                    if (col)
                        res_color =
                            ColorLerp(res_color, curr_color, 0.3 * (1. / col));
                    else
                        res_color = curr_color;

					last_collided_idx = curr_collided_idx;
                    if (intersected != 1) {
						debug = true;
                        break;
                    }
                    curr_ray = curr_refl;
                    if (col == 0)
                        res_color = curr_color;
                }
				// if (debug)
				// 	res_color = RED;
                DrawPixel(x, y, res_color);
            }
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
}
