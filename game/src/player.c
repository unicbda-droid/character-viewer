#include "player.h"
#include <SDL2/SDL.h>
#include <string.h>

void player_init(Player *p) {
    memset(p, 0, sizeof(Player));
    p->position = vec3(0, 1.0f, 0);
    p->yaw = 0;
    p->speed = 12.0f;
    p->jump_force = 8.0f;
    p->on_ground = 1;
}

void player_update(Player *p, float dt, const Uint8 *keys) {
    Vec3 forward = vec3(-sinf(p->yaw), 0, -cosf(p->yaw));
    Vec3 right = vec3(cosf(p->yaw), 0, -sinf(p->yaw));

    Vec3 move = vec3(0, 0, 0);
    if (keys[SDL_SCANCODE_W]) move = vec3_add(move, forward);
    if (keys[SDL_SCANCODE_S]) move = vec3_sub(move, forward);
    if (keys[SDL_SCANCODE_A]) move = vec3_sub(move, right);
    if (keys[SDL_SCANCODE_D]) move = vec3_add(move, right);

    if (vec3_length(move) > 0.001f) {
        move = vec3_normalize(move);
    }

    p->velocity.x = move.x * p->speed;
    p->velocity.z = move.z * p->speed;

    if (keys[SDL_SCANCODE_SPACE] && p->on_ground) {
        p->velocity.y = p->jump_force;
        p->on_ground = 0;
    }

    p->velocity.y -= 20.0f * dt;

    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;
    p->position.z += p->velocity.z * dt;

    if (p->position.y <= 1.0f) {
        p->position.y = 1.0f;
        p->velocity.y = 0;
        p->on_ground = 1;
    }
}
