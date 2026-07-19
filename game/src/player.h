#ifndef PLAYER_H
#define PLAYER_H

#include "math3d.h"
#include <SDL2/SDL.h>

typedef struct {
    Vec3 position;
    float yaw;
    float pitch;
    Vec3 velocity;
    int on_ground;
    float speed;
    float jump_force;
} Player;

void player_init(Player *p);
void player_update(Player *p, float dt, const Uint8 *keys);

#endif
