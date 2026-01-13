#pragma once

#include <cmath>
#include <memory>

#include "vec3.h"
#include "hittable.h"
#include "material.h"
#include "aabb.h"

namespace render_core {

    class plane : public hittable
    {
    private:
        point3 Q;
        vec3 u, v;
        vec3 w;
        std::shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        float D;
    public:
        plane(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat)
            : Q(Q), u(u), v(v), mat(mat)
        {
            vec3 n = cross(u, v);
            normal = unitv(n);
            D = -dot(normal, Q);
            w = n / dot(n, n);

            set_bounding_box();
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override
        {
            float denom = dot(normal, r.direction());

            if (std::fabs(denom) < 1e-8f) { return false; }
            float t = (-D - dot(normal, r.origin())) / denom;

            if (!ray_t.contains(t)) { return false; }

            point3 intersection = r.at(t);

            vec3 planar_hitpt_vec = intersection - Q;
            float alpha = dot(w, cross(planar_hitpt_vec, v));
            float beta = dot(w, cross(u, planar_hitpt_vec));

            if (!is_interior(alpha, beta, rec)) { return false; }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        virtual void set_bounding_box()
        {
            aabb bbox_diagonal1 = aabb(Q, Q + u + v);
            aabb bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        virtual bool is_interior(float a, float b, hit_record& rec) const {
            interval unit_interval = interval(0.0f, 1.0f);

            if (!unit_interval.contains(a) || !unit_interval.contains(b)) { return false; }
            rec.u = a;
            rec.v = b;
            return true;
        }
    };
}