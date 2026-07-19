#ifndef CAMERA_H
#define CAMERA_H

#include "math3d.h"

typedef struct {
    Vec3 position;
    Vec3 target;
    float distance;
    float yaw;
    float pitch;
    float sensitivity;
} Camera;

void camera_init(Camera *c);
void camera_update(Camera *c, Vec3 target_pos, float dt);
Mat4 camera_get_view(Camera *c);

#endif
