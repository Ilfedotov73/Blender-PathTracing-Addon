#pragma once

#include <iostream>

#include "interval.h"

namespace render_core {
	using color = vec3;

	inline float liner_to_gamma(float linear_component)
	{
		if (linear_component > 0) { return std::sqrt(linear_component); }
		return 0.0f;
	}

	inline void write_color(std::ostream& out, const color& pix_color)
	{
		float r = pix_color.x();
		float g = pix_color.y();
		float b = pix_color.z();

		r = liner_to_gamma(r);
		g = liner_to_gamma(g);
		b = liner_to_gamma(b);

		static const interval intensity(0.000f, 0.999f);
		int rbyte = int(255.999f * intensity.clamp(r));
		int gbyte = int(255.999f * intensity.clamp(g));
		int bbyte = int(255.998f * intensity.clamp(b));

		out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
	}
}
