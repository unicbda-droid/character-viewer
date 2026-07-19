#include "world.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static float rand_float(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

static Vec3 building_colors[] = {
    {0.6f, 0.6f, 0.65f},
    {0.5f, 0.5f, 0.55f},
    {0.7f, 0.7f, 0.72f},
    {0.55f, 0.55f, 0.6f},
    {0.65f, 0.62f, 0.58f},
    {0.45f, 0.48f, 0.52f},
};

void world_generate(World *w) {
    srand(42);
    w->building_count = 0;
    w->vehicle_count = 0;
    w->npc_count = 0;

    float total = CITY_SIZE * BLOCK_SIZE;
    float half = total / 2.0f;

    w->ground_mesh = mesh_create_box(total, 0.5f, total);

    for (int gx = 0; gx < CITY_SIZE; gx++) {
        for (int gz = 0; gz < CITY_SIZE; gz++) {
            float bx = gx * BLOCK_SIZE - half + BLOCK_SIZE / 2.0f;
            float bz = gz * BLOCK_SIZE - half + BLOCK_SIZE / 2.0f;

            float road_x = (float)gx * BLOCK_SIZE - half;
            (void)road_x;

            if (rand_float(0, 1) < 0.02f) continue;

            float bsize = BLOCK_SIZE - ROAD_WIDTH - 2.0f;
            if (bsize < 2.0f) bsize = 2.0f;

            float height = rand_float(3.0f, 25.0f);
            float color_idx = rand_float(0, 5.99f);
            Vec3 col = building_colors[(int)color_idx];

            if (w->building_count < 200) {
                Building *b = &w->buildings[w->building_count];
                b->position = vec3(bx, height / 2.0f + 0.25f, bz);
                b->width = rand_float(bsize * 0.4f, bsize);
                b->depth = rand_float(bsize * 0.4f, bsize);
                b->height = height;
                b->color = col;
                b->mesh = mesh_create_colored_box(b->width, b->height, b->depth, col.x, col.y, col.z);
                w->building_count++;
            }
        }
    }

    w->vehicle_count = 5;
    Vec3 car_colors[] = {
        {0.8f, 0.1f, 0.1f},
        {0.1f, 0.1f, 0.8f},
        {0.9f, 0.9f, 0.1f},
        {0.1f, 0.8f, 0.1f},
        {0.9f, 0.5f, 0.1f},
    };
    for (int i = 0; i < w->vehicle_count; i++) {
        Vehicle *v = &w->vehicles[i];
        float angle = rand_float(0, 6.28f);
        float dist = rand_float(20, 100);
        v->position = vec3(cosf(angle)*dist, 0.6f, sinf(angle)*dist);
        v->direction = vec3(cosf(angle), 0, sinf(angle));
        v->speed = rand_float(5, 15);
        Vec3 cc = car_colors[i % 5];
        v->body_mesh = mesh_create_colored_box(2.0f, 0.8f, 4.0f, cc.x, cc.y, cc.z);
        for (int j = 0; j < 4; j++) {
            v->wheel_meshes[j] = mesh_create_box(0.3f, 0.3f, 0.3f);
        }
    }

    w->npc_count = 20;
    Vec3 npc_colors[] = {
        {0.8f, 0.6f, 0.5f},
        {0.5f, 0.8f, 0.6f},
        {0.6f, 0.5f, 0.8f},
        {0.8f, 0.8f, 0.5f},
        {0.5f, 0.6f, 0.8f},
    };
    for (int i = 0; i < w->npc_count; i++) {
        NPC *n = &w->npcs[i];
        float angle = rand_float(0, 6.28f);
        float dist = rand_float(5, 80);
        n->position = vec3(cosf(angle)*dist, 1.0f, sinf(angle)*dist);
        n->direction = vec3(cosf(rand_float(0,6.28f)), 0, sinf(rand_float(0,6.28f)));
        n->speed = rand_float(1, 3);
        n->walk_timer = rand_float(0, 10);
        Vec3 nc = npc_colors[i % 5];
        n->mesh = mesh_create_colored_box(0.5f, 1.8f, 0.5f, nc.x, nc.y, nc.z);
    }
}

void world_draw(Renderer *r, World *w, GLuint program) {
    (void)r;
    Mat4 model;

    model = mat4_translate(0, -0.25f, 0);
    glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &model.m[0][0]);
    glUniform3f(glGetUniformLocation(program, "uColor"), 0.3f, 0.3f, 0.35f);
    mesh_draw(&w->ground_mesh);

    for (int i = 0; i < w->building_count; i++) {
        Building *b = &w->buildings[i];
        model = mat4_translate(b->position.x, b->position.y, b->position.z);
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &model.m[0][0]);
        glUniform3f(glGetUniformLocation(program, "uColor"), b->color.x, b->color.y, b->color.z);
        mesh_draw(&b->mesh);
    }

    for (int i = 0; i < w->vehicle_count; i++) {
        Vehicle *v = &w->vehicles[i];
        float angle = atan2f(v->direction.x, v->direction.z);
        model = mat4_mul(mat4_translate(v->position.x, v->position.y, v->position.z), mat4_rotate_y(angle));
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &model.m[0][0]);
        mesh_draw(&v->body_mesh);
    }

    for (int i = 0; i < w->npc_count; i++) {
        NPC *n = &w->npcs[i];
        model = mat4_translate(n->position.x, n->position.y, n->position.z);
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &model.m[0][0]);
        mesh_draw(&n->mesh);
    }
}

void world_destroy(World *w) {
    for (int i = 0; i < w->building_count; i++)
        mesh_destroy(&w->buildings[i].mesh);
    for (int i = 0; i < w->vehicle_count; i++) {
        mesh_destroy(&w->vehicles[i].body_mesh);
        for (int j = 0; j < 4; j++)
            mesh_destroy(&w->vehicles[i].wheel_meshes[j]);
    }
    for (int i = 0; i < w->npc_count; i++)
        mesh_destroy(&w->npcs[i].mesh);
    mesh_destroy(&w->ground_mesh);
}
