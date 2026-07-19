#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

GLuint shader_create(const char *vert_src, const char *frag_src);
GLuint shader_load_file(const char *vert_path, const char *frag_path);

#endif
