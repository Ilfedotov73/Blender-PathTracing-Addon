#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <memory>

#include "rt_obj_loader.h"

#include "vec3.h"
#include "hittable_list.h"
#include "color.h"
#include "material.h"
#include "texture.h"
#include "tri.h"

namespace render_core {
    obj_loader::obj_loader() { sides = std::make_shared<hittable_list>(); }

    void obj_loader::load_models(const char* filename, const char* mtlsdir)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, mtlsdir)) {
            std::cerr << warn << " " << err << ".\n";
        }

        for (size_t s = 0, s_size = shapes.size(); s < s_size; ++s) {
            std::cerr << "Shape:" << s << "\n";

            int material_id = shapes[s].mesh.material_ids[0];
            std::shared_ptr<material> surface;
            if (material_id >= 0) {
                tinyobj::material_t mat = materials[material_id];
                if (mat.diffuse_texname != "") {
                    std::shared_ptr<texture> tex = std::make_shared<image_texture>(mat.diffuse_texname.c_str());
                    surface = std::make_shared<lambertian>(tex);
                }
                else {
                    surface = std::make_shared<lambertian>(color(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]));
                }
            }
            else {
                surface = std::make_shared<lambertian>(color(.8f, .8f, .8f));
            }

            size_t index_offset = 0;
            for (size_t f = 0, size = shapes[s].mesh.num_face_vertices.size(); f < size; ++f) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
                std::vector<float> quad_vertices;
                std::vector<std::pair<float, float>> quad_texcoords;

                for (size_t v = 0; v < fv; ++v) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                        quad_texcoords.push_back(std::make_pair(tx, ty));
                    }

                    quad_vertices.push_back(vx);
                    quad_vertices.push_back(vy);
                    quad_vertices.push_back(vz);
                }
                index_offset += fv;

                point3 Q = point3(quad_vertices[0], quad_vertices[1], quad_vertices[2]);
                point3 U = point3(quad_vertices[3], quad_vertices[4], quad_vertices[5]) - Q;
                point3 V = point3(quad_vertices[6], quad_vertices[7], quad_vertices[8]) - Q;

                sides->add(std::make_shared<tri>(Q, U, V, surface, quad_texcoords));

                std::cerr << "Vertices for a triangle polygon" << f << ": ";
                for (int i = 0; i < fv; ++i) {
                    std::cerr << "(" << quad_vertices[i * 3] << quad_vertices[i * 3 + 1] << quad_vertices[i * 3 + 2] << ")" << " ";
                }
                std::cerr << '\n';
            }
        }
    }

    std::shared_ptr<hittable_list> obj_loader::get_sides() { return sides; }
}