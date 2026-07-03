#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

int main() {
    cgltf_options opt = {0};
    cgltf_data* data = NULL;
    cgltf_parse_file(&opt, "run_speed.glb", &data);
    cgltf_load_buffers(&opt, data, "run_speed.glb");

    printf("materials: %zu\n", data->materials_count);
    printf("textures: %zu\n", data->textures_count);
    printf("images: %zu\n", data->images_count);

    for (cgltf_size i = 0; i < data->materials_count; i++) {
        cgltf_material* m = &data->materials[i];
        printf("\nmaterial[%zu]: name=%s\n", i, m->name ? m->name : "(null)");
        printf("  pbr: bc=%d mr=%d\n",
            m->has_pbr_metallic_roughness,
            m->has_pbr_specular_glossiness);
        if (m->has_pbr_metallic_roughness) {
            cgltf_pbr_metallic_roughness* pbr = &m->pbr_metallic_roughness;
            printf("  base_color: (%.3f,%.3f,%.3f,%.3f)\n",
                pbr->base_color_factor[0], pbr->base_color_factor[1],
                pbr->base_color_factor[2], pbr->base_color_factor[3]);
            if (pbr->base_color_texture.texture) {
                cgltf_image* img = pbr->base_color_texture.texture->image;
                printf("  base_color_texture: image=%s\n", img ? (img->name ? img->name : "(unnamed)") : "(no image)");
                printf("    uri=%s\n", img && img->uri ? img->uri : "(no uri / embedded)");
                printf("    buffer_view=%s\n", img && img->buffer_view ? "yes (embedded)" : "no");
            }
            if (pbr->metallic_roughness_texture.texture) {
                printf("  metallic_roughness_texture: present\n");
            }
        }
    }

    for (cgltf_size i = 0; i < data->images_count; i++) {
        cgltf_image* img = &data->images[i];
        printf("\nimage[%zu]: name=%s uri=%s\n", i,
            img->name ? img->name : "(null)",
            img->uri ? img->uri : "(null/embedded)");
        printf("  buffer_view: %s\n", img->buffer_view ? "yes" : "no");
        if (img->buffer_view)
            printf("  size: %zu bytes, offset: %zu\n",
                img->buffer_view->size, img->buffer_view->offset);
    }

    // Also print all meshes and primitives with their material indices
    for (cgltf_size mi = 0; mi < data->meshes_count; mi++) {
        cgltf_mesh* mesh = &data->meshes[mi];
        printf("\nmesh[%zu]: name=%s\n", mi, mesh->name ? mesh->name : "(null)");
        for (cgltf_size pi = 0; pi < mesh->primitives_count; pi++) {
            cgltf_primitive* prim = &mesh->primitives[pi];
            cgltf_material* mat = prim->material;
            printf("  prim[%zu]: material=%s\n", pi,
                mat ? (mat->name ? mat->name : "(unnamed)") : "(null)");
        }
    }

    cgltf_free(data);
    return 0;
}
