#pragma once

#include "rt_helper.h"

namespace render_core {
	class interval
	{
	public:
		float min, max;
		static const interval empty, universe;

		interval() : min(+INF), max(-INF) {} 
		interval(float min, float max) : min(min), max(max) {}
		interval(const interval& a, const interval& b)
		{
			min = a.min <= b.min ? a.min : b.min;
			max = a.max >= b.max ? a.max : b.max;
		}

		float size() const { return max - min; }

		bool contains(float x) const { return min <= x && x <= max; } 

		bool surrounds(float x) const { return min < x && x < max; } 

		float clamp(float x) const
		{
			if (x < min) { return min; }
			if (x > max) { return max; }
			return x;
		}

		interval expand(float delta) const
		{
			float padding = delta / 2.0f;
			return interval(min - padding, max + padding);
		}
	};
}