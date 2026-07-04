#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <direct.h>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma comment(lib, "opengl32.lib")

// ======================== MATH ========================
struct vec3 {
    float x, y, z;
    vec3(float x=0, float y=0, float z=0): x(x), y(y), z(z) {}
    vec3 operator+(vec3 v) const { return {x+v.x, y+v.y, z+v.z}; }
    vec3 operator-(vec3 v) const { return {x-v.x, y-v.y, z-v.z}; }
    vec3 operator*(float s) const { return {x*s, y*s, z*s}; }
    vec3 operator/(float s) const { float i=1.0f/s; return {x*i, y*i, z*i}; }
    vec3 operator-() const { return {-x, -y, -z}; }
    vec3& operator+=(vec3 v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
    vec3& operator-=(vec3 v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
    float dot(vec3 v) const { return x*v.x + y*v.y + z*v.z; }
    vec3 cross(vec3 v) const { return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x}; }
    float len() const { return sqrtf(x*x + y*y + z*z); }
    vec3 norm() const { float l = len(); return l > 0.0001f ? *this * (1.0f/l) : vec3(0,0,0); }
};
struct vec4 {
    float x,y,z,w;
    vec4(float x=0, float y=0, float z=0, float w=1): x(x), y(y), z(z), w(w) {}
};
struct mat3x4 {
    float m[12] = {};
    mat3x4() {}
    mat3x4(float* f) { for(int i=0;i<12;i++) m[i]=f[i]; }
};
struct mat4 {
    float m[16] = {};
    mat4() {}
    static mat4 id() { mat4 r; r.m[0]=r.m[5]=r.m[10]=r.m[15]=1; return r; }
    static mat4 persp(float fov, float ar, float n, float f) {
        mat4 r; float t = 1.0f/tanf(fov/2);
        r.m[0]=t/ar; r.m[5]=t; r.m[10]=(f+n)/(n-f); r.m[11]=-1; r.m[14]=2*f*n/(n-f);
        return r;
    }
    static mat4 lookat(vec3 e, vec3 c, vec3 u) {
        vec3 f = (c-e).norm(), s = f.cross(u).norm(), t2 = s.cross(f);
        mat4 r = id();
        r.m[0]=s.x; r.m[4]=s.y; r.m[8]=s.z;
        r.m[1]=t2.x; r.m[5]=t2.y; r.m[9]=t2.z;
        r.m[2]=-f.x; r.m[6]=-f.y; r.m[10]=-f.z;
        r.m[12]=-s.dot(e); r.m[13]=-t2.dot(e); r.m[14]=f.dot(e);
        return r;
    }
    static mat4 trans(vec3 v) { mat4 r = id(); r.m[12]=v.x; r.m[13]=v.y; r.m[14]=v.z; return r; }
    static mat4 scale(vec3 v) { mat4 r = id(); r.m[0]=v.x; r.m[5]=v.y; r.m[10]=v.z; return r; }
    static mat4 rot(float a, vec3 ax) {
        float c = cosf(a), s = sinf(a), t = 1-c;
        float x = ax.x, y = ax.y, z = ax.z;
        mat4 r;
        r.m[0]=t*x*x+c;   r.m[4]=t*x*y-s*z; r.m[8]=t*x*z+s*y;
        r.m[1]=t*x*y+s*z; r.m[5]=t*y*y+c;   r.m[9]=t*y*z-s*x;
        r.m[2]=t*x*z-s*y; r.m[6]=t*y*z+s*x; r.m[10]=t*z*z+c;
        r.m[15]=1;
        return r;
    }
    mat4 operator*(mat4 r) const {
        mat4 res;
        for (int i=0; i<4; i++) for (int j=0; j<4; j++)
            res.m[i*4+j] = m[j]*r.m[i*4] + m[4+j]*r.m[i*4+1] + m[8+j]*r.m[i*4+2] + m[12+j]*r.m[i*4+3];
        return res;
    }
};
mat4 mat4_from_cols(float col0[3], float col1[3], float col2[3], float col3[3]) {
    mat4 r = mat4::id();
    r.m[0]=col0[0]; r.m[1]=col0[1]; r.m[2]=col0[2]; r.m[3]=0;
    r.m[4]=col1[0]; r.m[5]=col1[1]; r.m[6]=col1[2]; r.m[7]=0;
    r.m[8]=col2[0]; r.m[9]=col2[1]; r.m[10]=col2[2]; r.m[11]=0;
    r.m[12]=col3[0]; r.m[13]=col3[1]; r.m[14]=col3[2]; r.m[15]=1;
    return r;
}
mat4 mat4_from_gltf(const float* col_major_16) {
    mat4 r;
    for (int i=0;i<16;i++) r.m[i] = col_major_16[i];
    return r;
}
vec3 transform_pos(mat4 m, vec3 p) {
    float x=m.m[0]*p.x+m.m[4]*p.y+m.m[8]*p.z+m.m[12];
    float y=m.m[1]*p.x+m.m[5]*p.y+m.m[9]*p.z+m.m[13];
    float z=m.m[2]*p.x+m.m[6]*p.y+m.m[10]*p.z+m.m[14];
    return {x,y,z};
}

// ======================== cgltf ========================
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

struct Mesh { GLuint vao=0, vbo=0; int count=0; };

// ======================== GLOBALS ========================
GLFWwindow* window = nullptr;
int fbW = 1024, fbH = 768;
GLuint shader = 0, char_shader = 0, shadow_shader = 0, text_shader = 0;
Mesh sky_dome_mesh, ground_mesh, shadow_mesh;

GLuint g_material_textures[16] = {0};
GLuint font_tex = 0; stbtt_bakedchar font_cdata[96];
unsigned char font_atlas[512*512] = {0};
int font_px = 0; bool font_ready = false, need_font_rebake = true;

vec3 cam_eye(8,6,8), cam_center(0,0,0);
mat4 cam_view, cam_proj;
float cam_dist = 8, cam_theta = 0, cam_phi = 0.6f;

// Character
vec3 char_pos(0,0,0);
vec3 char_vel(0,0,0);
float char_rot = 3.14159f, char_rot_target = 3.14159f;
float anim_time = 0;
int anim_dir = 1;
bool w_pressed=false, s_pressed=false, a_pressed=false, d_pressed=false, space_pressed=false;

// cgltf data
cgltf_data* g_data = nullptr;
int g_num_bones = 0;
int g_bone_parent[256];
cgltf_node* g_armature_node = nullptr;
mat4 g_bone_invbind[256];
mat4 g_bone_world[256];
mat4 g_bone_skin[256];
bool g_has_skin = false;

struct CharMesh {
    GLuint vao=0, vbo=0, ibo=0;
    int num_indices=0, num_vertices=0;
    int base_vertex=0, base_index=0;
    int material_index=0;
    bool valid=false;
    std::vector<float> uv_data;
    std::vector<unsigned short> idx_data;
    bool is_cloth=false;
    // Cloth physics data
    GLuint cloth_vao=0, cloth_vbo=0;
    std::vector<vec3> cloth_pos, cloth_vel, cloth_rest;
    std::vector<bool> cloth_pinned;
    std::vector<int> spring_i0, spring_i1;
    std::vector<float> spring_rest;
    std::vector<float> cloth_jw; // skinning data: 8 floats per vert (4 joints + 4 weights)
    std::vector<vec3> bind_pos; // bind pose positions (for non-cloth collider meshes)
};
std::vector<CharMesh> g_char_meshes;
vec3 g_material_colors[16] = {};
const char* g_material_names[16] = {};
int g_material_count = 0;
int g_uv_view_mat = 0;
GLuint cloth_shader = 0;

// Baked cloth animation
struct BakedCloth {
    std::vector<std::vector<vec3>> frames; // [frame][vertex]
    float fps = 30.0f;
    float duration = 0;
    int num_verts = 0;
    bool ready = false;
};
BakedCloth g_baked[4]; // per animation index

// Editor mode
bool editor_mode = false;
int editor_sel = 0;
float editor_time = 0;
float editor_dur = 5.0f;
bool editor_play = false;
float editor_spd = 1.0f;

struct EdKF {
    float t, rx,ry,rz,rw, tx,ty,tz, sx,sy,sz;
};
struct EdCh {
    int bone;
    std::vector<EdKF> frames;
};
std::vector<EdCh> editor_ch;
bool editor_modified = false;

extern int cur_anim;

// Text rendering
GLuint text_vao = 0, text_vbo = 0;
GLuint ui_vao = 0, ui_vbo = 0;

// ======================== HELPERS ========================
void add_vert_9f(std::vector<float>& v, float* data) {
    for (int i=0;i<9;i++) v.push_back(data[i]);
}
void add_vert_vec(std::vector<float>& v, vec3 p, vec3 n, vec3 c) {
    v.push_back(p.x); v.push_back(p.y); v.push_back(p.z);
    v.push_back(n.x); v.push_back(n.y); v.push_back(n.z);
    v.push_back(c.x); v.push_back(c.y); v.push_back(c.z);
}
void build_mesh(Mesh& m, const std::vector<float>& v, int stride=9) {
    if (m.vao) glDeleteVertexArrays(1,&m.vao);
    if (m.vbo) glDeleteBuffers(1,&m.vbo);
    glGenVertexArrays(1,&m.vao); glGenBuffers(1,&m.vbo);
    glBindVertexArray(m.vao); glBindBuffer(GL_ARRAY_BUFFER,m.vbo);
    glBufferData(GL_ARRAY_BUFFER,v.size()*4,v.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride*4,(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride*4,(void*)(3*4)); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,stride*4,(void*)(6*4)); glEnableVertexAttribArray(2);
    m.count = (int)v.size()/stride;
}

// ======================== CHAR MESH BUILD ========================
void build_char_mesh(CharMesh& cm, const std::vector<float>& verts, const std::vector<unsigned short>& idx) {
    if (cm.vao) glDeleteVertexArrays(1,&cm.vao);
    if (cm.vbo) glDeleteBuffers(1,&cm.vbo);
    if (cm.ibo) glDeleteBuffers(1,&cm.ibo);
    glGenVertexArrays(1,&cm.vao); glGenBuffers(1,&cm.vbo); glGenBuffers(1,&cm.ibo);
    glBindVertexArray(cm.vao);
    glBindBuffer(GL_ARRAY_BUFFER, cm.vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*4, verts.data(), GL_STATIC_DRAW);
    int stride = 16*4;
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride,(void*)(3*4)); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,stride,(void*)(6*4)); glEnableVertexAttribArray(2);
    glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,stride,(void*)(8*4)); glEnableVertexAttribArray(3);
    glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,stride,(void*)(12*4)); glEnableVertexAttribArray(4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cm.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size()*2, idx.data(), GL_STATIC_DRAW);
    cm.num_vertices = (int)verts.size()/16;
    cm.num_indices = (int)idx.size();
    cm.valid = true;
}

// ======================== GLB LOADER ========================
bool load_character(const char* path) {
    cgltf_options opt = {0};
    cgltf_result res = cgltf_parse_file(&opt, path, &g_data);
    if (res != cgltf_result_success) { fprintf(stderr,"cgltf_parse failed\n"); return false; }
    res = cgltf_load_buffers(&opt, g_data, path);
    if (res != cgltf_result_success) { fprintf(stderr,"cgltf_load_buffers failed\n"); return false; }

    cgltf_skin* skin = nullptr;
    cgltf_animation* anim = nullptr;
    if (g_data->skins_count > 0) skin = &g_data->skins[0];
    if (g_data->animations_count > 0) anim = &g_data->animations[0];

    if (skin) {
        g_has_skin = true;
        g_num_bones = (int)skin->joints_count;
        if (g_num_bones > 256) g_num_bones = 256;

        for (int i=0; i<g_num_bones; i++) {
            g_bone_parent[i] = -1;
            if (skin->joints[i]->parent) {
                for (int j=0; j<g_num_bones; j++) {
                    if (skin->joints[j] == skin->joints[i]->parent) { g_bone_parent[i]=j; break; }
                }
            }
            cgltf_accessor* ibm = skin->inverse_bind_matrices;
            if (ibm && i < (int)ibm->count) {
                float* mat = (float*)((char*)ibm->buffer_view->buffer->data + ibm->buffer_view->offset + ibm->offset) + ibm->stride/4*i;
                g_bone_invbind[i] = mat4_from_gltf(mat);
            } else g_bone_invbind[i] = mat4::id();
        }

        for (int mi=0; mi<(int)g_data->meshes_count; mi++) {
            cgltf_mesh* mesh = &g_data->meshes[mi];
            for (int pi=0; pi<(int)mesh->primitives_count; pi++) {
                cgltf_primitive* prim = &mesh->primitives[pi];

                auto ga = [&](const char* name) -> cgltf_accessor* {
                    for (int a=0; a<(int)prim->attributes_count; a++)
                        if (!strcmp(prim->attributes[a].name, name))
                            return prim->attributes[a].data;
                    return nullptr;
                };

                cgltf_accessor* pos_a = ga("POSITION");
                cgltf_accessor* norm_a = ga("NORMAL");
                cgltf_accessor* uv_a = ga("TEXCOORD_0");
                cgltf_accessor* joints_a = ga("JOINTS_0");
                cgltf_accessor* weights_a = ga("WEIGHTS_0");
                if (!pos_a) continue;

                int num_v = (int)pos_a->count;
                std::vector<float> verts;
                verts.reserve(num_v*16);

    float* pos_data = (float*)((char*)pos_a->buffer_view->buffer->data + pos_a->buffer_view->offset + pos_a->offset);
    float* norm_data = norm_a ? (float*)((char*)norm_a->buffer_view->buffer->data + norm_a->buffer_view->offset + norm_a->offset) : nullptr;
    float* uv_accessor = uv_a ? (float*)((char*)uv_a->buffer_view->buffer->data + uv_a->buffer_view->offset + uv_a->offset) : nullptr;
                unsigned char* joints_raw = nullptr; int jstride=0;
                unsigned short* joints_s = nullptr;
                if (joints_a) {
                    joints_raw = (unsigned char*)((char*)joints_a->buffer_view->buffer->data + joints_a->buffer_view->offset + joints_a->offset);
                    jstride = (int)joints_a->stride;
                    if (joints_a->component_type == cgltf_component_type_r_16u)
                        joints_s = (unsigned short*)joints_raw;
                }
                float* weights_data = weights_a ? (float*)((char*)weights_a->buffer_view->buffer->data + weights_a->buffer_view->offset + weights_a->offset) : nullptr;

                int pos_stride = (int)pos_a->stride/4;
                int norm_stride = norm_a ? (int)norm_a->stride/4 : 0;
                int uv_stride = uv_a ? (int)uv_a->stride/4 : 0;
                int w_stride = weights_a ? (int)weights_a->stride/4 : 0;
                if (pi == 0) {
                    fprintf(stderr,"    mesh %s: %d verts uv_stride=%d jstride=%d w_stride=%d\n",
                        mesh->name ? mesh->name : "?", num_v,
                        uv_stride, jstride, w_stride);
                    for (int uvk=0; uvk<num_v&&uvk<5; uvk++)
                        fprintf(stderr,"      uv[%d]=(%.4f,%.4f)\n", uvk,
                            uv_accessor[uvk*uv_stride], uv_accessor[uvk*uv_stride+1]);
                }

                for (int vi=0; vi<num_v; vi++) {
                    verts.push_back(pos_data[vi*pos_stride]);
                    verts.push_back(pos_data[vi*pos_stride+1]);
                    verts.push_back(pos_data[vi*pos_stride+2]);
                    if (norm_data) {
                        verts.push_back(norm_data[vi*norm_stride]);
                        verts.push_back(norm_data[vi*norm_stride+1]);
                        verts.push_back(norm_data[vi*norm_stride+2]);
                    } else { verts.push_back(0); verts.push_back(1); verts.push_back(0); }
                    if (uv_accessor) {
                        verts.push_back(uv_accessor[vi*uv_stride]);
                        verts.push_back(uv_accessor[vi*uv_stride+1]);
                    } else { verts.push_back(0); verts.push_back(0); }
                    if (joints_s) {
                        verts.push_back((float)joints_s[vi*4]);
                        verts.push_back((float)joints_s[vi*4+1]);
                        verts.push_back((float)joints_s[vi*4+2]);
                        verts.push_back((float)joints_s[vi*4+3]);
                    } else if (joints_raw) {
                        verts.push_back((float)joints_raw[vi*jstride]);
                        verts.push_back((float)joints_raw[vi*jstride+1]);
                        verts.push_back((float)joints_raw[vi*jstride+2]);
                        verts.push_back((float)joints_raw[vi*jstride+3]);
                    } else { verts.push_back(0); verts.push_back(0); verts.push_back(0); verts.push_back(0); }
                    if (weights_data) {
                        verts.push_back(weights_data[vi*w_stride]);
                        verts.push_back(weights_data[vi*w_stride+1]);
                        verts.push_back(weights_data[vi*w_stride+2]);
                        verts.push_back(weights_data[vi*w_stride+3]);
                    } else { verts.push_back(1); verts.push_back(0); verts.push_back(0); verts.push_back(0); }
                }

                cgltf_accessor* idx_a = prim->indices;
                std::vector<unsigned short> idx;
                if (idx_a) {
                    int num_i = (int)idx_a->count;
                    idx.reserve(num_i);
                    if (idx_a->component_type == cgltf_component_type_r_16u) {
                        unsigned short* src = (unsigned short*)((char*)idx_a->buffer_view->buffer->data + idx_a->buffer_view->offset + idx_a->offset);
                        for (int i=0; i<num_i; i++) idx.push_back(src[i]);
                    } else if (idx_a->component_type == cgltf_component_type_r_8u) {
                        unsigned char* src = (unsigned char*)((char*)idx_a->buffer_view->buffer->data + idx_a->buffer_view->offset + idx_a->offset);
                        for (int i=0; i<num_i; i++) idx.push_back(src[i]);
                    } else if (idx_a->component_type == cgltf_component_type_r_32u) {
                        unsigned int* src = (unsigned int*)((char*)idx_a->buffer_view->buffer->data + idx_a->buffer_view->offset + idx_a->offset);
                        for (int i=0; i<num_i; i++) idx.push_back((unsigned short)src[i]);
                    }
                }

                CharMesh cm;
                cm.material_index = prim->material ? (int)(prim->material - g_data->materials) : -1;
                build_char_mesh(cm, verts, idx);
                // Store UV and index data for UV wireframe overlay
                if (uv_accessor) {
                    cm.uv_data.resize(num_v * 2);
                    for (int vi = 0; vi < num_v; vi++) {
                        cm.uv_data[vi*2] = uv_accessor[vi*uv_stride];
                        cm.uv_data[vi*2+1] = uv_accessor[vi*uv_stride+1];
                    }
                }
                cm.idx_data = idx;
                cm.bind_pos.resize(num_v);
                for (int vi = 0; vi < num_v; vi++)
                    cm.bind_pos[vi] = vec3(verts[vi*16], verts[vi*16+1], verts[vi*16+2]);
                // Mark cloth mesh (suit = highest material index)
                // Store skin data for CPU cloth sim: 8 floats per vert (4 joints + 4 weights)
                if (joints_s || joints_raw) {
                    cm.cloth_jw.resize(num_v * 8);
                    for (int vi = 0; vi < num_v; vi++) {
                        if (joints_s) {
                            cm.cloth_jw[vi*8] = (float)joints_s[vi*4];
                            cm.cloth_jw[vi*8+1] = (float)joints_s[vi*4+1];
                            cm.cloth_jw[vi*8+2] = (float)joints_s[vi*4+2];
                            cm.cloth_jw[vi*8+3] = (float)joints_s[vi*4+3];
                        } else if (joints_raw) {
                            cm.cloth_jw[vi*8] = (float)joints_raw[vi*jstride];
                            cm.cloth_jw[vi*8+1] = (float)joints_raw[vi*jstride+1];
                            cm.cloth_jw[vi*8+2] = (float)joints_raw[vi*jstride+2];
                            cm.cloth_jw[vi*8+3] = (float)joints_raw[vi*jstride+3];
                        }
                        if (weights_data) {
                            cm.cloth_jw[vi*8+4] = weights_data[vi*w_stride];
                            cm.cloth_jw[vi*8+5] = weights_data[vi*w_stride+1];
                            cm.cloth_jw[vi*8+6] = weights_data[vi*w_stride+2];
                            cm.cloth_jw[vi*8+7] = weights_data[vi*w_stride+3];
                        }
                    }
                }
                g_char_meshes.push_back(cm);
            }
        }
    }

    if (!anim) fprintf(stderr,"warning: no animation found (editor mode can create poses)\n");
    if (!g_has_skin) { fprintf(stderr,"no skin found\n"); return false; }
    if (g_char_meshes.empty()) { fprintf(stderr,"no meshes loaded\n"); return false; }

    fprintf(stderr,"loaded: %d bones, %zu meshes%s\n",
        g_num_bones, g_char_meshes.size(), anim ? " (has animation)" : "");

    // Load embedded textures
    GLuint fallback_tex = 0;
    { // Create 1x1 white fallback texture
        glGenTextures(1, &fallback_tex);
        glBindTexture(GL_TEXTURE_2D, fallback_tex);
        unsigned char wpx[] = {255,255,255,255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, wpx);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (g_data) {
        for (int mi=0; mi<(int)g_data->materials_count && mi<16; mi++) {
            cgltf_material* mat = &g_data->materials[mi];
            fprintf(stderr,"    mat[%d] '%s': has_pbr=%d\n",
                mi, mat->name ? mat->name : "?", mat->has_pbr_metallic_roughness);
            cgltf_texture* tex = mat->has_pbr_metallic_roughness
                ? mat->pbr_metallic_roughness.base_color_texture.texture : nullptr;
            if (!tex && mat->has_pbr_specular_glossiness)
                tex = mat->pbr_specular_glossiness.diffuse_texture.texture;
            if (!tex || !tex->image || !tex->image->buffer_view) {
                fprintf(stderr,"      NO TEXTURE or no image/buffer_view\n");
                g_material_textures[mi] = fallback_tex;
                continue;
            }
            fprintf(stderr,"      tex='%s' img='%s'\n",
                tex->name ? tex->name : "?",
                tex->image->name ? tex->image->name : "?");
            cgltf_buffer_view* bv = tex->image->buffer_view;
            int w, h, n;
            unsigned char* pixels = stbi_load_from_memory(
                (unsigned char*)((char*)bv->buffer->data + bv->offset),
                (int)bv->size, &w, &h, &n, 4);
            if (pixels) {
                glGenTextures(1, &g_material_textures[mi]);
                glBindTexture(GL_TEXTURE_2D, g_material_textures[mi]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(pixels);
                fprintf(stderr,"loaded texture %s (%dx%d)\n",
                    tex->image->name ? tex->image->name : "?", w, h);
            } else {
                fprintf(stderr,"failed to decode texture %s, using fallback\n",
                    tex->image->name ? tex->image->name : "?");
                g_material_textures[mi] = fallback_tex;
            }
        }
        // Fill remaining slots with fallback
        for (int mi=0; mi<16; mi++)
            if (!g_material_textures[mi]) g_material_textures[mi] = fallback_tex;

        // Initialize per-material colors
        g_material_count = g_data->materials_count < 16 ? (int)g_data->materials_count : 16;
        for (int mi = 0; mi < g_material_count; mi++) {
            g_material_colors[mi] = vec3(0.9f, 0.85f, 0.8f);
            g_material_names[mi] = g_data->materials[mi].name ? g_data->materials[mi].name : "?";
        }
    }
    return true;
}

// ======================== CLOTH PHYSICS ========================
#include <set>

void init_cloth(CharMesh& cm) {
    cm.is_cloth = true;
    int nv = (int)cm.cloth_jw.size() / 8;
    if (nv == 0) return;

    cm.cloth_rest.resize(nv);
    cm.cloth_pos.resize(nv);
    cm.cloth_vel.resize(nv);
    cm.cloth_pinned.assign(nv, false);
    cm.spring_i0.clear(); cm.spring_i1.clear(); cm.spring_rest.clear();

    // Read rest positions from VBO
    glBindBuffer(GL_ARRAY_BUFFER, cm.vbo);
    std::vector<float> vd(nv * 16);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, nv * 16 * 4, vd.data());

    for (int vi = 0; vi < nv; vi++) {
        cm.cloth_rest[vi] = vec3(vd[vi*16], vd[vi*16+1], vd[vi*16+2]);
        cm.cloth_vel[vi] = vec3(0,0,0);
    }

    // Skin to current (rest) pose so cloth doesn't start at bind pose while body is animated
    // compute_skin() must have been called before init_cloth
    for (int vi = 0; vi < nv; vi++) {
        vec3 rp = cm.cloth_rest[vi];
        float* jw = &cm.cloth_jw[vi * 8];
        int j[4] = { (int)jw[0], (int)jw[1], (int)jw[2], (int)jw[3] };
        float w[4] = { jw[4], jw[5], jw[6], jw[7] };
        vec3 sp(0,0,0);
        for (int k = 0; k < 4; k++) {
            if (w[k] < 0.001f || j[k] < 0 || j[k] >= g_num_bones) continue;
            mat4& m = g_bone_skin[j[k]];
            sp.x += w[k] * (m.m[0]*rp.x + m.m[4]*rp.y + m.m[8]*rp.z + m.m[12]);
            sp.y += w[k] * (m.m[1]*rp.x + m.m[5]*rp.y + m.m[9]*rp.z + m.m[13]);
            sp.z += w[k] * (m.m[2]*rp.x + m.m[6]*rp.y + m.m[10]*rp.z + m.m[14]);
        }
        cm.cloth_pos[vi] = sp;
    }

    // Build unique edges from index buffer
    struct EdgeKey { int a, b; };
    auto mk_key = [](int a, int b) { return a < b ? EdgeKey{a,b} : EdgeKey{b,a}; };
    struct EdgeCmp {
        bool operator()(const EdgeKey& x, const EdgeKey& y) const {
            if (x.a != y.a) return x.a < y.a; return x.b < y.b;
        }
    };
    std::set<EdgeKey, EdgeCmp> edge_set;
    for (size_t i = 0; i + 2 < cm.idx_data.size(); i += 3) {
        edge_set.insert(mk_key(cm.idx_data[i], cm.idx_data[i+1]));
        edge_set.insert(mk_key(cm.idx_data[i+1], cm.idx_data[i+2]));
        edge_set.insert(mk_key(cm.idx_data[i+2], cm.idx_data[i]));
    }

    for (auto& e : edge_set) {
        cm.spring_i0.push_back(e.a);
        cm.spring_i1.push_back(e.b);
        vec3 d = cm.cloth_rest[e.a] - cm.cloth_rest[e.b];
        cm.spring_rest.push_back(d.len());
    }

    // Pin top 15% vertices by Y
    float min_y = 1e9f, max_y = -1e9f;
    for (auto& v : cm.cloth_rest) {
        if (v.y < min_y) min_y = v.y;
        if (v.y > max_y) max_y = v.y;
    }
    float thresh = max_y - (max_y - min_y) * 0.15f;
    for (int vi = 0; vi < nv; vi++)
        cm.cloth_pinned[vi] = cm.cloth_rest[vi].y >= thresh;

    // Create cloth VBO: format pos(3)+norm(3)+uv(2) = 8 floats
    glGenVertexArrays(1, &cm.cloth_vao);
    glGenBuffers(1, &cm.cloth_vbo);
    glBindVertexArray(cm.cloth_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cm.cloth_vbo);
    // Initialize with rest positions/normals and UVs
    std::vector<float> cvd(nv * 8);
    for (int vi = 0; vi < nv; vi++) {
        cvd[vi*8] = vd[vi*16]; cvd[vi*8+1] = vd[vi*16+1]; cvd[vi*8+2] = vd[vi*16+2]; // pos
        cvd[vi*8+3] = vd[vi*16+3]; cvd[vi*8+4] = vd[vi*16+4]; cvd[vi*8+5] = vd[vi*16+5]; // norm
        cvd[vi*8+6] = vd[vi*16+6]; cvd[vi*8+7] = vd[vi*16+7]; // uv
    }
    glBufferData(GL_ARRAY_BUFFER, nv * 8 * 4, cvd.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,32,(void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,32,(void*)12); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,32,(void*)24); glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cm.ibo);
    cm.num_indices = (int)cm.idx_data.size();
}

void cloth_recompute_normals(CharMesh& cm) {
    int nv = (int)cm.cloth_pos.size();
    if (nv == 0) return;
    std::vector<vec3> norms(nv, vec3(0,0,0));
    for (size_t i = 0; i + 2 < cm.idx_data.size(); i += 3) {
        int i0 = cm.idx_data[i], i1 = cm.idx_data[i+1], i2 = cm.idx_data[i+2];
        vec3 e1 = cm.cloth_pos[i1] - cm.cloth_pos[i0];
        vec3 e2 = cm.cloth_pos[i2] - cm.cloth_pos[i0];
        vec3 fn = e1.cross(e2);
        norms[i0] += fn; norms[i1] += fn; norms[i2] += fn;
    }
    for (int vi = 0; vi < nv; vi++) {
        float l = norms[vi].len();
        if (l > 0.0001f) norms[vi] = norms[vi] * (1.0f / l);
    }
    glBindBuffer(GL_ARRAY_BUFFER, cm.cloth_vbo);
    for (int vi = 0; vi < nv; vi++) {
        float n[3] = { norms[vi].x, norms[vi].y, norms[vi].z };
        glBufferSubData(GL_ARRAY_BUFFER, vi * 32 + 12, 12, n);
    }
}



void update_cloth(float dt) {
    int anim_idx = cur_anim;
    if (anim_idx < 0 || anim_idx >= 4) anim_idx = 0;
    BakedCloth& baked = g_baked[anim_idx];
    if (baked.ready && !baked.frames.empty()) {
        for (auto& cm : g_char_meshes) {
            if (!cm.valid || !cm.is_cloth) continue;
            int nv = (int)cm.cloth_pos.size();
            if (nv == 0) continue;
            int nf = (int)baked.frames.size();
            if (nf < 1) continue;

            float t = fmodf(anim_time, baked.duration);
            if (t < 0) t += baked.duration;
            float fi = t * baked.fps;
            int f0 = (int)fi;
            int f1 = f0 < nf - 1 ? f0 + 1 : 0;
            float frac = fi - (float)f0;

            for (int vi=0; vi<nv && vi<(int)baked.frames[f0].size(); vi++)
                cm.cloth_pos[vi] = baked.frames[f0][vi] + (baked.frames[f1][vi] - baked.frames[f0][vi]) * frac;

            glBindBuffer(GL_ARRAY_BUFFER, cm.cloth_vbo);
            for (int vi=0; vi<nv; vi++)
                glBufferSubData(GL_ARRAY_BUFFER, vi * 32, 12, &cm.cloth_pos[vi]);
            cloth_recompute_normals(cm);
        }
        return;
    }

    // Fallback: live cloth simulation
    if (dt > 0.03f) dt = 0.03f;
    float gravity = -12.0f;
    float damping = 0.988f;
    float stiffness = 6.0f;
    int iters = 4;

    for (auto& cm : g_char_meshes) {
        if (!cm.valid || !cm.is_cloth) continue;
        int nv = (int)cm.cloth_pos.size();
        if (nv == 0 || cm.cloth_jw.empty()) continue;

        std::vector<vec3> target(nv);
        for (int vi = 0; vi < nv; vi++) {
            vec3 rp = cm.cloth_rest[vi];
            float* jw = &cm.cloth_jw[vi * 8];
            int j[4] = { (int)jw[0], (int)jw[1], (int)jw[2], (int)jw[3] };
            float w[4] = { jw[4], jw[5], jw[6], jw[7] };
            vec3 sp(0,0,0);
            for (int k=0;k<4;k++) {
                if (w[k]<0.001f||j[k]<0||j[k]>=g_num_bones) continue;
                mat4& m = g_bone_skin[j[k]];
                sp.x += w[k]*(m.m[0]*rp.x+m.m[4]*rp.y+m.m[8]*rp.z+m.m[12]);
                sp.y += w[k]*(m.m[1]*rp.x+m.m[5]*rp.y+m.m[9]*rp.z+m.m[13]);
                sp.z += w[k]*(m.m[2]*rp.x+m.m[6]*rp.y+m.m[10]*rp.z+m.m[14]);
            }
            target[vi] = sp;
        }

        for (int vi = 0; vi < nv; vi++) {
            if (cm.cloth_pinned[vi]) {
                cm.cloth_pos[vi] = target[vi];
                cm.cloth_vel[vi] = vec3(0,0,0);
            } else {
                cm.cloth_vel[vi] += (target[vi] - cm.cloth_pos[vi]) * stiffness * dt;
                cm.cloth_vel[vi].y += gravity * dt;
                cm.cloth_vel[vi] = cm.cloth_vel[vi] * damping;
                cm.cloth_pos[vi] += cm.cloth_vel[vi] * dt;
            }
        }

        for (int iter = 0; iter < iters; iter++) {
            for (size_t si = 0; si < cm.spring_i0.size(); si++) {
                int a = cm.spring_i0[si], b = cm.spring_i1[si];
                if (cm.cloth_pinned[a] && cm.cloth_pinned[b]) continue;
                vec3 d = cm.cloth_pos[b] - cm.cloth_pos[a];
                float dist = d.len();
                if (dist < 0.0001f) continue;
                float corr = (dist - cm.spring_rest[si]) / dist * 0.5f;
                if (!cm.cloth_pinned[a]) cm.cloth_pos[a] += d * corr;
                if (!cm.cloth_pinned[b]) cm.cloth_pos[b] -= d * corr;
            }
        }

        // Bone-based collision spheres
        for (int vi = 0; vi < nv; vi++) {
            if (cm.cloth_pinned[vi]) continue;
            vec3& p = cm.cloth_pos[vi];
            float* jw = &cm.cloth_jw[vi * 8];
            vec3 col_push(0,0,0);
            int col_cnt = 0;
            for (int k = 0; k < 4; k++) {
                int bi = (int)jw[k];
                if (bi < 0 || bi >= g_num_bones || jw[k+4] < 0.001f) continue;
                vec3 bpos(g_bone_world[bi].m[12], g_bone_world[bi].m[13], g_bone_world[bi].m[14]);
                vec3 d = p - bpos;
                float dist = d.len();
                float bone_r = 0.06f + jw[k+4] * 0.08f;
                if (dist < bone_r && dist > 0.001f) {
                    col_push += d * ((bone_r - dist) / dist) * jw[k+4];
                    col_cnt++;
                }
            }
            if (col_cnt > 0)
                p += col_push * (0.5f / col_cnt);
        }

        for (int vi = 0; vi < nv; vi++) {
            if (cm.cloth_pos[vi].y < 0) {
                cm.cloth_pos[vi].y = 0;
                cm.cloth_vel[vi].y = 0;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, cm.cloth_vbo);
        for (int vi = 0; vi < nv; vi++)
            glBufferSubData(GL_ARRAY_BUFFER, vi * 32, 12, &cm.cloth_pos[vi]);
        cloth_recompute_normals(cm);
    }
}

// ======================== EDITOR ========================
void editor_init() {
    if (!g_has_skin || g_num_bones < 1) return;
    editor_ch.clear();
    for (int i=0; i<g_num_bones; i++) {
        EdCh ch;
        ch.bone = i;
        EdKF kf;
        kf.t = 0; kf.rx=0; kf.ry=0; kf.rz=0; kf.rw=1;
        kf.tx=0; kf.ty=0; kf.tz=0;
        kf.sx=1; kf.sy=1; kf.sz=1;
        ch.frames.push_back(kf);
        editor_ch.push_back(ch);
    }
    // Try loading saved animation
    std::ifstream f("gussman.anim", std::ios::binary);
    if (f.is_open()) {
        char magic[4]; f.read(magic,4);
        if (magic[0]=='A'&&magic[1]=='N'&&magic[2]=='I'&&magic[3]=='M') {
            int ver; f.read((char*)&ver,4);
            int nch; f.read((char*)&nch,4);
            if (nch == g_num_bones) {
                for (int i=0; i<nch; i++) {
                    int bi; f.read((char*)&bi,4);
                    int nkf; f.read((char*)&nkf,4);
                    editor_ch[i].frames.clear();
                    for (int j=0; j<nkf; j++) {
                        EdKF kf;
                        f.read((char*)&kf.t,4);
                        f.read((char*)&kf.rx,4); f.read((char*)&kf.ry,4); f.read((char*)&kf.rz,4); f.read((char*)&kf.rw,4);
                        f.read((char*)&kf.tx,4); f.read((char*)&kf.ty,4); f.read((char*)&kf.tz,4);
                        f.read((char*)&kf.sx,4); f.read((char*)&kf.sy,4); f.read((char*)&kf.sz,4);
                        editor_ch[i].frames.push_back(kf);
                    }
                }
                f.close();
                fprintf(stderr,"loaded gussman.anim (%d bones)\n", g_num_bones);
                return;
            }
        }
        f.close();
    }
    fprintf(stderr,"editor initialized with %d bones\n", g_num_bones);
}

void editor_save() {
    std::ofstream f("gussman.anim", std::ios::binary);
    if (!f.is_open()) { fprintf(stderr,"save failed\n"); return; }
    char magic[4] = {'A','N','I','M'};
    f.write(magic,4);
    int ver = 1; f.write((char*)&ver,4);
    int nch = (int)editor_ch.size(); f.write((char*)&nch,4);
    for (auto& ch : editor_ch) {
        f.write((char*)&ch.bone,4);
        int nkf = (int)ch.frames.size(); f.write((char*)&nkf,4);
        for (auto& kf : ch.frames) {
            f.write((char*)&kf.t,4);
            f.write((char*)&kf.rx,4); f.write((char*)&kf.ry,4); f.write((char*)&kf.rz,4); f.write((char*)&kf.rw,4);
            f.write((char*)&kf.tx,4); f.write((char*)&kf.ty,4); f.write((char*)&kf.tz,4);
            f.write((char*)&kf.sx,4); f.write((char*)&kf.sy,4); f.write((char*)&kf.sz,4);
        }
    }
    f.close();
    editor_modified = false;
    fprintf(stderr,"saved gussman.anim\n");
}

void editor_rot_bone(int bone, float ang, vec3 axis) {
    if (bone < 0 || bone >= g_num_bones || !g_data || !g_data->skins_count) return;
    cgltf_node* node = g_data->skins[0].joints[bone];
    float s = sinf(ang/2);
    float dx = axis.x * s, dy = axis.y * s, dz = axis.z * s, dw = cosf(ang/2);
    float qx = node->rotation[0], qy = node->rotation[1], qz = node->rotation[2], qw = node->rotation[3];
    float nx = dw*qx + dx*qw + dy*qz - dz*qy;
    float ny = dw*qy - dx*qz + dy*qw + dz*qx;
    float nz = dw*qz + dx*qy - dy*qx + dz*qw;
    float nw = dw*qw - dx*qx - dy*qy - dz*qz;
    float nl = sqrtf(nx*nx+ny*ny+nz*nz+nw*nw);
    if (nl > 0.0001f) { nl = 1.0f/nl; node->rotation[0]=nx*nl; node->rotation[1]=ny*nl; node->rotation[2]=nz*nl; node->rotation[3]=nw*nl; }
}

void editor_reset_bone(int bone) {
    if (bone < 0 || bone >= g_num_bones || !g_data || !g_data->skins_count) return;
    cgltf_node* node = g_data->skins[0].joints[bone];
    node->rotation[0]=0; node->rotation[1]=0; node->rotation[2]=0; node->rotation[3]=1;
    node->translation[0]=0; node->translation[1]=0; node->translation[2]=0;
    node->scale[0]=1; node->scale[1]=1; node->scale[2]=1;
}

void editor_insert_kf(float t) {
    if (!g_data || !g_data->skins_count || editor_ch.empty()) return;
    t = std::max(0.0f, t);
    for (auto& ch : editor_ch) {
        cgltf_node* node = g_data->skins[0].joints[ch.bone];
        EdKF kf;
        kf.t = t;
        kf.rx=node->rotation[0]; kf.ry=node->rotation[1]; kf.rz=node->rotation[2]; kf.rw=node->rotation[3];
        kf.tx=node->translation[0]; kf.ty=node->translation[1]; kf.tz=node->translation[2];
        kf.sx=node->scale[0]; kf.sy=node->scale[1]; kf.sz=node->scale[2];
        bool added = false;
        for (auto it = ch.frames.begin(); it != ch.frames.end(); ++it) {
            if (fabsf(it->t - t) < 0.001f) { *it = kf; added = true; break; }
            if (it->t > t) { ch.frames.insert(it, kf); added = true; break; }
        }
        if (!added) ch.frames.push_back(kf);
    }
    // Update duration if needed
    if (t >= editor_dur - 0.01f) editor_dur = t + 1;
    editor_modified = true;
}

void editor_apply_pose(float t) {
    if (!g_data || !g_data->skins_count) return;
    cgltf_skin* skin = &g_data->skins[0];

    float arm_save[10] = {0};
    if (g_armature_node) {
        arm_save[0]=g_armature_node->translation[0]; arm_save[1]=g_armature_node->translation[1]; arm_save[2]=g_armature_node->translation[2];
        arm_save[3]=g_armature_node->rotation[0]; arm_save[4]=g_armature_node->rotation[1]; arm_save[5]=g_armature_node->rotation[2]; arm_save[6]=g_armature_node->rotation[3];
        arm_save[7]=g_armature_node->scale[0]; arm_save[8]=g_armature_node->scale[1]; arm_save[9]=g_armature_node->scale[2];
    }

    for (int i=0; i<(int)g_data->nodes_count; i++) {
        cgltf_node* node = &g_data->nodes[i];
        node->translation[0] = node->translation[1] = node->translation[2] = 0;
        node->rotation[0] = node->rotation[1] = node->rotation[2] = 0; node->rotation[3] = 1;
        node->scale[0] = node->scale[1] = 1; node->scale[2] = 1;
    }

    for (auto& ch : editor_ch) {
        if (ch.frames.empty()) continue;
        cgltf_node* node = skin->joints[ch.bone];
        int nkf = (int)ch.frames.size();

        if (nkf == 1) {
            auto& kf = ch.frames[0];
            node->rotation[0]=kf.rx; node->rotation[1]=kf.ry; node->rotation[2]=kf.rz; node->rotation[3]=kf.rw;
            node->translation[0]=kf.tx; node->translation[1]=kf.ty; node->translation[2]=kf.tz;
            node->scale[0]=kf.sx; node->scale[1]=kf.sy; node->scale[2]=kf.sz;
            continue;
        }

        int idx = 0;
        for (int k=0; k<nkf-1; k++) {
            if (t >= ch.frames[k].t && t <= ch.frames[k+1].t) { idx = k; break; }
            if (k == nkf-2 && t > ch.frames[k+1].t) idx = nkf-2;
        }
        int idx2 = idx+1 < nkf ? idx+1 : idx;
        float t0 = ch.frames[idx].t, t1 = ch.frames[idx2].t;
        float frac = (t1 > t0) ? (t - t0) / (t1 - t0) : 0;

        auto& kf0 = ch.frames[idx];
        auto& kf1 = ch.frames[idx2];

        float ix = kf0.rx + (kf1.rx - kf0.rx)*frac;
        float iy = kf0.ry + (kf1.ry - kf0.ry)*frac;
        float iz = kf0.rz + (kf1.rz - kf0.rz)*frac;
        float iw = kf0.rw + (kf1.rw - kf0.rw)*frac;
        float il = sqrtf(ix*ix+iy*iy+iz*iz+iw*iw);
        if (il > 0.0001f) { il = 1.0f/il; ix*=il; iy*=il; iz*=il; iw*=il; }
        node->rotation[0]=ix; node->rotation[1]=iy; node->rotation[2]=iz; node->rotation[3]=iw;

        node->translation[0] = kf0.tx + (kf1.tx - kf0.tx)*frac;
        node->translation[1] = kf0.ty + (kf1.ty - kf0.ty)*frac;
        node->translation[2] = kf0.tz + (kf1.tz - kf0.tz)*frac;
        node->scale[0] = kf0.sx + (kf1.sx - kf0.sx)*frac;
        node->scale[1] = kf0.sy + (kf1.sy - kf0.sy)*frac;
        node->scale[2] = kf0.sz + (kf1.sz - kf0.sz)*frac;
    }
    if (g_armature_node) {
        g_armature_node->translation[0]=arm_save[0]; g_armature_node->translation[1]=arm_save[1]; g_armature_node->translation[2]=arm_save[2];
        g_armature_node->rotation[0]=arm_save[3]; g_armature_node->rotation[1]=arm_save[4]; g_armature_node->rotation[2]=arm_save[5]; g_armature_node->rotation[3]=arm_save[6];
        g_armature_node->scale[0]=arm_save[7]; g_armature_node->scale[1]=arm_save[8]; g_armature_node->scale[2]=arm_save[9];
    }
}

// ======================== SKIN COMPUTE ========================
void compute_skin() {
    cgltf_skin* skin = g_data->skins_count > 0 ? &g_data->skins[0] : nullptr;
    if (!skin) return;

    // Armature world matrix (first joint's parent chain root)
    mat4 armature_world = mat4::id();
    if (g_armature_node) {
        cgltf_node* a = g_armature_node;
        float qx=a->rotation[0], qy=a->rotation[1], qz=a->rotation[2], qw=a->rotation[3];
        float xx=qx*qx, yy=qy*qy, zz=qz*qz;
        float xy=qx*qy, xz=qx*qz, xw=qx*qw;
        float yz=qy*qz, yw=qy*qw, zw=qz*qw;
        armature_world.m[0]=1-2*(yy+zz); armature_world.m[4]=2*(xy-zw);   armature_world.m[8]=2*(xz+yw);
        armature_world.m[1]=2*(xy+zw);   armature_world.m[5]=1-2*(xx+zz); armature_world.m[9]=2*(yz-xw);
        armature_world.m[2]=2*(xz-yw);   armature_world.m[6]=2*(yz+xw);   armature_world.m[10]=1-2*(xx+yy);
        armature_world.m[12]=a->translation[0]; armature_world.m[13]=a->translation[1]; armature_world.m[14]=a->translation[2];
        float sx=a->scale[0], sy=a->scale[1], sz=a->scale[2];
        for (int c=0;c<3;c++) { armature_world.m[c]*=sx; armature_world.m[4+c]*=sy; armature_world.m[8+c]*=sz; }
    }

    for (int i=0; i<g_num_bones; i++) {
        cgltf_node* node = skin->joints[i];
        float qx=node->rotation[0], qy=node->rotation[1], qz=node->rotation[2], qw=node->rotation[3];
        float xx=qx*qx, yy=qy*qy, zz=qz*qz;
        float xy=qx*qy, xz=qx*qz, xw=qx*qw;
        float yz=qy*qz, yw=qy*qw, zw=qz*qw;
        mat4 local = mat4::id();
        local.m[0]=1-2*(yy+zz); local.m[4]=2*(xy-zw);   local.m[8]=2*(xz+yw);
        local.m[1]=2*(xy+zw);   local.m[5]=1-2*(xx+zz); local.m[9]=2*(yz-xw);
        local.m[2]=2*(xz-yw);   local.m[6]=2*(yz+xw);   local.m[10]=1-2*(xx+yy);
        local.m[12]=node->translation[0]; local.m[13]=node->translation[1]; local.m[14]=node->translation[2];
        float sx=node->scale[0], sy=node->scale[1], sz=node->scale[2];
        for (int c=0;c<3;c++) { local.m[c]*=sx; local.m[4+c]*=sy; local.m[8+c]*=sz; }

        int pi = g_bone_parent[i];
        if (pi >= 0) g_bone_world[i] = g_bone_world[pi] * local;
        else g_bone_world[i] = armature_world * local;
        g_bone_skin[i] = g_bone_world[i] * g_bone_invbind[i];
    }
}

// ======================== ANIMATION ========================
int cur_anim = 0;

void update_animation(float dt) {
    if (!g_data || !g_data->animations_count) { compute_skin(); return; }

    int idx = cur_anim;
    if (idx >= (int)g_data->animations_count) idx = 0;
    cgltf_animation* anim = &g_data->animations[idx];
    float max_t = 0;
    for (int i=0; i<(int)anim->samplers_count; i++) {
        cgltf_accessor* input = anim->samplers[i].input;
        float* tdata = (float*)((char*)input->buffer_view->buffer->data + input->buffer_view->offset + input->offset);
        int cnt = (int)input->count;
        // Last keyframe is a duplicate of the first (looping guard)
        float candidate = (cnt >= 2) ? tdata[cnt-2] : tdata[cnt-1];
        if (candidate > max_t) max_t = candidate;
    }

    anim_time += dt * anim_dir;
    if (max_t > 0.001f) {
        anim_time = fmodf(anim_time, max_t);
        if (anim_time < 0) anim_time += max_t;
    } else {
        if (anim_time < 0) anim_time = 0;
    }

    // Save armature node transform (not animated, must be preserved)
    float arm_save[10] = {0};
    if (g_armature_node) {
        arm_save[0]=g_armature_node->translation[0]; arm_save[1]=g_armature_node->translation[1]; arm_save[2]=g_armature_node->translation[2];
        arm_save[3]=g_armature_node->rotation[0]; arm_save[4]=g_armature_node->rotation[1]; arm_save[5]=g_armature_node->rotation[2]; arm_save[6]=g_armature_node->rotation[3];
        arm_save[7]=g_armature_node->scale[0]; arm_save[8]=g_armature_node->scale[1]; arm_save[9]=g_armature_node->scale[2];
    }

    for (int i=0; i<(int)g_data->nodes_count; i++) {
        cgltf_node* node = &g_data->nodes[i];
        node->translation[0] = node->translation[1] = node->translation[2] = 0;
        node->rotation[0] = node->rotation[1] = node->rotation[2] = 0; node->rotation[3] = 1;
        node->scale[0] = node->scale[1] = 1; node->scale[2] = 1;
    }

    // Restore armature node transform
    if (g_armature_node) {
        g_armature_node->translation[0]=arm_save[0]; g_armature_node->translation[1]=arm_save[1]; g_armature_node->translation[2]=arm_save[2];
        g_armature_node->rotation[0]=arm_save[3]; g_armature_node->rotation[1]=arm_save[4]; g_armature_node->rotation[2]=arm_save[5]; g_armature_node->rotation[3]=arm_save[6];
        g_armature_node->scale[0]=arm_save[7]; g_armature_node->scale[1]=arm_save[8]; g_armature_node->scale[2]=arm_save[9];
    }

    for (int i=0; i<(int)anim->channels_count; i++) {
        cgltf_animation_channel* ch = &anim->channels[i];
        cgltf_animation_sampler* samp = ch->sampler;
        cgltf_node* target = ch->target_node;
        if (!target) continue;

        cgltf_accessor* input = samp->input;
        cgltf_accessor* output = samp->output;
        float* tdata = (float*)((char*)input->buffer_view->buffer->data + input->buffer_view->offset + input->offset);
        float* vdata = (float*)((char*)output->buffer_view->buffer->data + output->buffer_view->offset + output->offset);
        int cnt = (int)input->count;

        int idx = 0;
        for (int k=0; k<cnt-1; k++) {
            if (anim_time >= tdata[k] && anim_time <= tdata[k+1]) { idx = k; break; }
            if (k == cnt-2 && anim_time > tdata[k+1]) idx = k;
        }

        float t0 = tdata[idx], t1 = tdata[idx+1 < cnt ? idx+1 : idx];
        float frac = (t1 > t0) ? (anim_time - t0) / (t1 - t0) : 0;

        if (ch->target_path == cgltf_animation_path_type_translation) {
            int vo = idx * 3;
            target->translation[0] = vdata[vo] + (vdata[vo+3]-vdata[vo])*frac;
            target->translation[1] = vdata[vo+1] + (vdata[vo+4]-vdata[vo+1])*frac;
            target->translation[2] = vdata[vo+2] + (vdata[vo+5]-vdata[vo+2])*frac;
        } else if (ch->target_path == cgltf_animation_path_type_rotation) {
            int vo = idx * 4;
            float ix = vdata[vo] + (vdata[vo+4]-vdata[vo])*frac;
            float iy = vdata[vo+1] + (vdata[vo+5]-vdata[vo+1])*frac;
            float iz = vdata[vo+2] + (vdata[vo+6]-vdata[vo+2])*frac;
            float iw = vdata[vo+3] + (vdata[vo+7]-vdata[vo+3])*frac;
            float il = sqrtf(ix*ix+iy*iy+iz*iz+iw*iw);
            if (il > 0.0001f) { il = 1.0f/il; ix*=il; iy*=il; iz*=il; iw*=il; }
            target->rotation[0]=ix; target->rotation[1]=iy;
            target->rotation[2]=iz; target->rotation[3]=iw;
        } else if (ch->target_path == cgltf_animation_path_type_scale) {
            int vo = idx * 3;
            target->scale[0] = vdata[vo] + (vdata[vo+3]-vdata[vo])*frac;
            target->scale[1] = vdata[vo+1] + (vdata[vo+4]-vdata[vo+1])*frac;
            target->scale[2] = vdata[vo+2] + (vdata[vo+5]-vdata[vo+2])*frac;
        }
    }

    compute_skin();
}

static float get_anim_duration() {
    if (!g_data || !g_data->animations_count) return 0;
    int idx = cur_anim;
    if (idx >= (int)g_data->animations_count) idx = 0;
    cgltf_animation* anim = &g_data->animations[idx];
    float max_t = 0;
    for (int i=0; i<(int)anim->samplers_count; i++) {
        cgltf_accessor* input = anim->samplers[i].input;
        float* tdata = (float*)((char*)input->buffer_view->buffer->data + input->buffer_view->offset + input->offset);
        int cnt = (int)input->count;
        float candidate = (cnt >= 2) ? tdata[cnt-2] : tdata[cnt-1];
        if (candidate > max_t) max_t = candidate;
    }
    return max_t > 0.001f ? max_t : 5.0f;
}

void bake_cloth_animation(int anim_idx) {
    if (anim_idx < 0 || anim_idx >= 4) return;
    int cm_idx = -1;
    for (int i=0; i<(int)g_char_meshes.size(); i++)
        if (g_char_meshes[i].is_cloth) { cm_idx = i; break; }
    if (cm_idx < 0) return;

    CharMesh& cm = g_char_meshes[cm_idx];
    int nv = (int)cm.cloth_pos.size();
    if (nv == 0) return;

    int saved_anim = cur_anim;
    cur_anim = anim_idx;
    float duration = get_anim_duration();
    cur_anim = saved_anim;
    if (duration < 0.001f) return;

    float fps = 30.0f;
    float dt = 1.0f / fps;
    int total_frames = (int)ceilf(duration * fps);
    int substeps = 6;

    float saved_time = anim_time;
    int saved_dir = anim_dir;
    anim_dir = 1;

    for (int vi=0; vi<nv; vi++) {
        cm.cloth_pos[vi] = cm.cloth_rest[vi];
        cm.cloth_vel[vi] = vec3(0,0,0);
    }

    fprintf(stderr, "baking cloth anim[%d]: %d frames, %.2fs...\n",
        anim_idx, total_frames, duration);

    int settle_frames = (int)(1.0f * fps);
    for (int f=0; f<settle_frames; f++) {
        anim_time = (float)f * dt;
        cur_anim = anim_idx;
        update_animation(0);
        float sub_dt = dt / substeps;
        for (int s=0; s<substeps; s++) {
            std::vector<vec3> target(nv);
            for (int vi=0; vi<nv; vi++) {
                vec3 rp = cm.cloth_rest[vi];
                float* jw = &cm.cloth_jw[vi*8];
                int j[4]={(int)jw[0],(int)jw[1],(int)jw[2],(int)jw[3]};
                float w[4]={jw[4],jw[5],jw[6],jw[7]};
                vec3 sp(0,0,0);
                for (int k=0;k<4;k++) {
                    if (w[k]<0.001f||j[k]<0||j[k]>=g_num_bones) continue;
                    mat4& m = g_bone_skin[j[k]];
                    sp.x += w[k]*(m.m[0]*rp.x+m.m[4]*rp.y+m.m[8]*rp.z+m.m[12]);
                    sp.y += w[k]*(m.m[1]*rp.x+m.m[5]*rp.y+m.m[9]*rp.z+m.m[13]);
                    sp.z += w[k]*(m.m[2]*rp.x+m.m[6]*rp.y+m.m[10]*rp.z+m.m[14]);
                }
                target[vi] = sp;
            }
            for (int vi=0; vi<nv; vi++) {
                if (cm.cloth_pinned[vi]) { cm.cloth_pos[vi]=target[vi]; cm.cloth_vel[vi]=vec3(0,0,0); }
                else {
                    cm.cloth_vel[vi] += (target[vi]-cm.cloth_pos[vi])*6.0f*sub_dt;
                    cm.cloth_vel[vi].y += -12.0f*sub_dt;
                    cm.cloth_vel[vi] = cm.cloth_vel[vi]*0.988f;
                    cm.cloth_pos[vi] += cm.cloth_vel[vi]*sub_dt;
                }
            }
            for (int it=0; it<4; it++)
                for (size_t si=0; si<cm.spring_i0.size(); si++) {
                    int a=cm.spring_i0[si],b=cm.spring_i1[si];
                    if (cm.cloth_pinned[a]&&cm.cloth_pinned[b]) continue;
                    vec3 d=cm.cloth_pos[b]-cm.cloth_pos[a];
                    float dist=d.len();
                    if (dist<0.0001f) continue;
                    float corr=(dist-cm.spring_rest[si])/dist*0.5f;
                    if (!cm.cloth_pinned[a]) cm.cloth_pos[a] += d*corr;
                    if (!cm.cloth_pinned[b]) cm.cloth_pos[b] -= d*corr;
                }
            for (int vi=0; vi<nv; vi++) {
                if (cm.cloth_pinned[vi]) continue;
                float* jw = &cm.cloth_jw[vi*8];
                vec3 col_push(0,0,0); int col_cnt=0;
                for (int k=0;k<4;k++) {
                    int bi=(int)jw[k];
                    if (bi<0||bi>=g_num_bones||jw[k+4]<0.001f) continue;
                    vec3 bpos(g_bone_world[bi].m[12],g_bone_world[bi].m[13],g_bone_world[bi].m[14]);
                    vec3 d=cm.cloth_pos[vi]-bpos;
                    float dist=d.len();
                    float bone_r=0.06f+jw[k+4]*0.08f;
                    if (dist<bone_r&&dist>0.001f) { col_push+=d*((bone_r-dist)/dist)*jw[k+4]; col_cnt++; }
                }
                if (col_cnt>0) cm.cloth_pos[vi]+=col_push*(0.5f/col_cnt);
                if (cm.cloth_pos[vi].y<0) cm.cloth_pos[vi].y=0;
            }
        }
    }

    BakedCloth& baked = g_baked[anim_idx];
    baked.frames.clear();
    baked.fps = fps;
    baked.duration = duration;
    baked.num_verts = nv;

    for (int f=0; f<total_frames; f++) {
        anim_time = (float)f * dt;
        cur_anim = anim_idx;
        update_animation(0);
        float sub_dt = dt / substeps;
        for (int s=0; s<substeps; s++) {
            std::vector<vec3> target(nv);
            for (int vi=0; vi<nv; vi++) {
                vec3 rp = cm.cloth_rest[vi];
                float* jw = &cm.cloth_jw[vi*8];
                int j[4]={(int)jw[0],(int)jw[1],(int)jw[2],(int)jw[3]};
                float w[4]={jw[4],jw[5],jw[6],jw[7]};
                vec3 sp(0,0,0);
                for (int k=0;k<4;k++) {
                    if (w[k]<0.001f||j[k]<0||j[k]>=g_num_bones) continue;
                    mat4& m = g_bone_skin[j[k]];
                    sp.x += w[k]*(m.m[0]*rp.x+m.m[4]*rp.y+m.m[8]*rp.z+m.m[12]);
                    sp.y += w[k]*(m.m[1]*rp.x+m.m[5]*rp.y+m.m[9]*rp.z+m.m[13]);
                    sp.z += w[k]*(m.m[2]*rp.x+m.m[6]*rp.y+m.m[10]*rp.z+m.m[14]);
                }
                target[vi] = sp;
            }
            for (int vi=0; vi<nv; vi++) {
                if (cm.cloth_pinned[vi]) { cm.cloth_pos[vi]=target[vi]; cm.cloth_vel[vi]=vec3(0,0,0); }
                else {
                    cm.cloth_vel[vi] += (target[vi]-cm.cloth_pos[vi])*6.0f*sub_dt;
                    cm.cloth_vel[vi].y += -12.0f*sub_dt;
                    cm.cloth_vel[vi] = cm.cloth_vel[vi]*0.988f;
                    cm.cloth_pos[vi] += cm.cloth_vel[vi]*sub_dt;
                }
            }
            for (int it=0; it<4; it++)
                for (size_t si=0; si<cm.spring_i0.size(); si++) {
                    int a=cm.spring_i0[si],b=cm.spring_i1[si];
                    if (cm.cloth_pinned[a]&&cm.cloth_pinned[b]) continue;
                    vec3 d=cm.cloth_pos[b]-cm.cloth_pos[a];
                    float dist=d.len();
                    if (dist<0.0001f) continue;
                    float corr=(dist-cm.spring_rest[si])/dist*0.5f;
                    if (!cm.cloth_pinned[a]) cm.cloth_pos[a] += d*corr;
                    if (!cm.cloth_pinned[b]) cm.cloth_pos[b] -= d*corr;
                }
            for (int vi=0; vi<nv; vi++) {
                if (cm.cloth_pinned[vi]) continue;
                float* jw = &cm.cloth_jw[vi*8];
                vec3 col_push(0,0,0); int col_cnt=0;
                for (int k=0;k<4;k++) {
                    int bi=(int)jw[k];
                    if (bi<0||bi>=g_num_bones||jw[k+4]<0.001f) continue;
                    vec3 bpos(g_bone_world[bi].m[12],g_bone_world[bi].m[13],g_bone_world[bi].m[14]);
                    vec3 d=cm.cloth_pos[vi]-bpos;
                    float dist=d.len();
                    float bone_r=0.06f+jw[k+4]*0.08f;
                    if (dist<bone_r&&dist>0.001f) { col_push+=d*((bone_r-dist)/dist)*jw[k+4]; col_cnt++; }
                }
                if (col_cnt>0) cm.cloth_pos[vi]+=col_push*(0.5f/col_cnt);
                if (cm.cloth_pos[vi].y<0) cm.cloth_pos[vi].y=0;
            }
        }
        baked.frames.push_back(cm.cloth_pos);
    }

    baked.ready = true;
    fprintf(stderr, "  baked anim[%d]: %zu frames\n", anim_idx, baked.frames.size());
    cur_anim = saved_anim;
    anim_time = saved_time;
    anim_dir = saved_dir;
    update_animation(0);
}

// ======================== SHADERS ========================
bool compile_shader() {
    auto c = [](GLuint t, const char* s) -> GLuint {
        GLuint sh = glCreateShader(t);
        glShaderSource(sh,1,&s,nullptr); glCompileShader(sh);
        GLint ok; glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);
        if (!ok) { char l[512]; glGetShaderInfoLog(sh,512,nullptr,l); fprintf(stderr,"S:%s\n",l); }
        return ok ? sh : 0;
    };
    const char* vs = "#version 330 core\nlayout(location=0)in vec3 aP;layout(location=1)in vec3 aN;layout(location=2)in vec3 aC;uniform mat4 uMVP,uM;uniform vec3 uLD;out vec3 vN,vC,vP;void main(){vec4 wp=uM*vec4(aP,1);vP=wp.xyz;vN=mat3(transpose(inverse(uM)))*aN;vC=aC;gl_Position=uMVP*vec4(aP,1);}";
    const char* fs = "#version 330 core\nin vec3 vN,vC,vP;uniform vec3 uVP,uLD;uniform float uAmb;uniform vec3 uCol;out vec4 F;void main(){vec3 col=vC*uCol;vec3 n=normalize(vN);vec3 ld=normalize(-uLD);float df=max(dot(n,ld),0);vec3 vd=normalize(uVP-vP);vec3 rd=reflect(ld,n);float sp=pow(max(dot(vd,rd),0),64);float sh=step(0.001,df);vec3 amb=uAmb*col;vec3 dif=df*col;vec3 spe=sh*sp*vec3(0.9,0.92,0.95);F=vec4(min(amb+dif+spe,vec3(1)),1);}";
    GLuint vs_h=c(GL_VERTEX_SHADER,vs),fs_h=c(GL_FRAGMENT_SHADER,fs);
    if (!vs_h||!fs_h) return false;
    shader=glCreateProgram();glAttachShader(shader,vs_h);glAttachShader(shader,fs_h);glLinkProgram(shader);
    GLint ok;glGetProgramiv(shader,GL_LINK_STATUS,&ok);glDeleteShader(vs_h);glDeleteShader(fs_h);
    return ok?true:false;
}

bool compile_char_shader() {
    auto c=[](GLuint t,const char*s)->GLuint{GLuint sh=glCreateShader(t);glShaderSource(sh,1,&s,nullptr);glCompileShader(sh);GLint ok;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);if(!ok){char l[512];glGetShaderInfoLog(sh,512,nullptr,l);fprintf(stderr,"CS:%s\n",l);return 0;}return sh;};
    const char* vs = "#version 330 core\n"
        "layout(location=0)in vec3 aP;layout(location=1)in vec3 aN;"
        "layout(location=2)in vec2 aU;layout(location=3)in vec4 aJ;layout(location=4)in vec4 aW;"
        "uniform mat4 uMVP,uM;uniform vec4 uBones[320];"
        "out vec3 vN,vP;out vec2 vU;"
        "void main(){"
        "mat4 bm=mat4(0);"
        "for(int i=0;i<4;i++){int bi=int(aJ[i]);float w=aW[i];"
        "for(int r=0;r<4;r++){int ci=bi*4+r;if(ci<320){bm[r]+=w*uBones[ci];}}"
        "}"
        "vec4 wp=bm*vec4(aP,1);vP=wp.xyz;vN=mat3(bm)*aN;vU=aU;gl_Position=uMVP*wp;}";
    const char*fs="#version 330 core\nin vec3 vN,vP;in vec2 vU;uniform vec3 uVP,uLD,uCol;uniform float uAmb;uniform sampler2D uTex;out vec4 F;void main(){vec3 col=uCol*texture(uTex,vU).rgb;vec3 n=normalize(vN);vec3 ld=normalize(-uLD);float df=max(dot(n,ld),0);vec3 vd=normalize(uVP-vP);vec3 rd=reflect(ld,n);float sp=pow(max(dot(vd,rd),0),32);float rim=pow(1-max(dot(n,vd),0),3)*0.3;F=vec4(col*(uAmb+df+rim)+sp*0.1,1);}";
    GLuint vs_h=c(GL_VERTEX_SHADER,vs),fs_h=c(GL_FRAGMENT_SHADER,fs);
    if(!vs_h||!fs_h)return false;
    char_shader=glCreateProgram();glAttachShader(char_shader,vs_h);glAttachShader(char_shader,fs_h);glLinkProgram(char_shader);
    GLint ok;glGetProgramiv(char_shader,GL_LINK_STATUS,&ok);glDeleteShader(vs_h);glDeleteShader(fs_h);
    return ok?true:false;
}

bool compile_cloth_shader() {
    auto c=[](GLuint t,const char*s)->GLuint{GLuint sh=glCreateShader(t);glShaderSource(sh,1,&s,nullptr);glCompileShader(sh);GLint ok;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);if(!ok){char l[512];glGetShaderInfoLog(sh,512,nullptr,l);fprintf(stderr,"KCS:%s\n",l);return 0;}return sh;};
    const char*vs="#version 330 core\nlayout(location=0)in vec3 aP;layout(location=1)in vec3 aN;layout(location=2)in vec2 aU;uniform mat4 uMVP,uM;out vec3 vN,vP;out vec2 vU;void main(){vec4 wp=uM*vec4(aP,1);vP=wp.xyz;vN=mat3(uM)*aN;vU=aU;gl_Position=uMVP*vec4(aP,1);}";
    const char*fs="#version 330 core\nin vec3 vN,vP;in vec2 vU;uniform vec3 uVP,uLD,uCol;uniform float uAmb;uniform sampler2D uTex;out vec4 F;void main(){vec3 col=uCol*texture(uTex,vU).rgb;vec3 n=normalize(vN);vec3 ld=normalize(-uLD);float df=max(dot(n,ld),0);vec3 vd=normalize(uVP-vP);vec3 rd=reflect(ld,n);float sp=pow(max(dot(vd,rd),0),32);float rim=pow(1-max(dot(n,vd),0),3)*0.3;F=vec4(col*(uAmb+df+rim)+sp*0.1,1);}";
    GLuint vs_h=c(GL_VERTEX_SHADER,vs),fs_h=c(GL_FRAGMENT_SHADER,fs);
    if(!vs_h||!fs_h)return false;
    cloth_shader=glCreateProgram();glAttachShader(cloth_shader,vs_h);glAttachShader(cloth_shader,fs_h);glLinkProgram(cloth_shader);
    GLint ok;glGetProgramiv(cloth_shader,GL_LINK_STATUS,&ok);glDeleteShader(vs_h);glDeleteShader(fs_h);
    return ok?true:false;
}

bool compile_text_shader() {
    auto c=[](GLuint t,const char*s)->GLuint{GLuint sh=glCreateShader(t);glShaderSource(sh,1,&s,nullptr);glCompileShader(sh);GLint ok;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);if(!ok){char l[512];glGetShaderInfoLog(sh,512,nullptr,l);fprintf(stderr,"TS:%s\n",l);return 0;}return sh;};
    const char*vs="#version 330 core\nlayout(location=0)in vec2 aP;layout(location=1)in vec2 aU;uniform mat4 uO;out vec2 vU;void main(){vU=aU;gl_Position=uO*vec4(aP,0,1);}";
    const char*fs="#version 330 core\nin vec2 vU;uniform sampler2D uT;uniform vec4 uC;out vec4 F;void main(){F=vec4(uC.rgb,texture(uT,vU).r*uC.a);}";
    GLuint v=c(GL_VERTEX_SHADER,vs),f=c(GL_FRAGMENT_SHADER,fs);
    if(!v||!f)return false;
    text_shader=glCreateProgram();glAttachShader(text_shader,v);glAttachShader(text_shader,f);glLinkProgram(text_shader);
    GLint ok;glGetProgramiv(text_shader,GL_LINK_STATUS,&ok);glDeleteShader(v);glDeleteShader(f);
    return ok?true:false;
}

void init_shadow() {
    auto c=[](GLuint t,const char*s)->GLuint{GLuint sh=glCreateShader(t);glShaderSource(sh,1,&s,nullptr);glCompileShader(sh);GLint ok;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);if(!ok){char l[512];glGetShaderInfoLog(sh,512,nullptr,l);fprintf(stderr,"SS:%s\n",l);return 0;}return sh;};
    const char*vs="#version 330 core\nlayout(location=0)in vec3 aP;uniform mat4 uMVP;void main(){gl_Position=uMVP*vec4(aP,1);}";
    const char*fs="#version 330 core\nuniform vec4 uColor;out vec4 F;void main(){F=uColor;}";
    GLuint v=c(GL_VERTEX_SHADER,vs),f=c(GL_FRAGMENT_SHADER,fs);
    if(!v||!f)return;
    shadow_shader=glCreateProgram();glAttachShader(shadow_shader,v);glAttachShader(shadow_shader,f);glLinkProgram(shadow_shader);
    GLint ok;glGetProgramiv(shadow_shader,GL_LINK_STATUS,&ok);glDeleteShader(v);glDeleteShader(f);
    if(!ok)fprintf(stderr,"shadow shader link failed\n");
}

// ======================== MESH GENERATION ========================
void gen_ground() {
    std::vector<float> v;
    int divs = 40; float half = 20.0f, step = 2.0f*half/divs;
    float gap = 0.04f;
    vec3 col(0.85f, 0.85f, 0.88f), n(0,1,0);
    for (int x=0;x<divs;x++) for (int z=0;z<divs;z++) {
        float x0=-half+x*step+gap, x1=-half+(x+1)*step-gap;
        float z0=-half+z*step+gap, z1=-half+(z+1)*step-gap;
        add_vert_vec(v, vec3(x0,0,z0), n, col);
        add_vert_vec(v, vec3(x1,0,z0), n, col);
        add_vert_vec(v, vec3(x1,0,z1), n, col);
        add_vert_vec(v, vec3(x0,0,z0), n, col);
        add_vert_vec(v, vec3(x1,0,z1), n, col);
        add_vert_vec(v, vec3(x0,0,z1), n, col);
    }
    build_mesh(ground_mesh,v);
}

void gen_sky_dome() {
    std::vector<float> v;
    int sl=24, st=8; float r=35;
    for (int j=0;j<st;j++) {
        float t0=(float)j/st*3.14159f/2,t1=(float)(j+1)/st*3.14159f/2;
        float r0=r*sinf(t0),r1=r*sinf(t1),y0=r*cosf(t0),y1=r*cosf(t1);
        float f0=(float)j/st,f1=(float)(j+1)/st;
        vec3 top(0.05f,0.08f,0.20f),bot(0.18f,0.22f,0.38f);
        vec3 c0=top+(bot-top)*f0,c1=top+(bot-top)*f1;
        vec3 n(0,1,0);
        for (int i=0;i<sl;i++) {
            float a0=6.283185f*i/sl,a1=6.283185f*(i+1)/sl;
            vec3 p00(r0*cosf(a0),y0,r0*sinf(a0)),p01(r0*cosf(a1),y0,r0*sinf(a1));
            vec3 p10(r1*cosf(a0),y1,r1*sinf(a0)),p11(r1*cosf(a1),y1,r1*sinf(a1));
            add_vert_vec(v,p00,n,c0); add_vert_vec(v,p01,n,c0);
            add_vert_vec(v,p11,n,c1);
            add_vert_vec(v,p00,n,c0); add_vert_vec(v,p11,n,c1);
            add_vert_vec(v,p10,n,c1);
        }
    }
    build_mesh(sky_dome_mesh,v);
}

void gen_shadow_disc() {
    std::vector<float> v;
    vec3 n(0,1,0), col(0.12f,0.14f,0.18f);
    int sl=20; float r=1.0f;
    for (int i=0;i<sl;i++) {
        float a0=6.283185f*i/sl,a1=6.283185f*(i+1)/sl;
        float d0[]={0,0,0,n.x,n.y,n.z,col.x,col.y,col.z};
        float d1[]={r*cosf(a0),0,r*sinf(a0),n.x,n.y,n.z,col.x,col.y,col.z};
        float d2[]={r*cosf(a1),0,r*sinf(a1),n.x,n.y,n.z,col.x,col.y,col.z};
        add_vert_9f(v,d0); add_vert_9f(v,d1); add_vert_9f(v,d2);
    }
    build_mesh(shadow_mesh,v);
}

// ======================== CAMERA ========================
void cam_update() {
    // Third-person camera: follows character, mouse orbits around it.
    cam_center = char_pos;
    cam_eye = cam_center + vec3(
        cam_dist*sinf(cam_theta)*cosf(cam_phi),
        cam_dist*sinf(cam_phi),
        cam_dist*cosf(cam_theta)*cosf(cam_phi)
    );
    if (cam_eye.y < 0.3f) cam_eye.y = 0.3f;
    cam_view = mat4::lookat(cam_eye, cam_center, {0,1,0});
    float ar = (float)fbW/fbH;
    cam_proj = mat4::persp(0.6f, ar, 0.1f, 150);
}

// ======================== FONT ========================
void bake_font(int px) {
    font_px = px;
    FILE* f = fopen("C:\\Windows\\Fonts\\arial.ttf", "rb");
    if (!f) f = fopen("C:\\Windows\\Fonts\\consola.ttf", "rb");
    if (!f) f = fopen("C:\\Windows\\Fonts\\segoeui.ttf", "rb");
    if (!f) { fprintf(stderr,"no font file found\n"); font_ready = false; return; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char* ttf = (unsigned char*)malloc(sz);
    if (!ttf) { fclose(f); return; }
    fread(ttf, 1, sz, f);
    fclose(f);

    stbtt_BakeFontBitmap(ttf, 0, (float)px, font_atlas, 512, 512, 32, 96, font_cdata);
    free(ttf);

    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, font_atlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    font_ready = true;
}

// ======================== TEXT RENDERING ========================
GLuint ui_shader = 0;

bool compile_ui_shader() {
    auto c=[](GLuint t,const char*s)->GLuint{GLuint sh=glCreateShader(t);glShaderSource(sh,1,&s,nullptr);glCompileShader(sh);GLint ok;glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);if(!ok){char l[512];glGetShaderInfoLog(sh,512,nullptr,l);fprintf(stderr,"UIS:%s\n",l);return 0;}return sh;};
    const char*vs="#version 330 core\nlayout(location=0)in vec2 aP;uniform mat4 uO;void main(){gl_Position=uO*vec4(aP,0,1);}";
    const char*fs="#version 330 core\nuniform vec4 uC;out vec4 F;void main(){F=uC;}";
    GLuint v=c(GL_VERTEX_SHADER,vs),f=c(GL_FRAGMENT_SHADER,fs);
    if(!v||!f)return false;
    ui_shader=glCreateProgram();glAttachShader(ui_shader,v);glAttachShader(ui_shader,f);glLinkProgram(ui_shader);
    GLint ok;glGetProgramiv(ui_shader,GL_LINK_STATUS,&ok);glDeleteShader(v);glDeleteShader(f);
    return ok?true:false;
}

void init_text_vbo() {
    glGenVertexArrays(1, &text_vao);
    glGenBuffers(1, &text_vbo);
    glBindVertexArray(text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8); glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &ui_vao);
    glGenBuffers(1, &ui_vbo);
    glBindVertexArray(ui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, (void*)0); glEnableVertexAttribArray(0);
}

void render_text(float x, float y, const char* text, vec3 col, float alpha=1) {
    if (!font_ready || !text_shader) return;
    struct TV { float px, py, tu, tv; };
    std::vector<TV> verts;
    float ox = x;
    while (*text) {
        if (*text == '\n') { x = ox; y += font_px; text++; continue; }
        if ((unsigned char)*text < 32) { text++; continue; }
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(font_cdata, 512, 512, *text-32, &x, &y, &q, 1);
        float tx0 = q.x0, ty0 = q.y0, tx1 = q.x1, ty1 = q.y1;
        float u0 = q.x0/512.0f, v0 = 1.0f - q.y1/512.0f;
        float u1 = q.x1/512.0f, v1 = 1.0f - q.y0/512.0f;
        verts.push_back({tx0, ty0, u0, v1});
        verts.push_back({tx1, ty0, u1, v1});
        verts.push_back({tx1, ty1, u1, v0});
        verts.push_back({tx0, ty0, u0, v1});
        verts.push_back({tx1, ty1, u1, v0});
        verts.push_back({tx0, ty1, u0, v0});
        text++;
    }
    if (verts.empty()) return;

    glUseProgram(text_shader);
    float l=0, r=(float)fbW, b=(float)fbH, t=0;
    float om[16]={2/(r-l),0,0,0, 0,2/(t-b),0,0, 0,0,-1,0, (r+l)/(l-r),(t+b)/(b-t),0,1};
    glUniformMatrix4fv(glGetUniformLocation(text_shader,"uO"),1,GL_FALSE,om);
    glUniform4f(glGetUniformLocation(text_shader,"uC"),col.x,col.y,col.z,alpha);
    glUniform1i(glGetUniformLocation(text_shader,"uT"),0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, font_tex);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(TV), verts.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, (int)verts.size());
    glDisable(GL_BLEND);
}

void ortho_mat4(float* m, int w, int h) {
    float l=0,r=(float)w,b=(float)h,t=0;
    m[0]=2/(r-l);m[1]=0;m[2]=0;m[3]=0;
    m[4]=0;m[5]=2/(t-b);m[6]=0;m[7]=0;
    m[8]=0;m[9]=0;m[10]=-1;m[11]=0;
    m[12]=(r+l)/(l-r);m[13]=(t+b)/(b-t);m[14]=0;m[15]=1;
}

void draw_rect(float x, float y, float w, float h, vec3 col, float alpha=1) {
    if (!ui_shader) return;
    float verts[8] = {x,y, x+w,y, x+w,y+h, x,y+h};
    glUseProgram(ui_shader);
    float om[16]; ortho_mat4(om, fbW, fbH);
    glUniformMatrix4fv(glGetUniformLocation(ui_shader,"uO"),1,GL_FALSE,om);
    glUniform4f(glGetUniformLocation(ui_shader,"uC"),col.x,col.y,col.z,alpha);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(ui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
    glBufferData(GL_ARRAY_BUFFER, 8*4, verts, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);
}

void draw_line(float ax,float ay,float bx,float by,vec3 col,float alpha=1) {
    if (!ui_shader) return;
    float dx=bx-ax, dy=by-ay;
    float lx=-dy, ly=dx;
    float ll=sqrtf(lx*lx+ly*ly);
    if (ll<0.001f) return;
    lx/=ll; ly/=ll;
    float hw=0.5f;
    float verts[8] = {ax+lx*hw,ay+ly*hw, bx+lx*hw,by+ly*hw, bx-lx*hw,by-ly*hw, ax-lx*hw,ay-ly*hw};
    glUseProgram(ui_shader);
    float om[16]; ortho_mat4(om, fbW, fbH);
    glUniformMatrix4fv(glGetUniformLocation(ui_shader,"uO"),1,GL_FALSE,om);
    glUniform4f(glGetUniformLocation(ui_shader,"uC"),col.x,col.y,col.z,alpha);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(ui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo);
    glBufferData(GL_ARRAY_BUFFER, 8*4, verts, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_BLEND);
}



// ======================== RENDER ========================
void render_mesh(const Mesh& m, const mat4& model) {
    if (!m.count) return;
    mat4 mvp = cam_proj * cam_view * model;
    glUniformMatrix4fv(glGetUniformLocation(shader,"uMVP"),1,GL_FALSE,mvp.m);
    glUniformMatrix4fv(glGetUniformLocation(shader,"uM"),1,GL_FALSE,model.m);
    glBindVertexArray(m.vao);
    glDrawArrays(GL_TRIANGLES, 0, m.count);
}

void render_character() {
    if (!char_shader || !g_has_skin || g_num_bones < 1) return;
    glUseProgram(char_shader);

    int max_bones = g_num_bones > 80 ? 80 : g_num_bones;
    float bone_uniforms[80*16];
    for (int i=0; i<max_bones; i++) {
        for (int c=0; c<4; c++) {
            bone_uniforms[i*16 + c*4 + 0] = g_bone_skin[i].m[c*4+0];
            bone_uniforms[i*16 + c*4 + 1] = g_bone_skin[i].m[c*4+1];
            bone_uniforms[i*16 + c*4 + 2] = g_bone_skin[i].m[c*4+2];
            bone_uniforms[i*16 + c*4 + 3] = g_bone_skin[i].m[c*4+3];
        }
    }
    glUniform4fv(glGetUniformLocation(char_shader,"uBones"),max_bones*4,bone_uniforms);

    mat4 char_model = mat4::trans(char_pos) * mat4::rot(char_rot, vec3(0,1,0));
    mat4 mvp = cam_proj * cam_view * char_model;
    glUniformMatrix4fv(glGetUniformLocation(char_shader,"uMVP"),1,GL_FALSE,mvp.m);
    glUniformMatrix4fv(glGetUniformLocation(char_shader,"uM"),1,GL_FALSE,char_model.m);
    glUniform3f(glGetUniformLocation(char_shader,"uVP"),cam_eye.x,cam_eye.y,cam_eye.z);
    glUniform3f(glGetUniformLocation(char_shader,"uLD"),-0.4f,-0.8f,-0.3f);
    glUniform1f(glGetUniformLocation(char_shader,"uAmb"),0.3f);
    glUniform1i(glGetUniformLocation(char_shader,"uTex"),0);

    // Render outer layers (clothes/hair) first so they write depth.
    // Body last with polygon offset to prevent z-fighting (same skeleton).
    glEnable(GL_POLYGON_OFFSET_FILL);
    for (int pass = 0; pass < 2; pass++) {
        for (auto& cm : g_char_meshes) {
            if (!cm.valid || cm.is_cloth) continue;
            if ((pass == 0) == (cm.material_index == 0)) continue;
            if (pass == 1) glPolygonOffset(1.0f, 1.0f);
            else glPolygonOffset(0, 0);
            int mat_idx = cm.material_index;
            if (mat_idx < 0 || mat_idx >= 16) mat_idx = 0;
            vec3 col = g_material_colors[mat_idx];
            glUniform3f(glGetUniformLocation(char_shader,"uCol"), col.x, col.y, col.z);
            GLuint tex = g_material_textures[mat_idx];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBindVertexArray(cm.vao);
            glDrawElements(GL_TRIANGLES, cm.num_indices, GL_UNSIGNED_SHORT, 0);
        }
    }
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Render cloth meshes via cloth_shader (CPU-skinned, physics-simulated)
    if (cloth_shader) {
        glUseProgram(cloth_shader);
        mat4 mvp = cam_proj * cam_view * char_model;
        glUniformMatrix4fv(glGetUniformLocation(cloth_shader,"uMVP"),1,GL_FALSE,mvp.m);
        glUniformMatrix4fv(glGetUniformLocation(cloth_shader,"uM"),1,GL_FALSE,char_model.m);
        glUniform3f(glGetUniformLocation(cloth_shader,"uVP"),cam_eye.x,cam_eye.y,cam_eye.z);
        glUniform3f(glGetUniformLocation(cloth_shader,"uLD"),-0.4f,-0.8f,-0.3f);
        glUniform1f(glGetUniformLocation(cloth_shader,"uAmb"),0.3f);
        glUniform1i(glGetUniformLocation(cloth_shader,"uTex"),0);
        for (auto& cm : g_char_meshes) {
            if (!cm.valid || !cm.is_cloth || !cm.cloth_vao) continue;
            int mat_idx = cm.material_index;
            if (mat_idx < 0 || mat_idx >= 16) mat_idx = 0;
            vec3 col = g_material_colors[mat_idx];
            glUniform3f(glGetUniformLocation(cloth_shader,"uCol"), col.x, col.y, col.z);
            GLuint tex = g_material_textures[mat_idx];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBindVertexArray(cm.cloth_vao);
            glDrawElements(GL_TRIANGLES, cm.num_indices, GL_UNSIGNED_SHORT, 0);
        }
    }

    // Draw bones as lines (editor mode)
    if (editor_mode && g_data && g_data->skins_count > 0) {
        cgltf_skin* skin = &g_data->skins[0];
        std::vector<float> bline;
        for (int i=0; i<g_num_bones; i++) {
            vec3 p0(g_bone_world[i].m[12], g_bone_world[i].m[13], g_bone_world[i].m[14]);
            int pi = g_bone_parent[i];
            if (pi >= 0) {
                vec3 p1(g_bone_world[pi].m[12], g_bone_world[pi].m[13], g_bone_world[pi].m[14]);
                vec3 wp0 = transform_pos(char_model, p0);
                vec3 wp1 = transform_pos(char_model, p1);
                bline.push_back(wp0.x); bline.push_back(wp0.y); bline.push_back(wp0.z);
                bline.push_back(wp1.x); bline.push_back(wp1.y); bline.push_back(wp1.z);
            }
        }
        if (!bline.empty()) {
            GLuint bone_vao, bone_vbo;
            glGenVertexArrays(1, &bone_vao);
            glGenBuffers(1, &bone_vbo);
            glBindVertexArray(bone_vao);
            glBindBuffer(GL_ARRAY_BUFFER, bone_vbo);
            glBufferData(GL_ARRAY_BUFFER, bline.size()*4, bline.data(), GL_STREAM_DRAW);
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,(void*)0); glEnableVertexAttribArray(0);
            glVertexAttrib3f(1, 0, 1, 0); // default normal (avoid NaN)
            glVertexAttrib3f(2, 1, 1, 1); // default color

            glUseProgram(shader);
            mat4 bmvp = cam_proj * cam_view;
            glUniformMatrix4fv(glGetUniformLocation(shader,"uMVP"),1,GL_FALSE,bmvp.m);
            glUniformMatrix4fv(glGetUniformLocation(shader,"uM"),1,GL_FALSE,mat4::id().m);
            glUniform3f(glGetUniformLocation(shader,"uLD"),-0.4f,-0.8f,-0.3f);
            glUniform1f(glGetUniformLocation(shader,"uAmb"),1);
            glUniform3f(glGetUniformLocation(shader,"uCol"),0,1,0);
            glDisable(GL_DEPTH_TEST);
            glBindVertexArray(bone_vao);
            glDrawArrays(GL_LINES, 0, (int)bline.size()/3);

            // Draw selected bone in red
            if (editor_sel >= 0 && editor_sel < g_num_bones) {
                vec3 p0(g_bone_world[editor_sel].m[12], g_bone_world[editor_sel].m[13], g_bone_world[editor_sel].m[14]);
                vec3 wp0 = transform_pos(char_model, p0);
                int pi = g_bone_parent[editor_sel];
                if (pi >= 0) {
                    vec3 p1(g_bone_world[pi].m[12], g_bone_world[pi].m[13], g_bone_world[pi].m[14]);
                    vec3 wp1 = transform_pos(char_model, p1);
                    float sverts[] = {wp0.x,wp0.y,wp0.z, wp1.x,wp1.y,wp1.z};
                    glBindBuffer(GL_ARRAY_BUFFER, bone_vbo);
                    glBufferData(GL_ARRAY_BUFFER, 6*4, sverts, GL_STREAM_DRAW);
                    glUniform3f(glGetUniformLocation(shader,"uCol"),1,0,0);
                    glDrawArrays(GL_LINES, 0, 2);
                }
                // Draw small cross at selected bone position
                float csz = 0.08f;
                float cross[] = {
                    wp0.x-csz,wp0.y,wp0.z, wp0.x+csz,wp0.y,wp0.z,
                    wp0.x,wp0.y-csz,wp0.z, wp0.x,wp0.y+csz,wp0.z,
                    wp0.x,wp0.y,wp0.z-csz, wp0.x,wp0.y,wp0.z+csz,
                };
                glBindBuffer(GL_ARRAY_BUFFER, bone_vbo);
                glBufferData(GL_ARRAY_BUFFER, 18*4, cross, GL_STREAM_DRAW);
                glUniform3f(glGetUniformLocation(shader,"uCol"),1,0.5f,0);
                glDrawArrays(GL_LINES, 0, 6);
            }
            glEnable(GL_DEPTH_TEST);
            glUseProgram(char_shader);
            glDeleteVertexArrays(1, &bone_vao);
            glDeleteBuffers(1, &bone_vbo);
        }
    }
    glUseProgram(shader);
}

void render() {
    if (!shader) return;
    if (editor_mode) {
        // In editor mode, apply pose from keyframes
        if (editor_play) {
            editor_time += 0.016f * editor_spd;
            if (editor_time > editor_dur) editor_time = 0;
        }
        editor_apply_pose(editor_time);
        compute_skin();
    }
    cam_update();
    glViewport(0,0,fbW,fbH);
    glClearColor(0.08f,0.10f,0.18f,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader,"uVP"),cam_eye.x,cam_eye.y,cam_eye.z);

    // Sky dome
    glDisable(GL_DEPTH_TEST);
    glUniform3f(glGetUniformLocation(shader,"uLD"),0,-1,0);
    glUniform1f(glGetUniformLocation(shader,"uAmb"),0.65f);
    glUniform3f(glGetUniformLocation(shader,"uCol"),1,1,1);
    render_mesh(sky_dome_mesh, mat4::trans(vec3(cam_center.x,0,cam_center.z)));
    glEnable(GL_DEPTH_TEST);

    // Scene lighting
    glUniform3f(glGetUniformLocation(shader,"uLD"),-0.4f,-0.8f,-0.3f);
    glUniform1f(glGetUniformLocation(shader,"uAmb"),0.22f);
    glUniform3f(glGetUniformLocation(shader,"uCol"),1,1,1);

    // Ground
    render_mesh(ground_mesh, mat4::id());

    // Shadow disc
    if (shadow_shader) {
        glUseProgram(shadow_shader);
        mat4 sm = mat4::trans(vec3(char_pos.x,0.002f,char_pos.z)) * mat4::scale(vec3(1.2f,1,1.2f));
        mat4 smvp = cam_proj * cam_view * sm;
        glUniformMatrix4fv(glGetUniformLocation(shadow_shader,"uMVP"),1,GL_FALSE,smvp.m);
        glUniform4f(glGetUniformLocation(shadow_shader,"uColor"),0,0,0,0.3f);
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
        glBindVertexArray(shadow_mesh.vao);
        glDrawArrays(GL_TRIANGLES,0,shadow_mesh.count);
        glDepthMask(GL_TRUE); glDisable(GL_BLEND);
        glUseProgram(shader);
    }

    // Character (test cube or GLB)
    render_character();

    // Editor UI overlay
    if (editor_mode) {
        glDisable(GL_DEPTH_TEST);
        // Semi-transparent dark panel at left
        draw_rect(0, 0, 160, (float)fbH, vec3(0,0,0), 0.7f);
        // Bone list
        for (int i=0; i<g_num_bones; i++) {
            char label[32];
            if (editor_sel == i)
                snprintf(label,32,"> %d: Bone.%03d", i, i);
            else
                snprintf(label,32,"  %d: Bone.%03d", i, i);
            vec3 tc = (editor_sel == i) ? vec3(1,0.8f,0) : vec3(0.8f,0.8f,0.8f);
            render_text(8, (float)(8 + i*22), label, tc);
        }
        // Timeline at bottom
        float tl_y = (float)fbH - 50;
        draw_rect(0, tl_y, (float)fbW, 50, vec3(0,0,0), 0.7f);
        // Timeline bar
        float bar_l = 80, bar_r = (float)fbW - 80;
        draw_line(bar_l, tl_y+35, bar_r, tl_y+35, vec3(0.5f,0.5f,0.5f), 0.8f);
        // Keyframe marks
        for (auto& ch : editor_ch) {
            for (auto& kf : ch.frames) {
                float kx = bar_l + (kf.t / (editor_dur > 0.01f ? editor_dur : 1)) * (bar_r - bar_l);
                draw_line(kx, tl_y+30, kx, tl_y+40, vec3(1,1,1), 0.5f);
            }
        }
        // Current time marker
        float cx = bar_l + (editor_time / (editor_dur > 0.01f ? editor_dur : 1)) * (bar_r - bar_l);
        draw_line(cx, tl_y+25, cx, tl_y+45, vec3(0,1,0), 1);

        // Status text
        char status[128];
        snprintf(status,128,"Time: %.2fs / %.2fs  Bone: %d  %s",
            editor_time, editor_dur, editor_sel,
            editor_play ? "PLAYING" : "STOPPED");
        render_text(bar_l, tl_y+5, status, vec3(1,1,1));

        // Help text above timeline
        char help[256];
        snprintf(help,256,
            "[W/A/S/D/Q/E] Rotate  [K] Keyframe  [Space] Play  [1-9,0,-,=] Bone  [R] Reset  [F1] Exit Editor  [Ctrl+S] Save  [Ctrl+L] Load");
        render_text(bar_l, tl_y-18, help, vec3(0.7f,0.7f,0.7f));
    }

    // HUD text (viewer mode)
    if (!editor_mode) {
        render_text(10, 10, "[F1] Animation Editor", vec3(0.6f,0.6f,0.6f), 0.6f);
    }

    glEnable(GL_DEPTH_TEST);
}

// ======================== INPUT ========================
bool mouse_captured = true;
double last_mx=0,last_my=0;
bool first_mouse=true;

void toggle_mouse() {
    mouse_captured=!mouse_captured;
    if(mouse_captured)glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
}

void on_key(GLFWwindow* win,int key,int sc,int action,int mods) {
    ImGui_ImplGlfw_KeyCallback(win,key,sc,action,mods);
    if (ImGui::GetIO().WantCaptureKeyboard) return;
    // Editor mode toggle (F1)
    if (key==GLFW_KEY_F1 && action==GLFW_PRESS) {
        editor_mode = !editor_mode;
        if (editor_mode) fprintf(stderr,"EDITOR MODE\n");
        else fprintf(stderr,"VIEWER MODE\n");
        return;
    }

    // Editor mode keys
    if (editor_mode && action!=GLFW_RELEASE) {
        // Bone selection
        int bone = -1;
        if (key>=GLFW_KEY_1 && key<=GLFW_KEY_9) bone = key - GLFW_KEY_1;
        else if (key==GLFW_KEY_0) bone = 9;
        else if (key==GLFW_KEY_MINUS) bone = 10;
        else if (key==GLFW_KEY_EQUAL) bone = 11;

        if (bone >= 0 && bone < g_num_bones) { editor_sel = bone; return; }
        if (key==GLFW_KEY_RIGHT_BRACKET && action==GLFW_PRESS) { editor_sel = (editor_sel+1)%g_num_bones; return; }
        if (key==GLFW_KEY_LEFT_BRACKET && action==GLFW_PRESS) { editor_sel = (editor_sel-1+g_num_bones)%g_num_bones; return; }

        // Bone rotation
        float ang = (mods & GLFW_MOD_SHIFT) ? 0.01f : 0.05f;
        if (mods & GLFW_MOD_CONTROL) ang = 0.2f;
        bool rot = true;
        if (key==GLFW_KEY_W) editor_rot_bone(editor_sel, ang, vec3(1,0,0));
        else if (key==GLFW_KEY_S) editor_rot_bone(editor_sel, -ang, vec3(1,0,0));
        else if (key==GLFW_KEY_A) editor_rot_bone(editor_sel, ang, vec3(0,1,0));
        else if (key==GLFW_KEY_D) editor_rot_bone(editor_sel, -ang, vec3(0,1,0));
        else if (key==GLFW_KEY_Q) editor_rot_bone(editor_sel, ang, vec3(0,0,1));
        else if (key==GLFW_KEY_E) editor_rot_bone(editor_sel, -ang, vec3(0,0,1));
        else if (key==GLFW_KEY_R && action==GLFW_PRESS) editor_reset_bone(editor_sel);
        else if (key==GLFW_KEY_K && action==GLFW_PRESS) editor_insert_kf(editor_time);
        else if (key==GLFW_KEY_SPACE && action==GLFW_PRESS) editor_play = !editor_play;
        else if (key==GLFW_KEY_RIGHT && action==GLFW_PRESS) {
            // Step to next keyframe across all channels
            float next_t = editor_dur;
            for (auto& ch : editor_ch)
                for (auto& kf : ch.frames)
                    if (kf.t > editor_time + 0.001f && kf.t < next_t) next_t = kf.t;
            if (next_t < editor_dur) editor_time = next_t;
        }
        else if (key==GLFW_KEY_LEFT && action==GLFW_PRESS) {
            float prev_t = -1;
            for (auto& ch : editor_ch)
                for (auto& kf : ch.frames)
                    if (kf.t < editor_time - 0.001f && kf.t > prev_t) prev_t = kf.t;
            if (prev_t >= 0) editor_time = prev_t;
        }
        else if (key==GLFW_KEY_S && (mods & GLFW_MOD_CONTROL) && action==GLFW_PRESS) editor_save();
        else if (key==GLFW_KEY_L && (mods & GLFW_MOD_CONTROL) && action==GLFW_PRESS) { editor_init(); }
        else rot = false;

        if (rot) return; // Don't process viewer keys when rotating
    }

    // Viewer mode keys
    if(key==GLFW_KEY_W||key==GLFW_KEY_UP)w_pressed=(action==GLFW_PRESS||action==GLFW_REPEAT);
    if(key==GLFW_KEY_S||key==GLFW_KEY_DOWN)s_pressed=(action==GLFW_PRESS||action==GLFW_REPEAT);
    if(key==GLFW_KEY_A||key==GLFW_KEY_LEFT)a_pressed=(action==GLFW_PRESS||action==GLFW_REPEAT);
    if(key==GLFW_KEY_D||key==GLFW_KEY_RIGHT)d_pressed=(action==GLFW_PRESS||action==GLFW_REPEAT);
    if(key==GLFW_KEY_SPACE)space_pressed=(action==GLFW_PRESS||action==GLFW_REPEAT);

    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS){if(mouse_captured)toggle_mouse();else glfwSetWindowShouldClose(window,1);}
}

void on_cursor(GLFWwindow* win,double x,double y) {
    ImGui_ImplGlfw_CursorPosCallback(win,x,y);
    if (ImGui::GetIO().WantCaptureMouse) return;
    if(!mouse_captured)return;
    if(first_mouse){last_mx=x;last_my=y;first_mouse=false;return;}
    double dx=x-last_mx,dy=y-last_my;last_mx=x;last_my=y;
    cam_theta+=(float)dx*0.005f;cam_phi+=(float)dy*0.005f;
    if(cam_phi>1.4f)cam_phi=1.4f;if(cam_phi<0.1f)cam_phi=0.1f;
}

void on_mouse(GLFWwindow* win,int button,int action,int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(win,button,action,mods);
    if (ImGui::GetIO().WantCaptureMouse) return;
    if(button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS&&!mouse_captured)toggle_mouse();
}

void on_char(GLFWwindow* win,unsigned int c) {
    ImGui_ImplGlfw_CharCallback(win,c);
}

void on_scroll(GLFWwindow* win,double dx,double dy) {
    ImGui_ImplGlfw_ScrollCallback(win,dx,dy);
    if (ImGui::GetIO().WantCaptureMouse) return;
    cam_dist*=(float)(1.0-dy*0.06);if(cam_dist<3)cam_dist=3;if(cam_dist>40)cam_dist=40;
}

// ======================== PHYSICS ========================
vec3 char_gravity(0, -20, 0);
bool char_grounded = false;

void update_physics(float dt) {
    if(dt>0.05f)dt=0.05f;

    if (!editor_mode) {
        float speed = 4.0f;
        vec3 input(0,0,0);
        if(w_pressed)input.z-=1;
        if(s_pressed)input.z+=1;
        if(a_pressed)input.x-=1;
        if(d_pressed)input.x+=1;
        float ilen = input.len();
        if (ilen > 1) input = input * (1.0f / ilen);

        float ct=cosf(cam_theta),st=sinf(cam_theta);
        vec3 wish_dir(input.x*ct+input.z*st,0,input.z*ct-input.x*st);

        // Save previous position for distance-based animation
        float prev_x = char_pos.x, prev_z = char_pos.z;

        // Instant movement (like MarbleMania ball)
        if (ilen > 0.01f) {
            char_pos += wish_dir * speed * dt;
            char_rot_target = atan2f(wish_dir.x, wish_dir.z);
        }

        // Smooth rotation
        float rot_diff = char_rot_target - char_rot;
        if (rot_diff > 3.14159f) rot_diff -= 6.28318f;
        if (rot_diff < -3.14159f) rot_diff += 6.28318f;
        char_rot += rot_diff * std::min(1.0f, 8.0f * dt);

        // Animation driven by actual distance traveled
        {
            float dx = char_pos.x - prev_x, dz = char_pos.z - prev_z;
            float dist_moved = sqrtf(dx*dx + dz*dz);
            bool moving = dist_moved > 0.001f;
            static bool stopping = false;

            // Walk animation duration (for neutral-pose detection)
            float walk_max_t = 0;
            if (g_data && g_data->animations_count > 1) {
                cgltf_animation* w = &g_data->animations[1];
                for (int i=0; i<(int)w->samplers_count; i++) {
                    cgltf_accessor* in = w->samplers[i].input;
                    float* td = (float*)((char*)in->buffer_view->buffer->data + in->buffer_view->offset + in->offset);
                    int cnt = (int)in->count;
                    float c = (cnt >= 2) ? td[cnt-2] : td[cnt-1];
                    if (c > walk_max_t) walk_max_t = c;
                }
            }

            // Natural stop: when stopping, fast-forward walk to neutral pose
            if (g_data && g_data->animations_count > 1) {
                if (moving) {
                    cur_anim = 1;
                    stopping = false;
                } else if (cur_anim == 1 && !stopping) {
                    stopping = true;
                } else if (cur_anim == 0) {
                    stopping = false;
                }
            }

            if (moving) {
                anim_dir = s_pressed ? -1 : 1;
            } else if (stopping) {
                anim_dir = 1; // keep walk forward to reach neutral
            } else if (!editor_play) {
                anim_dir = 0;
            }

            float anim_dt = dist_moved * 0.67f;
            if (stopping) anim_dt = dt * 4.0f; // fast-forward to neutral
            if (anim_dt < 0.0001f && editor_play) anim_dt = dt;

            update_animation(anim_dir != 0 || editor_play ? anim_dt : 0);

            // When walk wraps to near 0, we're at neutral (both feet on ground)
            if (stopping && (anim_time < 0.03f || (walk_max_t > 0 && anim_time > walk_max_t - 0.03f))) {
                stopping = false;
                if (g_data && g_data->animations_count > 1) cur_anim = 0;
                anim_dir = 0;
                anim_time = 0;
            }
        }
    } else {
        w_pressed = s_pressed = a_pressed = d_pressed = false;
        if (g_data && g_data->animations_count > 1) cur_anim = 0;
        if (editor_play) update_animation(dt);
    }
}

// ======================== MAIN ========================
static void save_bmp(const char* path, int w, int h, unsigned char* rgb) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    unsigned char hdr[54] = {0};
    hdr[0]='B'; hdr[1]='M';
    unsigned int sz = 54 + w*h*3;
    hdr[2]=sz&0xFF; hdr[3]=(sz>>8)&0xFF; hdr[4]=(sz>>16)&0xFF; hdr[5]=(sz>>24)&0xFF;
    hdr[10]=54;
    hdr[14]=40;
    *(int*)&hdr[18]=w; *(int*)&hdr[22]=h;
    hdr[26]=1; hdr[28]=24;
    fwrite(hdr,1,54,f);
    // BMP is BGR, bottom-up
    for (int y=h-1;y>=0;y--) {
        for (int x=0;x<w;x++) {
            unsigned char* p = rgb + (y*w + x)*3;
            unsigned char bgr[3] = {p[2], p[1], p[0]};
            fwrite(bgr,1,3,f);
        }
        unsigned char pad[3]={0,0,0};
        fwrite(pad,1,(4-(w*3)%4)%4,f);
    }
    fclose(f);
    fprintf(stderr,"saved screenshot: %s (%dx%d)\n", path, w, h);
}

int main() {
    bool screenshot_mode = false;
    const char* screenshot_path = "screenshot.bmp";
    const char* glb_path = "animated_character.glb";
    for (int i=1;i<__argc;i++) {
        if (!strcmp(__argv[i],"--screenshot")) {
            screenshot_mode = true;
            if (i+1 < __argc && __argv[i+1][0] != '-') screenshot_path = __argv[++i];
        } else if (__argv[i][0] != '-') {
            glb_path = __argv[i];
        }
    }

    if(!glfwInit()){fprintf(stderr,"glfwInit failed\n");return -1;}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS,24);
    if (screenshot_mode) glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    window=glfwCreateWindow(fbW,fbH,"Character",nullptr,nullptr);
    if(!window){glfwTerminate();return -1;}
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    if(glewInit()!=GLEW_OK){fprintf(stderr,"glewInit failed\n");return -1;}
    if(!compile_shader()){fprintf(stderr,"main shader failed\n");return -1;}
    if(!compile_char_shader()){fprintf(stderr,"char shader failed\n");return -1;}
    if(!compile_cloth_shader()){fprintf(stderr,"cloth shader failed\n");return -1;}
    if(!compile_text_shader()){fprintf(stderr,"text shader failed\n");return -1;}
    if(!compile_ui_shader()){fprintf(stderr,"ui shader failed\n");return -1;}
    init_shadow();

    glfwGetFramebufferSize(window,&fbW,&fbH);
    gen_ground();
    gen_sky_dome();
    gen_shadow_disc();
    init_text_vbo();
    bake_font(18);

    if (!load_character(glb_path)) {
        fprintf(stderr,"FATAL: could not load character GLB\n");
        return -1;
    }

    // Init cloth physics on the highest-material-index mesh (the suit)
    int cm_idx = -1;
    for (size_t i = 0; i < g_char_meshes.size(); i++)
        if (g_char_meshes[i].valid && g_char_meshes[i].material_index > cm_idx)
            cm_idx = (int)i;
    if (cm_idx >= 0 && !g_char_meshes[cm_idx].cloth_jw.empty()) {
        compute_skin(); // ensure g_bone_skin is valid for cloth init
        init_cloth(g_char_meshes[cm_idx]);
        fprintf(stderr,"cloth physics initialized on mesh %d (%d verts, %zu springs)\n",
            cm_idx, (int)g_char_meshes[cm_idx].cloth_pos.size(),
            g_char_meshes[cm_idx].spring_i0.size());
    }

    // Find armature node
    g_armature_node = g_data->skins[0].skeleton;
    if (!g_armature_node && g_data->skins[0].joints_count > 0) {
        cgltf_node* n = g_data->skins[0].joints[0];
        while (n->parent) n = n->parent;
        g_armature_node = n;
    }

    // Pre-bake cloth for all available animations
    if (cm_idx >= 0 && !g_char_meshes[cm_idx].cloth_jw.empty()) {
        for (int ai = 0; ai < (int)g_data->animations_count && ai < 4; ai++)
            bake_cloth_animation(ai);
        // Restore initial cloth pose from baked idle
        if (g_baked[0].ready && !g_baked[0].frames.empty())
            g_char_meshes[cm_idx].cloth_pos = g_baked[0].frames[0];
    }

    if (screenshot_mode) {
        // Extract base path (strip .bmp extension if present)
        char base[256]; strncpy(base, screenshot_path, 255); base[255]=0;
        char* dot = strrchr(base, '.'); if (dot && !strcmp(dot,".bmp")) *dot=0;

        // Ensure output directory exists
        char dir[256]; strncpy(dir, base, 255); dir[255]=0;
        char* slash = strrchr(dir, '\\');
        if (slash) { *slash = 0; _mkdir(dir); }

        struct { const char* name; float theta, phi, dist; vec3 pos; int anim; float anim_t; } shots[] = {
            {"front",     0.0f,   0.6f, 5.5f, {0,0,0}, 0, 0},
            {"left",     -1.57f,  0.6f, 5.5f, {0,0,0}, 0, 0},
            {"back",      3.14f,  0.6f, 5.5f, {0,0,0}, 0, 0},
            {"right",     1.57f,  0.6f, 5.5f, {0,0,0}, 0, 0},
            {"walk_fwd",  0.3f,   0.5f, 6.5f, {2,0,0}, 1, 0.5f},
            {"walk_side",-0.7f,   0.5f, 6.5f, {3,-1,0},1, 1.2f},
        };
        int w,h; glfwGetFramebufferSize(window,&w,&h);
        for (auto& s : shots) {
            cam_theta = s.theta; cam_phi = s.phi; cam_dist = s.dist;
            char_pos = s.pos;
            char_rot = s.theta + 3.14159f;
            cur_anim = s.anim; anim_dir = 1; anim_time = s.anim_t;
            update_cloth(0.016f);
            render();
            glfwSwapBuffers(window);
            unsigned char* pixels = (unsigned char*)malloc(w*h*3);
            glReadBuffer(GL_FRONT);
            glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,pixels);
            char path[300]; snprintf(path,300,"%s_%s.bmp",base,s.name);
            save_bmp(path, w, h, pixels);
            free(pixels);
            fprintf(stderr,"  saved %s\n", path);
        }
        glfwTerminate();
        return 0;
    }

    // Initialize editor with the loaded skeleton
    editor_init();

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,false);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glfwSetKeyCallback(window,on_key);
    glfwSetCursorPosCallback(window,on_cursor);
    glfwSetMouseButtonCallback(window,on_mouse);
    glfwSetScrollCallback(window,on_scroll);
    glfwSetCharCallback(window,on_char);
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow*,int w,int h){fbW=w;fbH=h;});

    fprintf(stderr,"Controls:\n");
    fprintf(stderr,"  F1       Toggle editor mode\n");
    fprintf(stderr,"  W/A/S/D  Rotate bone (X/Y axes)\n");
    fprintf(stderr,"  Q/E      Rotate bone (Z axis / roll)\n");
    fprintf(stderr,"  Shift    Fine rotation\n");
    fprintf(stderr,"  Ctrl     Coarse rotation\n");
    fprintf(stderr,"  1-9,0,-,= Select bone\n");
    fprintf(stderr,"  [ ]      Previous/next bone\n");
    fprintf(stderr,"  K        Insert keyframe\n");
    fprintf(stderr,"  Space    Play/Pause\n");
    fprintf(stderr,"  R        Reset selected bone\n");
    fprintf(stderr,"  <- ->    Step keyframes\n");
    fprintf(stderr,"  Ctrl+S   Save animation\n");
    fprintf(stderr,"  Ctrl+L   Load animation\n");
    fprintf(stderr,"  --screenshot [path]  Render a frame and save\n");

    double last=glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        double now=glfwGetTime();
        float dt=(float)(now-last);last=now;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui GUI
        {
            ImGui::Begin("Character Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("File: %s", glb_path ? glb_path : "?");
            ImGui::Text("Meshes: %zu  Bones: %d", g_data->meshes_count, g_num_bones);
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
                bool playing = editor_play;
                ImGui::Checkbox("Play", &editor_play);
                ImGui::SameLine();
                if (ImGui::SmallButton("|<")) anim_time = 0;
                ImGui::SameLine();
                if (ImGui::SmallButton(">|")) anim_time = editor_dur;
                float max_t = editor_dur > 0.001f ? editor_dur : 10.0f;
                ImGui::SliderFloat("Time", &anim_time, 0, max_t, "%.2fs");
                if (g_num_bones > 0) {
                    int sel = editor_sel;
                    const char* preview = g_data && g_data->skins_count > 0 && g_data->skins[0].joints[editor_sel]->name
                        ? g_data->skins[0].joints[editor_sel]->name : "?";
                    if (ImGui::BeginCombo("Edit Bone", preview)) {
                        for (int i = 0; i < g_num_bones; i++) {
                            const char* n = g_data->skins[0].joints[i]->name ? g_data->skins[0].joints[i]->name : "?";
                            if (ImGui::Selectable(n, i == editor_sel))
                                editor_sel = i;
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::Button("Reset Bone")) editor_reset_bone(editor_sel);
                }
            }

            if (ImGui::CollapsingHeader("Screenshot", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::Button("Save BMP")) {
                    int w,h; glfwGetFramebufferSize(window,&w,&h);
                    unsigned char* pixels = (unsigned char*)malloc(w*h*3);
                    glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,pixels);
                    save_bmp("imgui_screenshot.bmp", w, h, pixels);
                    free(pixels);
                }
            }

            if (ImGui::CollapsingHeader("UV/Material Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (g_material_count > 0) {
                    const char* preview = g_material_names[g_uv_view_mat];
                    if (ImGui::BeginCombo("Material", preview)) {
                        for (int i = 0; i < g_material_count; i++) {
                            if (ImGui::Selectable(g_material_names[i], i == g_uv_view_mat))
                                g_uv_view_mat = i;
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::ColorEdit3("Tint", &g_material_colors[g_uv_view_mat].x);

                    // Texture preview
                    GLuint tex = g_material_textures[g_uv_view_mat];
                    if (tex) {
                        GLint prev_w = 0, prev_h = 0;
                        glBindTexture(GL_TEXTURE_2D, tex);
                        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &prev_w);
                        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &prev_h);
                        if (prev_w > 0 && prev_h > 0) {
                            float aspect = (float)prev_w / prev_h;
                            float disp_w = 200, disp_h = disp_w / aspect;
                            if (disp_h > 200) { disp_h = 200; disp_w = disp_h * aspect; }
                            ImGui::Image((ImTextureID)(intptr_t)tex, ImVec2(disp_w, disp_h));
                        }
                    }

                    // UV wireframe overlay
                    bool found = false;
                    for (auto& cm : g_char_meshes) {
                        if (cm.material_index == g_uv_view_mat && cm.valid && !cm.uv_data.empty()) {
                            found = true;
                            ImVec2 canvas_sz(260, 260);
                            ImGui::Text("UV Layout");
                            ImGui::BeginChild("uv_canvas", canvas_sz, true);
                            ImDrawList* dl = ImGui::GetWindowDrawList();
                            ImVec2 org = ImGui::GetCursorScreenPos();
                            float pad = 5;
                            ImVec2 cmin = ImVec2(org.x + pad, org.y + pad);
                            ImVec2 cmax = ImVec2(org.x + canvas_sz.x - pad, org.y + canvas_sz.y - pad);
                            float scale = (cmax.x - cmin.x);
                            // Draw UV triangles
                            int num_tri = cm.idx_data.size() / 3;
                            for (int ti = 0; ti < num_tri; ti++) {
                                ImVec2 pts[3];
                                for (int k = 0; k < 3; k++) {
                                    int vi = cm.idx_data[ti*3 + k];
                                    float u = cm.uv_data[vi*2];
                                    float v = 1.0f - cm.uv_data[vi*2+1]; // flip V
                                    pts[k] = ImVec2(cmin.x + u * scale, cmin.y + v * scale);
                                }
                                dl->AddTriangle(pts[0], pts[1], pts[2], IM_COL32(100, 180, 255, 80), 0.8f);
                            }
                            ImGui::EndChild();
                            break;
                        }
                    }
                    if (!found) ImGui::Text("No UV data for this material");
                } else {
                    ImGui::Text("No materials loaded");
                }
            }

            ImGui::Separator();
            ImGui::Text("%.1f FPS (%.1f ms)", ImGui::GetIO().Framerate, 1000.0 / ImGui::GetIO().Framerate);
            ImGui::End();
        }

        update_physics(dt);
        update_cloth(dt);
        render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
