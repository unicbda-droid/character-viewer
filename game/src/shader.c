#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

static GLuint compile_shader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "Shader-Fehler: %s\n", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint shader_create(const char *vert_src, const char *frag_src) {
    GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    if (!vert || !frag) return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        fprintf(stderr, "Shader-Link-Fehler: %s\n", log);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

GLuint shader_load_file(const char *vert_path, const char *frag_path) {
    FILE *f = fopen(vert_path, "r");
    if (!f) { fprintf(stderr, "Kann %s nicht oeffnen\n", vert_path); return 0; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = 0;
    fclose(f);

    FILE *g = fopen(frag_path, "r");
    if (!g) { fprintf(stderr, "Kann %s nicht oeffnen\n", frag_path); free(buf); return 0; }
    fseek(g, 0, SEEK_END);
    long len2 = ftell(g);
    fseek(g, 0, SEEK_SET);
    char *buf2 = malloc(len2 + 1);
    fread(buf2, 1, len2, g);
    buf2[len2] = 0;
    fclose(g);

    GLuint prog = shader_create(buf, buf2);
    free(buf);
    free(buf2);
    return prog;
}
