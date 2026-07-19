#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>
#include <string.h>

typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;
typedef struct { float m[4][4]; } Mat4;

static inline Vec3 vec3(float x, float y, float z) {
    return (Vec3){x, y, z};
}

static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3(a.x-b.x, a.y-b.y, a.z-b.z);
}

static inline Vec3 vec3_scale(Vec3 v, float s) {
    return vec3(v.x*s, v.y*s, v.z*s);
}

static inline float vec3_dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

static inline float vec3_length(Vec3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len < 0.0001f) return vec3(0, 0, 0);
    return vec3_scale(v, 1.0f / len);
}

static inline Mat4 mat4_identity(void) {
    Mat4 m;
    memset(&m, 0, sizeof(Mat4));
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
    return m;
}

static inline Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 r;
    memset(&r, 0, sizeof(Mat4));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                r.m[i][j] += a.m[i][k] * b.m[k][j];
    return r;
}

static inline Mat4 mat4_translate(float x, float y, float z) {
    Mat4 m = mat4_identity();
    m.m[0][3] = x; m.m[1][3] = y; m.m[2][3] = z;
    return m;
}

static inline Mat4 mat4_scale(float x, float y, float z) {
    Mat4 m = mat4_identity();
    m.m[0][0] = x; m.m[1][1] = y; m.m[2][2] = z;
    return m;
}

static inline Mat4 mat4_rotate_y(float angle) {
    Mat4 m = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    m.m[0][0] = c;  m.m[0][2] = s;
    m.m[2][0] = -s; m.m[2][2] = c;
    return m;
}

static inline Mat4 mat4_rotate_x(float angle) {
    Mat4 m = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    m.m[1][1] = c;  m.m[1][2] = -s;
    m.m[2][1] = s;  m.m[2][2] = c;
    return m;
}

static inline Mat4 mat4_perspective(float fov_rad, float aspect, float near, float far) {
    Mat4 m;
    memset(&m, 0, sizeof(Mat4));
    float tan_half = tanf(fov_rad / 2.0f);
    m.m[0][0] = 1.0f / (aspect * tan_half);
    m.m[1][1] = 1.0f / tan_half;
    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -(2.0f * far * near) / (far - near);
    m.m[3][2] = -1.0f;
    return m;
}

static inline Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 f = vec3_normalize(vec3_sub(center, eye));
    Vec3 r = vec3_normalize(vec3_cross(f, up));
    Vec3 u = vec3_cross(r, f);

    Mat4 m = mat4_identity();
    m.m[0][0] = r.x;  m.m[0][1] = r.y;  m.m[0][2] = r.z;
    m.m[1][0] = u.x;  m.m[1][1] = u.y;  m.m[1][2] = u.z;
    m.m[2][0] = -f.x; m.m[2][1] = -f.y; m.m[2][2] = -f.z;
    m.m[0][3] = -vec3_dot(r, eye);
    m.m[1][3] = -vec3_dot(u, eye);
    m.m[2][3] =  vec3_dot(f, eye);
    return m;
}

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

#endif
