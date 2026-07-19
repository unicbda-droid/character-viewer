#include "camera.h"
#include <SDL2/SDL.h>
#include <string.h>

void camera_init(Camera *c) {
    memset(c, 0, sizeof(Camera));
    c->distance = 8.0f;
    c->yaw = 0;
    c->pitch = 0.3f;
    c->sensitivity = 0.003f;
}

void camera_update(Camera *c, Vec3 target_pos, float dt) {
    (void)dt;
    int mx, my;
    SDL_GetRelativeMouseState(&mx, &my);
    c->yaw -= mx * c->sensitivity;
    c->pitch += my * c->sensitivity;
    if (c->pitch > 1.2f) c->pitch = 1.2f;
    if (c->pitch < -0.5f) c->pitch = -0.5f;

    float offset_x = sinf(c->yaw) * cosf(c->pitch) * c->distance;
    float offset_y = sinf(c->pitch) * c->distance;
    float offset_z = cosf(c->yaw) * cosf(c->pitch) * c->distance;

    c->target = target_pos;
    c->position = vec3(
        target_pos.x + offset_x,
        target_pos.y + offset_y + 2.0f,
        target_pos.z + offset_z
    );
}

Mat4 camera_get_view(Camera *c) {
    return mat4_look_at(c->position, c->target, vec3(0, 1, 0));
}
