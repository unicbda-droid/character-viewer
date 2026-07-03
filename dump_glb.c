#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
int main() {
    cgltf_options opt = {0};
    cgltf_data* data = NULL;
    cgltf_result res = cgltf_parse_file(&opt, "run_speed.glb", &data);
    if (res != cgltf_result_success) { printf("parse failed: %d\n",res); return 1; }
    res = cgltf_load_buffers(&opt, data, "run_speed.glb");
    if (res != cgltf_result_success) { printf("load buffers failed\n"); return 1; }
    printf("nodes: %zu\n", data->nodes_count);
    printf("meshes: %zu\n", data->meshes_count);
    printf("skins: %zu\n", data->skins_count);
    printf("animations: %zu\n", data->animations_count);
    printf("buffer_views: %zu\n", data->buffer_views_count);
    printf("accessors: %zu\n", data->accessors_count);
    printf("buffers: %zu\n", data->buffers_count);
    for (cgltf_size i = 0; i < data->buffer_views_count; i++) {
        printf("bv[%zu]: offset=%zu size=%zu stride=%zu\n",
            i, data->buffer_views[i].offset, data->buffer_views[i].size,
            data->buffer_views[i].stride);
    }
    if (data->skins_count > 0) {
        cgltf_skin* skin = &data->skins[0];
        printf("skin joints: %zu\n", skin->joints_count);
        if (skin->inverse_bind_matrices) {
            cgltf_accessor* ibm = skin->inverse_bind_matrices;
            printf("IBM accessor: offset=%zu stride=%zu count=%zu bv_offset=%zu\n",
                ibm->offset, ibm->stride, ibm->count,
                ibm->buffer_view ? ibm->buffer_view->offset : 0);
            float* mat = (float*)((char*)ibm->buffer_view->buffer->data + ibm->offset);
            printf("  IBM[0]:\n");
            for (int r=0;r<4;r++) printf("    %f %f %f %f\n", mat[r*4], mat[r*4+1], mat[r*4+2], mat[r*4+3]);
        }
        for (cgltf_size i = 0; i < skin->joints_count && i < 5; i++) {
            printf("  joint[%zu]: %s parent=%s\n", i, skin->joints[i]->name,
                skin->joints[i]->parent ? skin->joints[i]->parent->name : "(null)");
            printf("    T=(%f,%f,%f) R=(%f,%f,%f,%f) S=(%f,%f,%f)\n",
                skin->joints[i]->translation[0], skin->joints[i]->translation[1], skin->joints[i]->translation[2],
                skin->joints[i]->rotation[0], skin->joints[i]->rotation[1], skin->joints[i]->rotation[2], skin->joints[i]->rotation[3],
                skin->joints[i]->scale[0], skin->joints[i]->scale[1], skin->joints[i]->scale[2]);
        }
    }
    if (data->animations_count > 0) {
        cgltf_animation* anim = &data->animations[0];
        printf("animation channels: %zu\n", anim->channels_count);
        for (cgltf_size i = 0; i < anim->channels_count && i < 6; i++) {
            cgltf_animation_channel* ch = &anim->channels[i];
            printf("  ch[%zu]: target=%s path=%d\n", i, ch->target_node ? ch->target_node->name : "(null)", ch->target_path);
            if (ch->sampler) {
                printf("    input count=%zu offset=%zu stride=%zu bv_off=%zu\n",
                    ch->sampler->input->count, ch->sampler->input->offset, ch->sampler->input->stride,
                    ch->sampler->input->buffer_view ? ch->sampler->input->buffer_view->offset : 0);
                printf("    output count=%zu offset=%zu stride=%zu bv_off=%zu\n",
                    ch->sampler->output->count, ch->sampler->output->offset, ch->sampler->output->stride,
                    ch->sampler->output->buffer_view ? ch->sampler->output->buffer_view->offset : 0);
            }
        }
    }
    if (data->meshes_count > 0) {
        cgltf_mesh* mesh = &data->meshes[0];
        for (cgltf_size pi = 0; pi < mesh->primitives_count; pi++) {
            cgltf_primitive* prim = &mesh->primitives[pi];
            printf("prim[%zu]: attributes=%zu\n", pi, prim->attributes_count);
            for (cgltf_size ai = 0; ai < prim->attributes_count; ai++) {
                cgltf_attribute* attr = &prim->attributes[ai];
                cgltf_accessor* acc = attr->data;
                printf("  attr[%zu]: name=%s comp=%d type=%d count=%zu offset=%zu stride=%zu bv_off=%zu\n",
                    ai, attr->name, acc->component_type, acc->type, acc->count,
                    acc->offset, acc->stride,
                    acc->buffer_view ? acc->buffer_view->offset : 0);
            }
        }
    }
    cgltf_free(data);
    return 0;
}
