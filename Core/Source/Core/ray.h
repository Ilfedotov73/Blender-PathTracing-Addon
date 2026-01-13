#pragma once

#include "vec3.h"

namespace render_core{
	class ray
	{
	private:
		point3 orig;  
		vec3 dir;    
		float tm;
	public:
		ray() {}
		ray(const point3& origin, const vec3& direction, float time) : orig(origin), dir(direction), tm(time) {}
		ray(const point3& origin, const vec3& direction) : ray(origin, direction, 0.0f) {}

		const point3& origin() const { return orig; }
		const vec3& direction() const { return dir; }
		float time() const { return tm; }

		point3 at(float t) const { return orig + t * dir; }
	};
}
