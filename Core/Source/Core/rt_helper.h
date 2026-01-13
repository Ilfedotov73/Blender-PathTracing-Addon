#pragma once

#include <cstdlib>
#include <limits>

namespace render_core {
	const float INF = std::numeric_limits<float>::infinity();
	const float PI = 3.1415926535897932385f;

	inline float degrees_to_radians(float degrees) { return degrees * PI / 180.0f; }

	inline float random_float() { return std::rand() / (RAND_MAX + 1.0f); } 
	
	inline float random_float(float min, float max) { return min + (max - min) * random_float(); } 
	
	inline int random_int(int min, int max) { return int(random_float(min, max + 1)); }
}
