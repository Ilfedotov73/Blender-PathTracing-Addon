#pragma once

#include <vector>
#include <memory>

#include "hittable.h"
#include "aabb.h"

namespace render_core {
	class hittable_list : public hittable {
	private:
		aabb bbox;
	public:
		std::vector<std::shared_ptr<hittable>> objects;

		hittable_list() {}
		hittable_list(std::shared_ptr<hittable> object) { add(object); }

		void add(std::shared_ptr<hittable> object)
		{
			objects.push_back(object);
			bbox = aabb(bbox, object->bounding_box());
		}
		void clear() { objects.clear(); }

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override
		{
			hit_record temp_rec;
			bool hit_anything = false;
			float closest_so_far = ray_t.max;

			for (const std::shared_ptr<hittable>& object : objects) {
				if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
					hit_anything = true;
					closest_so_far = temp_rec.t;
					rec = temp_rec;
				}
			}
			return hit_anything;
		}

		aabb bounding_box() const override { return bbox; }
	};
}