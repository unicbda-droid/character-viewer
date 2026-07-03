#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

static cgltf_accessor* ga(cgltf_primitive* prim, const char* name) {
    for (cgltf_size a=0; a<prim->attributes_count; a++)
        if (!strcmp(prim->attributes[a].name, name))
            return prim->attributes[a].data;
    return NULL;
}

int main() {
    cgltf_options opt = {0};
    cgltf_data* data = NULL;
    cgltf_parse_file(&opt, "run_speed.glb", &data);
    cgltf_load_buffers(&opt, data, "run_speed.glb");

    cgltf_skin* skin = &data->skins[0];
    cgltf_mesh* mesh = &data->meshes[0];
    cgltf_primitive* prim = &mesh->primitives[0];
    cgltf_animation* anim = &data->animations[0];
    cgltf_accessor* ibm = skin->inverse_bind_matrices;

    printf("=== IBM[0] ===\n");
    float ibm0[16];
    cgltf_accessor_read_float(ibm, 0, ibm0, 16);
    for (int r=0;r<4;r++) printf("  %f %f %f %f\n", ibm0[r*4], ibm0[r*4+1], ibm0[r*4+2], ibm0[r*4+3]);

    printf("=== IBM[1] ===\n");
    float ibm1[16];
    cgltf_accessor_read_float(ibm, 1, ibm1, 16);
    for (int r=0;r<4;r++) printf("  %f %f %f %f\n", ibm1[r*4], ibm1[r*4+1], ibm1[r*4+2], ibm1[r*4+3]);

    cgltf_accessor* pos_a = ga(prim, "POSITION");
    cgltf_accessor* norm_a = ga(prim, "NORMAL");
    cgltf_accessor* uv_a = ga(prim, "TEXCOORD_0");
    cgltf_accessor* joints_a = ga(prim, "JOINTS_0");
    cgltf_accessor* weights_a = ga(prim, "WEIGHTS_0");

    printf("\n=== First 3 vertices (correct data) ===\n");
    float pos[3], norm[3], uv[2];
    float weights[4];
    unsigned char joints[4];
    unsigned char* jraw = (unsigned char*)((char*)joints_a->buffer_view->buffer->data + joints_a->buffer_view->offset + joints_a->offset);
    for (int vi=0; vi<3; vi++) {
        cgltf_accessor_read_float(pos_a, vi, pos, 3);
        cgltf_accessor_read_float(norm_a, vi, norm, 3);
        cgltf_accessor_read_float(uv_a, vi, uv, 2);
        cgltf_accessor_read_float(weights_a, vi, weights, 4);
        for (int j=0;j<4;j++) joints[j] = jraw[vi*joints_a->stride + j];
        printf("v%d: pos=(%+.2f,%+.2f,%+.2f) norm=(%+.2f,%+.2f,%+.2f) uv=(%+.2f,%+.2f)\n",
            vi, pos[0], pos[1], pos[2], norm[0], norm[1], norm[2], uv[0], uv[1]);
        printf("     joints=(%d,%d,%d,%d) weights=(%.3f,%.3f,%.3f,%.3f)\n",
            joints[0], joints[1], joints[2], joints[3],
            weights[0], weights[1], weights[2], weights[3]);
    }

    printf("\n=== Old broken access (no buffer_view offset) ===\n");
    float* old_norm = (float*)((char*)norm_a->buffer_view->buffer->data + norm_a->offset);
    unsigned char* old_joints = (unsigned char*)((char*)joints_a->buffer_view->buffer->data + joints_a->offset);
    printf("  (reads from buffer offset 0 = position data)\n");
    for (int vi=0; vi<3; vi++) {
        printf("v%d: old_norm=(%+.2f,%+.2f,%+.2f) old_joints=(%d,%d,%d,%d)\n",
            vi, old_norm[vi*norm_a->stride/4], old_norm[vi*norm_a->stride/4+1], old_norm[vi*norm_a->stride/4+2],
            old_joints[vi*joints_a->stride], old_joints[vi*joints_a->stride+1],
            old_joints[vi*joints_a->stride+2], old_joints[vi*joints_a->stride+3]);
    }

    printf("\n=== Validation: joint indices in range [0,68] ===\n");
    int bad = 0;
    for (int vi=0; vi<(int)joints_a->count; vi++) {
        for (int j=0;j<4;j++) {
            int idx = jraw[vi*joints_a->stride + j];
            if (idx < 0 || idx >= (int)skin->joints_count) bad++;
        }
    }
    size_t total = joints_a->count * 4;
    printf("Total joint assignments: %zu, out-of-range: %d (%.1f%%)\n", total, bad, bad*100.0/total);

    printf("\n=== Animation: Root translation keyframes ===\n");
    for (cgltf_size ci=0; ci<anim->channels_count; ci++) {
        cgltf_animation_channel* ch = &anim->channels[ci];
        if (ch->target_node == skin->joints[0] && ch->target_path == cgltf_animation_path_type_translation) {
            float out[3];
            cgltf_accessor_read_float(ch->sampler->output, 0, out, 3);
            printf("  frame 0: (%+.4f, %+.4f, %+.4f)\n", out[0], out[1], out[2]);
            cgltf_accessor_read_float(ch->sampler->output, 1, out, 3);
            printf("  frame 1: (%+.4f, %+.4f, %+.4f)\n", out[0], out[1], out[2]);
            break;
        }
    }

    cgltf_free(data);
    return 0;
}
