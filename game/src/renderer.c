#include "renderer.h"
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *VERT_SRC =
    "#version 330 core\n"
    "layout(location=0) in vec3 aPos;\n"
    "layout(location=1) in vec3 aColor;\n"
    "layout(location=2) in vec3 aNormal;\n"
    "uniform mat4 uModel;\n"
    "uniform mat4 uView;\n"
    "uniform mat4 uProj;\n"
    "out vec3 vColor;\n"
    "out vec3 vNormal;\n"
    "out vec3 vWorldPos;\n"
    "void main() {\n"
    "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
    "    vWorldPos = worldPos.xyz;\n"
    "    vNormal = mat3(uModel) * aNormal;\n"
    "    vColor = aColor;\n"
    "    gl_Position = uProj * uView * worldPos;\n"
    "}\n";

static const char *FRAG_SRC =
    "#version 330 core\n"
    "in vec3 vColor;\n"
    "in vec3 vNormal;\n"
    "in vec3 vWorldPos;\n"
    "uniform vec3 uLightDir;\n"
    "uniform vec3 uLightColor;\n"
    "uniform vec3 uAmbient;\n"
    "uniform vec3 uCameraPos;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    vec3 norm = normalize(vNormal);\n"
    "    vec3 lightD = normalize(-uLightDir);\n"
    "    float diff = max(dot(norm, lightD), 0.0);\n"
    "    vec3 viewDir = normalize(uCameraPos - vWorldPos);\n"
    "    vec3 reflectDir = reflect(normalize(uLightDir), norm);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n"
    "    vec3 result = uAmbient * vColor + diff * uLightColor * vColor + spec * 0.2 * uLightColor;\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}\n";

void renderer_init(Renderer *r, int screen_w, int screen_h) {
    r->program = shader_create(VERT_SRC, FRAG_SRC);
    if (!r->program) {
        fprintf(stderr, "Shader-Fehler!\n");
    }
    r->projection = mat4_perspective(1.047f, (float)screen_w / (float)screen_h, 0.1f, 500.0f);
    r->light_dir = vec3(-0.3f, -1.0f, -0.5f);
    r->light_color = vec3(1.0f, 0.95f, 0.9f);
    r->ambient = vec3(0.25f, 0.25f, 0.3f);
    r->time_of_day = 0.5f;

    glViewport(0, 0, screen_w, screen_h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void renderer_begin_frame(Renderer *r) {
    float sun_angle = r->time_of_day * 3.14159f;
    float sun_height = sinf(sun_angle);
    float sun_horiz = cosf(sun_angle);

    r->light_dir = vec3(-sun_horiz * 0.5f, -sun_height, -0.3f);

    float brightness = clampf(sun_height * 2.0f + 0.2f, 0.1f, 1.0f);
    r->light_color = vec3(brightness, brightness * 0.95f, brightness * 0.9f);
    r->ambient = vec3(0.15f + brightness * 0.15f, 0.15f + brightness * 0.15f, 0.2f + brightness * 0.15f);

    float sky_r = clampf(0.3f + sun_height * 0.5f, 0.05f, 0.6f);
    float sky_g = clampf(0.4f + sun_height * 0.4f, 0.05f, 0.7f);
    float sky_b = clampf(0.6f + sun_height * 0.3f, 0.1f, 0.9f);
    glClearColor(sky_r, sky_g, sky_b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(r->program);
    glUniformMatrix4fv(glGetUniformLocation(r->program, "uProj"), 1, GL_FALSE, &r->projection.m[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(r->program, "uView"), 1, GL_FALSE, &r->view.m[0][0]);
    glUniform3fv(glGetUniformLocation(r->program, "uLightDir"), 1, &r->light_dir.x);
    glUniform3fv(glGetUniformLocation(r->program, "uLightColor"), 1, &r->light_color.x);
    glUniform3fv(glGetUniformLocation(r->program, "uAmbient"), 1, &r->ambient.x);
}

static Mesh make_mesh(float *verts, int count, int floats_per_vert) {
    Mesh m;
    m.vertex_count = count;
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER, count * floats_per_vert * sizeof(float), verts, GL_STATIC_DRAW);
    int stride = floats_per_vert * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    return m;
}

Mesh mesh_create_cube(void) {
    float verts[] = {
        // Vorne (+Z)
        -0.5f,-0.5f, 0.5f, 1,1,1,  0,0,1,
         0.5f,-0.5f, 0.5f, 1,1,1,  0,0,1,
         0.5f, 0.5f, 0.5f, 1,1,1,  0,0,1,
        -0.5f,-0.5f, 0.5f, 1,1,1,  0,0,1,
         0.5f, 0.5f, 0.5f, 1,1,1,  0,0,1,
        -0.5f, 0.5f, 0.5f, 1,1,1,  0,0,1,
        // Hinten (-Z)
         0.5f,-0.5f,-0.5f, 1,1,1,  0,0,-1,
        -0.5f,-0.5f,-0.5f, 1,1,1,  0,0,-1,
        -0.5f, 0.5f,-0.5f, 1,1,1,  0,0,-1,
         0.5f,-0.5f,-0.5f, 1,1,1,  0,0,-1,
        -0.5f, 0.5f,-0.5f, 1,1,1,  0,0,-1,
         0.5f, 0.5f,-0.5f, 1,1,1,  0,0,-1,
        // Rechts (+X)
         0.5f,-0.5f, 0.5f, 1,1,1,  1,0,0,
         0.5f,-0.5f,-0.5f, 1,1,1,  1,0,0,
         0.5f, 0.5f,-0.5f, 1,1,1,  1,0,0,
         0.5f,-0.5f, 0.5f, 1,1,1,  1,0,0,
         0.5f, 0.5f,-0.5f, 1,1,1,  1,0,0,
         0.5f, 0.5f, 0.5f, 1,1,1,  1,0,0,
        // Links (-X)
        -0.5f,-0.5f,-0.5f, 1,1,1, -1,0,0,
        -0.5f,-0.5f, 0.5f, 1,1,1, -1,0,0,
        -0.5f, 0.5f, 0.5f, 1,1,1, -1,0,0,
        -0.5f,-0.5f,-0.5f, 1,1,1, -1,0,0,
        -0.5f, 0.5f, 0.5f, 1,1,1, -1,0,0,
        -0.5f, 0.5f,-0.5f, 1,1,1, -1,0,0,
        // Oben (+Y)
        -0.5f, 0.5f, 0.5f, 1,1,1,  0,1,0,
         0.5f, 0.5f, 0.5f, 1,1,1,  0,1,0,
         0.5f, 0.5f,-0.5f, 1,1,1,  0,1,0,
        -0.5f, 0.5f, 0.5f, 1,1,1,  0,1,0,
         0.5f, 0.5f,-0.5f, 1,1,1,  0,1,0,
        -0.5f, 0.5f,-0.5f, 1,1,1,  0,1,0,
        // Unten (-Y)
        -0.5f,-0.5f,-0.5f, 1,1,1,  0,-1,0,
         0.5f,-0.5f,-0.5f, 1,1,1,  0,-1,0,
         0.5f,-0.5f, 0.5f, 1,1,1,  0,-1,0,
        -0.5f,-0.5f,-0.5f, 1,1,1,  0,-1,0,
         0.5f,-0.5f, 0.5f, 1,1,1,  0,-1,0,
        -0.5f,-0.5f, 0.5f, 1,1,1,  0,-1,0,
    };
    return make_mesh(verts, 36, 9);
}

Mesh mesh_create_plane(float size) {
    float h = size / 2.0f;
    float verts[] = {
        -h, 0, -h, 1,1,1,  0,1,0,
         h, 0, -h, 1,1,1,  0,1,0,
         h, 0,  h, 1,1,1,  0,1,0,
        -h, 0, -h, 1,1,1,  0,1,0,
         h, 0,  h, 1,1,1,  0,1,0,
        -h, 0,  h, 1,1,1,  0,1,0,
    };
    return make_mesh(verts, 6, 9);
}

Mesh mesh_create_box(float w, float h, float d) {
    float hw = w/2, hh = h/2, hd = d/2;
    float verts[] = {
        -hw,-hh, hd, 1,1,1,  0,0,1,
         hw,-hh, hd, 1,1,1,  0,0,1,
         hw, hh, hd, 1,1,1,  0,0,1,
        -hw,-hh, hd, 1,1,1,  0,0,1,
         hw, hh, hd, 1,1,1,  0,0,1,
        -hw, hh, hd, 1,1,1,  0,0,1,

         hw,-hh,-hd, 1,1,1,  0,0,-1,
        -hw,-hh,-hd, 1,1,1,  0,0,-1,
        -hw, hh,-hd, 1,1,1,  0,0,-1,
         hw,-hh,-hd, 1,1,1,  0,0,-1,
        -hw, hh,-hd, 1,1,1,  0,0,-1,
         hw, hh,-hd, 1,1,1,  0,0,-1,

         hw,-hh, hd, 1,1,1,  1,0,0,
         hw,-hh,-hd, 1,1,1,  1,0,0,
         hw, hh,-hd, 1,1,1,  1,0,0,
         hw,-hh, hd, 1,1,1,  1,0,0,
         hw, hh,-hd, 1,1,1,  1,0,0,
         hw, hh, hd, 1,1,1,  1,0,0,

        -hw,-hh,-hd, 1,1,1, -1,0,0,
        -hw,-hh, hd, 1,1,1, -1,0,0,
        -hw, hh, hd, 1,1,1, -1,0,0,
        -hw,-hh,-hd, 1,1,1, -1,0,0,
        -hw, hh, hd, 1,1,1, -1,0,0,
        -hw, hh,-hd, 1,1,1, -1,0,0,

        -hw, hh, hd, 1,1,1,  0,1,0,
         hw, hh, hd, 1,1,1,  0,1,0,
         hw, hh,-hd, 1,1,1,  0,1,0,
        -hw, hh, hd, 1,1,1,  0,1,0,
         hw, hh,-hd, 1,1,1,  0,1,0,
        -hw, hh,-hd, 1,1,1,  0,1,0,

        -hw,-hh,-hd, 1,1,1,  0,-1,0,
         hw,-hh,-hd, 1,1,1,  0,-1,0,
         hw,-hh, hd, 1,1,1,  0,-1,0,
        -hw,-hh,-hd, 1,1,1,  0,-1,0,
         hw,-hh, hd, 1,1,1,  0,-1,0,
        -hw,-hh, hd, 1,1,1,  0,-1,0,
    };
    return make_mesh(verts, 36, 9);
}

Mesh mesh_create_colored_box(float w, float h, float d, float r, float g, float b) {
    float hw = w/2, hh = h/2, hd = d/2;
    float verts[] = {
        -hw,-hh, hd, r,g,b,  0,0,1,
         hw,-hh, hd, r,g,b,  0,0,1,
         hw, hh, hd, r,g,b,  0,0,1,
        -hw,-hh, hd, r,g,b,  0,0,1,
         hw, hh, hd, r,g,b,  0,0,1,
        -hw, hh, hd, r,g,b,  0,0,1,

         hw,-hh,-hd, r,g,b,  0,0,-1,
        -hw,-hh,-hd, r,g,b,  0,0,-1,
        -hw, hh,-hd, r,g,b,  0,0,-1,
         hw,-hh,-hd, r,g,b,  0,0,-1,
        -hw, hh,-hd, r,g,b,  0,0,-1,
         hw, hh,-hd, r,g,b,  0,0,-1,

         hw,-hh, hd, r,g,b,  1,0,0,
         hw,-hh,-hd, r,g,b,  1,0,0,
         hw, hh,-hd, r,g,b,  1,0,0,
         hw,-hh, hd, r,g,b,  1,0,0,
         hw, hh,-hd, r,g,b,  1,0,0,
         hw, hh, hd, r,g,b,  1,0,0,

        -hw,-hh,-hd, r,g,b, -1,0,0,
        -hw,-hh, hd, r,g,b, -1,0,0,
        -hw, hh, hd, r,g,b, -1,0,0,
        -hw,-hh,-hd, r,g,b, -1,0,0,
        -hw, hh, hd, r,g,b, -1,0,0,
        -hw, hh,-hd, r,g,b, -1,0,0,

        -hw, hh, hd, r,g,b,  0,1,0,
         hw, hh, hd, r,g,b,  0,1,0,
         hw, hh,-hd, r,g,b,  0,1,0,
        -hw, hh, hd, r,g,b,  0,1,0,
         hw, hh,-hd, r,g,b,  0,1,0,
        -hw, hh,-hd, r,g,b,  0,1,0,

        -hw,-hh,-hd, r,g,b,  0,-1,0,
         hw,-hh,-hd, r,g,b,  0,-1,0,
         hw,-hh, hd, r,g,b,  0,-1,0,
        -hw,-hh,-hd, r,g,b,  0,-1,0,
         hw,-hh, hd, r,g,b,  0,-1,0,
        -hw,-hh, hd, r,g,b,  0,-1,0,
    };
    return make_mesh(verts, 36, 9);
}

void mesh_draw(Mesh *m) {
    glBindVertexArray(m->vao);
    glDrawArrays(GL_TRIANGLES, 0, m->vertex_count);
    glBindVertexArray(0);
}

void mesh_destroy(Mesh *m) {
    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
    m->vao = m->vbo = 0;
}
