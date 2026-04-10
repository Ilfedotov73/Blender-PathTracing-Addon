#pragma once

#include "interval.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "aabb.h"

namespace render_core {
	class material;

	class hit_point {
	private:
		bool is_hit;
	public:
		vec3 p;
		vec3 normal;
		float intens = 1.0;

		hit_point(bool is_hit_) : is_hit(is_hit_) {}
		hit_point(bool is_hit_, vec3 p_, vec3 normal_) : is_hit(is_hit_), p(p_), normal(normal_) {}
		bool get_is_hit() { return is_hit; }
	};

	class hit_record {
	public:
		point3 p;
		vec3 normal;
		std::shared_ptr<material> mat; 

		float t;
		bool front_face;
		float u, v;

		void set_face_normal(const ray& r, const vec3& outward_normal)
		{
			front_face = dot(r.direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};

	class hittable {
	public:
		virtual ~hittable() = default;
		virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
		virtual aabb bounding_box() const = 0;
	};
}