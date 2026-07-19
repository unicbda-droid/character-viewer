#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"
#include <GL/glew.h>

typedef struct {
    GLuint vao, vbo;
    int vertex_count;
} Mesh;

typedef struct {
    GLuint program;
    Mat4 projection, view;
    Vec3 light_dir;
    Vec3 light_color;
    Vec3 ambient;
    float time_of_day;
} Renderer;

void renderer_init(Renderer *r, int screen_w, int screen_h);
void renderer_begin_frame(Renderer *r);
Mesh mesh_create_cube(void);
Mesh mesh_create_plane(float size);
Mesh mesh_create_box(float w, float h, float d);
Mesh mesh_create_colored_box(float w, float h, float d, float r, float g, float b);
void mesh_draw(Mesh *m);
void mesh_destroy(Mesh *m);

#endif
