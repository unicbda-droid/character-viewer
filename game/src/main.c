#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "math3d.h"
#include "shader.h"
#include "renderer.h"
#include "world.h"
#include "player.h"
#include "camera.h"

#define WINDOW_W 1280
#define WINDOW_H 720

static float rand_float(float min, float max) {
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL-Init Fehler: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow(
        "GTA6 Europa - Open World",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        fprintf(stderr, "Fenster Fehler: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_ctx = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW Fehler: %s\n", glewGetErrorString(err));
        return 1;
    }
    printf("OpenGL %s\n", glGetString(GL_VERSION));

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);

    Renderer renderer;
    renderer_init(&renderer, WINDOW_W, WINDOW_H);

    World world;
    world_generate(&world);

    Player player;
    player_init(&player);

    Camera camera;
    camera_init(&camera);

    GLuint program = renderer.program;

    int running = 1;
    Uint64 last_time = SDL_GetPerformanceCounter();
    float time_of_day = 0.4f;

    while (running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last_time) / (float)SDL_GetPerformanceFrequency();
        last_time = now;
        if (dt > 0.05f) dt = 0.05f;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) running = 0;
                if (event.key.keysym.sym == SDLK_F1) {
                    SDL_SetRelativeMouseMode(
                        SDL_GetRelativeMouseMode() ? SDL_FALSE : SDL_TRUE
                    );
                    SDL_ShowCursor(SDL_GetRelativeMouseMode() ? SDL_DISABLE : SDL_ENABLE);
                }
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int w2 = event.window.data1;
                int h2 = event.window.data2;
                renderer.projection = mat4_perspective(1.047f, (float)w2/(float)h2, 0.1f, 500.0f);
                glViewport(0, 0, w2, h2);
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_L]) {
            player.yaw += 2.0f * dt;
        }
        if (keys[SDL_SCANCODE_J]) {
            player.yaw -= 2.0f * dt;
        }

        player_update(&player, dt, keys);
        camera_update(&camera, player.position, dt);

        time_of_day += dt * 0.05f;
        if (time_of_day > 1.0f) time_of_day -= 1.0f;
        renderer.time_of_day = time_of_day;

        renderer_begin_frame(&renderer);
        renderer.view = camera_get_view(&camera);

        glUniform3fv(glGetUniformLocation(program, "uCameraPos"), 1, &camera.position.x);

        world_draw(&renderer, &world, program);

        Mat4 model = mat4_translate(player.position.x, player.position.y, player.position.z);
        glUniformMatrix4fv(glGetUniformLocation(program, "uModel"), 1, GL_FALSE, &model.m[0][0]);
        glUniform3f(glGetUniformLocation(program, "uColor"), 0.2f, 0.5f, 0.9f);

        Mesh cube = mesh_create_cube();
        mesh_draw(&cube);
        mesh_destroy(&cube);

        for (int i = 0; i < world.vehicle_count; i++) {
            Vehicle *v = &world.vehicles[i];
            v->position = vec3_add(v->position, vec3_scale(v->direction, v->speed * dt));
        }

        for (int i = 0; i < world.npc_count; i++) {
            NPC *n = &world.npcs[i];
            n->walk_timer += dt;
            if (n->walk_timer > 3.0f) {
                float new_angle = rand_float(0, 6.28f);
                n->direction = vec3(cosf(new_angle), 0, sinf(new_angle));
                n->walk_timer = 0;
            }
            n->position = vec3_add(n->position, vec3_scale(n->direction, n->speed * dt));
            n->position.y = 1.0f;
        }

        SDL_GL_SwapWindow(window);
    }

    world_destroy(&world);
    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
