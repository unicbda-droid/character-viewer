#ifndef WORLD_H
#define WORLD_H

#include "renderer.h"

#define CITY_SIZE 40
#define BLOCK_SIZE 20.0f
#define ROAD_WIDTH 6.0f

typedef struct {
    Vec3 position;
    float width, height, depth;
    Vec3 color;
    Mesh mesh;
} Building;

typedef struct {
    Vec3 position;
    Vec3 direction;
    float speed;
    Mesh body_mesh;
    Mesh wheel_meshes[4];
} Vehicle;

typedef struct {
    Vec3 position;
    Vec3 direction;
    float speed;
    float walk_timer;
    Mesh mesh;
} NPC;

typedef struct {
    Building buildings[200];
    int building_count;
    Mesh ground_mesh;
    Mesh road_mesh;
    Vehicle vehicles[20];
    int vehicle_count;
    NPC npcs[50];
    int npc_count;
} World;

void world_generate(World *w);
void world_draw(Renderer *r, World *w, GLuint program);
void world_destroy(World *w);

#endif
